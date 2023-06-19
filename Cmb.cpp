#include "Cmb_i.h"

// constructor and destructor
Cmb::Cmb()
  :
    mName("universe"),
    mVerbose(10),
    mTopoType(TORUS)
{
    // initialize the RNG
    mRandom.seed(42);    // resets the random generator

    // set some default values
    maxl(MAXL);
    maxn(0); // determined automatically
    LSS(1.56);
    L(1.0,1.0,1.0);
    
}
Cmb::~Cmb()
{
}

int Cmb::maxl()
{
    return mMaxL;
}

int Cmb::maxl(int pMaxL)
{
    // changing maxl deletes the alm database, makes mCmbData uncorrect
    mMaxL    = pMaxL;
    mAlm.setl(mMaxL);
    return mMaxL;
}

int Cmb::maxn()
{
    return mMaxN;
}

int Cmb::maxn(int pMaxN)
{
    mMaxN    = pMaxN;
    if(mMaxN > 128) {
        cerr << "#WARNING nMax too big? nMax:" << mMaxN << " database size:" << 8*mMaxN*mMaxN*mMaxN*8 << endl;
        cout << "#WARNING nMax too big? nMax:" << mMaxN << " database size:" << 8*mMaxN*mMaxN*mMaxN*8 << endl;
    }

    // propagate the maximum size to Ek which actually holds the
    // ξ values
    mEk.size(mMaxN);

    return mMaxN;
}

Cmb::topologies Cmb::topology()
{
    return mTopoType;
}

Cmb::topologies Cmb::topology(topologies pTopoType)
{
    // setting topo makes ek,alm,mCmbData useless
    mTopoType = pTopoType;
    return mTopoType;
}

double Cmb::L(int ll)
{
    return mL[ll];
}

double Cmb::L(int ll, double l)
{
    if ( (ll>=0) && (ll<3) )
    {
        mL[ll]=l;
    }
    return volume();
}

double Cmb::L(double l0, double l1, double l2)
{
    mL[0] = l0;
    mL[1] = l1;
    mL[2] = l2;
    return volume();
}

double Cmb::LSS()
{
    return mLSS;
}

double Cmb::LSS(double pLSS)
{
    mLSS = pLSS;
    return mLSS;
}

double Cmb::maxKLSS()
{
    return mMaxKLSS;
}

double Cmb::maxKLSS(double pMaxKLSS)
{
    mMaxKLSS = pMaxKLSS;
    return mMaxKLSS;
}

double Cmb::uberL()
{
    return mUberL;
}

double Cmb::uberL(double pUberL)
{
    mUberL = pUberL;
    return mUberL;
}

int Cmb::calculateMaxN(){
    // We want to sample at least to k*LSS = mMaxKLSS because otherwise
    // we use too few periods of bessel (about 4 periods is enough)
    // but not more then maxN = MaxNUlti because the computer hasn't
    // got enough memory
    int MaxNUlti = 128;
    // but we want at least maxNMini n's to be sampled so it
    // is statistically stable
    int MaxNMini = 8;
    
    // first calculate an upper bound for MaxN, based on the largest
    // size we have in the simulation, Luber, so that even that size
    // covers several bessel periods
    double Lmax = max(max(mL[0], mL[1]), mL[2]);
    int pMaxN = static_cast<int>(Lmax * mMaxKLSS / mLSS / CIRC);

    // calculate how many n-samples we actualy have in the lowest L
    // case
    double Lmin = min(min(mL[0], mL[1]), mL[2]);
    double lMaxN = static_cast<int>(Lmin * mMaxKLSS / mLSS / CIRC);
    
    // too few samples in total (this will not happen often)
    if(pMaxN < MaxNMini)
    {
        pMaxN = MaxNMini;
        cout << "#WARNING MaxN too low. setting maxN:" << pMaxN << endl;
        cerr << "#WARNING MaxN too low. setting maxN:" << pMaxN << endl;
    }
    
    // Although we have samples, they are not allowed because they
    // reach too far into the bessel function,
    // -> allow them anyway up to MaxNMini, this does require
    // extensive bessel querying.

    if(lMaxN < MaxNMini)
    {
        mMaxKLSS = static_cast<double>(MaxNMini) * CIRC * mLSS / Lmin;
        cout << "#WARNING maxKLSS too low. setting maxKLSS:" << mMaxKLSS << endl;
        cerr << "#WARNING maxKLSS too low. setting maxKLSS:" << mMaxKLSS << endl;
        pMaxN = static_cast<int>(Lmax*mMaxKLSS/mLSS/CIRC);
    }

    // In any case, do not make maxN too large, because (2n)^3 complex
    // doubles are stored as random variables.
    if(pMaxN > MaxNUlti)
    {
        pMaxN = MaxNUlti;
        mMaxKLSS = static_cast<double>(pMaxN) * CIRC * mLSS / mUberL;
        cout << "#WARNING MaxN too high. setting maxN:" << pMaxN << " maxKLSS:" << mMaxKLSS << endl;
        cerr << "#WARNING MaxN too high. setting maxN:" << pMaxN << " maxKLSS:" << mMaxKLSS << endl;
    }

    return maxn(pMaxN);
}

string Cmb::name()
{
    return mName;

}

string Cmb::name(string pName)
{
    mName = pName;
    return mName;
}

int Cmb::verbose()
{
    return mVerbose;
}

int Cmb::verbose(int pVerbose)
{
    mVerbose = pVerbose;
    return mVerbose;
}


// spherical harmonics

dcomp Cmb::spham(int l, int m, Coordinate coor)
{
    /*
             ┌────┐  ┌──────┐
             │2l+1   │(l-m)!    m          imΦ
    Y(θ,Φ) = │──── * │────── * P (cos(θ)) e
     lm      ⎷ 4π    ⎷(l+m)!    l

            ╰──────────────┬─────────────╯
          gsl_sf_legendre_sphPlm(l,m,cos(θ))    

    where θ is the zenith/polar_angle/colatitude [0, π]
    and   Φ is the azimuth/longitude             [0,2π)

                m    *
    Y     = (-1)  * Y
     l,-m            m,-l
    */

    dcomp xiv;
    xiv  = dcomp(
        gsl_sf_legendre_sphPlm(l, abs(m), cos(coor.sphericalZenith())),
        0
    );
    xiv *= exp(dcomp(0, coor.sphericalAzimuth() * abs(m)));
    if (m<0) xiv = pow(double(-1.0), m) * conj(xiv);
    return xiv;
}

// test whether this ξ should be calculated
int    Cmb::xiitest(int nx, int ny, int nz)
{
    switch (mTopoType){
        case EUCLIDIAN:
            //Euclidian, not discrete
            return 1;
            break;
        case TORUS:
            return 1;
            break;
        case HALFTURN:
            if ((nx > 0) || ((nx == 0) && (ny > 0))) {
                return 1;
            } else if ((nx==0) && (ny == 0) && (nz % 2 == 0)) {
                return 1;
            }
            break;
        case QUARTERTURN:
            if ((nx > 0) && (ny >= 0)) {
                return 1;
            } else if ((nx == 0) && (ny == 0) && (nz % 4 == 0)) {
                return 1;
            }
            break;
        case THIRDTURN:
            if ((nx > 0) && (ny >= 0) && (ny < nx)) {
                return 1;
            } else if ((nx == 0) && (ny == 0) && ((nz % 6) == 0)) {
                return 1;
            }
            break;
        case SIXTHTURN:
            if ((nx > 0) && (ny >= 0)) {
                return 1;
            } else if ((nx == 0) && (ny == 0) && ((nz % 3) == 0)) {
                return 1;
            }
            break;
        case HANTZSCHEWENDT:
            if (((nx > 0) && (ny > 0)) || ((nx == 0) && (ny > 0) && (nz > 0)) || ((nx > 0) && (ny == 0) && (nz > 0))) {
                return 1;
            } else if ((nx % 2 == 0) && (ny == 0) && (nz == 0)) {
                return 1;
            } else if ((nx == 0) && (ny % 2 == 0) && (nz == 0)) {
                return 1;
            }
            else if ((nx == 0) && (ny == 0) && (nz % 2 == 0)) {
                return 1;
            }
            break;
    }
    return 0;    
}

// count how many ξ's we will calculate
int    Cmb::xiicount()
{
    int counter = 0;
    for (int nx=-mMaxN; nx<=mMaxN; nx++) {
        for (int ny=-mMaxN; ny<=mMaxN; ny++) {
            for (int nz=-mMaxN; nz<=mMaxN; nz++) {
                if (xiitest(nx,ny,nz)) {
                    counter++;
                }
            }
        }
    }
    return counter;
}

dcomp  Cmb::xii(int nx, int ny, int nz, int l, int m)
{
    /* This function is the heart of the program, it returns the discrete xi^k_klm values
    */

    double x, y, z;
    Coordinate coor;
    double circ = 6.2831853071795862;
    double two = 2.0;
    double sqrt2 = sqrt(two);
    
    dcomp xiv = dcomp(0,0);
    dcomp ci = dcomp(0,1);
    
    switch (mTopoType){
        case EUCLIDIAN:
            //Euclidian, not discrete
            break;
        case TORUS:
            // Torus
            coor.cartesian(
                circ * nx/mL[0],
                circ * ny/mL[1],
                circ * nz/mL[2]
            );
            xiv = pow(ci,l) * conj(spham(l, m, coor));
            break;
        case HALFTURN:
            // Half turn space
            if ((nx > 0) || ((nx == 0) && (ny > 0))) {
                coor.cartesian(
                    circ * nx/mL[0],
                    circ * ny/mL[1],
                    circ * nz/mL[2]
                ); // mL[2] is twice as big as mL[2] from torus
                xiv = pow(ci, l) / sqrt2 * conj(spham(l, m, coor));
                coor.cartesian(
                    -circ * nx/mL[0],
                    -circ * ny/mL[1],
                    circ * nz/mL[2]
                );
                xiv += pow(ci, l) / sqrt2 * pow(-1., nz) * conj(spham(l, m, coor));
            } else if ((nx == 0) && (ny == 0) && (nz % 2 == 0)) {
                coor.cartesian(
                    circ * nx/mL[0],
                    circ * ny/mL[1],
                    circ * nz/mL[2]
                );
                xiv = pow(ci, l) * conj(spham(l, m, coor));
            } else {
                // hmm what then?
                xiv = dcomp (0,0);
            }
            break;
        case QUARTERTURN:
            x = circ * nx/mL[0]; // if Lx=1
            y = circ * ny/mL[1]; // and Ly=1
            z = circ * nz/mL[2]; // and Lz=4 then the FD has size 1
            if ((nx > 0) && (ny >= 0)) {
                xiv = pow(ci,l) / 2. * (
                    conj(spham(l,m, coor.cartesian( x, y, z))) +
                    pow(ci,  nz) * conj(spham(l,m, coor.cartesian( y,-x, z))) +
                    pow(ci,2*nz) * conj(spham(l,m, coor.cartesian(-x,-y, z))) +
                    pow(ci,3*nz) * conj(spham(l,m, coor.cartesian(-y, x, z)))
                );
            } else if ((nx == 0) && (ny == 0) && ((nz % 4) == 0)) {
                xiv = pow(ci, l) * conj(spham(l, m, coor.cartesian(x, y, z)));
            } else {
                xiv = dcomp (0,0);
            }
            break;
        case THIRDTURN:
            /* It is not usefull to use names as nx/ny/nz. n1/n2/n3
             * would be better. Also there are only 2 L values.
             */
            if ((nx > 0) && (ny >= 0)) {
                xiv = pow(ci, l) / sqrt(3.) * (
                    conj(spham(l, m, coor.cartesian(
                        (-ny) / mL[0],
                        (2 * nx - ny) / (sqrt(3.) * mL[0]),
                        nz / mL[2])
                    )) + 
                    exp(dcomp(0, nz * circ/3.)) * 
                    conj(spham(l, m, coor.cartesian(
                        (nx) / mL[0],
                        (2 * ny - nx) / (sqrt(3.) * mL[0]),
                        nz/mL[2])
                    )) +
                    exp(dcomp(0, 2. * nz * circ / 3.)) * 
                    conj(spham(l, m, coor.cartesian(
                        (ny - nx) / mL[0],
                        (-nx - ny) / (sqrt(3.) * mL[0]),
                        nz/mL[2])
                    ))
                );
            }
            else if ((nx == 0) && (ny == 0) && ((nz % 3) == 0)) {
                xiv = pow(ci,l) * conj(spham(l, m, coor.cartesian(
                    0, 0, nz/mL[2])
                ));
            }
            break;
        case SIXTHTURN:
            // Use long lines to read properly.
            if ((nx > 0) && (ny >= 0) && (ny < nx)) {
                xiv = pow(ci, l) / sqrt(6.) * (
                                                  conj(spham(l, m, coor.cartesian((   -ny) / mL[0], (2*nx-ny) / (sqrt(3.) * mL[0]), nz/mL[2])))
                  + exp(dcomp(0,   nz*circ/6.)) * conj(spham(l, m, coor.cartesian(( nx-ny) / mL[0], (  nx+ny) / (sqrt(3.) * mL[0]), nz/mL[2])))
                  + exp(dcomp(0,2.*nz*circ/6.)) * conj(spham(l, m, coor.cartesian(( nx   ) / mL[0], (2*ny-nx) / (sqrt(3.) * mL[0]), nz/mL[2])))
                  + exp(dcomp(0,3.*nz*circ/6.)) * conj(spham(l, m, coor.cartesian((    ny) / mL[0], (ny-2*nx) / (sqrt(3.) * mL[0]), nz/mL[2])))
                  + exp(dcomp(0,4.*nz*circ/6.)) * conj(spham(l, m, coor.cartesian((-nx+ny) / mL[0], (-nx- ny) / (sqrt(3.) * mL[0]), nz/mL[2])))
                  + exp(dcomp(0,5.*nz*circ/6.)) * conj(spham(l, m, coor.cartesian((-nx   ) / mL[0], (nx-2*ny) / (sqrt(3.) * mL[0]), nz/mL[2])))
                );
            } else if ((nx == 0) && (ny == 0) && ((nz % 6) == 0)) {
                xiv = pow(ci,l) * conj(spham(l, m, coor.cartesian(0, 0, nz/mL[2])));
            }
            break;
        case HANTZSCHEWENDT:
            if (((nx > 0) && (ny > 0)) || ((nx == 0) && (ny > 0) && (nz > 0)) || ((nx > 0) && (ny == 0) && (nz > 0))) {
                xiv = pow(ci, l) / 2. * (
                                        conj(spham(l, m, coor.cartesian( nx/mL[0],  ny/mL[1],  nz/mL[2])))
                    + pow(-1., nx-ny) * conj(spham(l, m, coor.cartesian( nx/mL[0], -ny/mL[1], -nz/mL[2])))
                    + pow(-1., ny-nz) * conj(spham(l, m, coor.cartesian(-nx/mL[0],  ny/mL[1], -nz/mL[2])))
                    + pow(-1., nz-nx) * conj(spham(l, m, coor.cartesian(-nx/mL[0], -ny/mL[1],  nz/mL[2])))
                );
            } else if ((nx > 0) && (nx % 2 == 0) && (ny == 0) && (nz == 0)) {
                xiv = pow(ci,l) / sqrt(2.0) * (
                      conj(spham(l, m, coor.cartesian( nx/mL[0], 0, 0)))
                    + conj(spham(l, m, coor.cartesian(-nx/mL[0], 0, 0)))
                );
            } else if ((nx == 0) && (ny > 0) && (ny % 2 == 0) && (nz == 0)) {
                xiv = pow(ci, l) / sqrt(2.0) * (
                      conj(spham(l, m, coor.cartesian(0,  ny/mL[1], 0)))
                    + conj(spham(l, m, coor.cartesian(0, -ny/mL[1], 0)))
                );
            } else if ((nx == 0) && (ny == 0) && (nz > 0) && (nz % 2 == 0)) {
                xiv = pow(ci, l) / sqrt(2.0) * (
                      conj(spham(l, m, coor.cartesian(0, 0,  nz/mL[2])))
                    + conj(spham(l, m, coor.cartesian(0, 0, -nz/mL[2])))
                );
            }
            break;
        default:
            // should not happen
            xiv = dcomp(0, 0);
    }
    return xiv;
}

double Cmb::powerk(double k)
{
    /* Power that wavenumber k has. */
    /*
        FORTRAN:

      dk=twopi/boxlen
      akmax=n1*dk

      do k3=1,n1
       do 180 k2=1,n1
        do 170 k1=2,n1/2

          ak3=(k3-1)*dk
          if (k3 > n12) ak3=ak3-akmax
          ak2=(k2-1)*dk
          if (k2 > n12) ak2=ak2-akmax
          ak1=(k1-1)*dk
          k=k1 + (k2-1)*n1/2 + (k3-1)*n1*n1/2

          akk=ak1*ak1 + ak2*ak2 + ak3*ak3
          ak  = sqrt(akk)

          power(k)=amplit*primor(ak)*(trans(ak)**2)*d3k

         continue
        continue
       continue

      PRIMOR(XK)=XK**PRIMN // primn between -3 and 0
      TRANS(ak) = 1        // for power spectr
    */

    double pwk;
    
    // Harrison-Zel'dovich
    pwk = pow(k,-3);
    // Other Power spectra
    //pwk = 1;
    //pwk = pow(k,-4);

    // Normalisation should go here, but doesn't matter for
    // this project.
    //pwk *= 1;
    
    // From the Fortran code
    if (k == 0) pwk = 0;
    
    return pwk;
}

double Cmb::kk(int nx, int ny, int nz) {
    /* returns the absolute value of the k vector */
    double k;
    switch (mTopoType) {
        case EUCLIDIAN:
            // Doesn't make sense.
            k = 0;
            break;
        case TORUS:
        case HALFTURN:
        case QUARTERTURN:
        case HANTZSCHEWENDT:
            /*      ╭            ╮
                    │ nx  ny  nz │
               k=2π │ ──, ──, ── │
                    │ Lx  Ly  Lz │
                    ╰            ╯  */
            k = CIRC * sqrt(
                (nx / mL[0]) * (nx / mL[0]) +
                (ny / mL[1]) * (ny / mL[1]) +
                (nz / mL[2]) * (nz / mL[2])
            );
            break;
        case THIRDTURN:
        case SIXTHTURN:
            // Does this really work well? Shouldn't this depend on
            // the M matrix as well?
            k = CIRC * sqrt(
                pow( (-ny) / mL[0],2) + 
                pow( (2 * nx - ny) / (sqrt(3.) * mL[0]), 2) +
                pow( nz / mL[2], 2)
            );
            break;
        default:
            // Unknown
            k = 0;
            break;
    }
    return k;
}

double Cmb::volume(){
    double vol;
    switch (mTopoType) {
        case EUCLIDIAN:
        // Euclidian volume is infinite!!
        vol = 1;
        break;
    case TORUS:
        vol = mL[0] * mL[1] * mL[2];
        break;
    case HALFTURN:
        vol = mL[0] * mL[1] * mL[2] / 2;
        break;
    case QUARTERTURN:
        vol = mL[0] * mL[1] * mL[2] / 4;
        break;
    case THIRDTURN:
        vol = mL[0] * mL[0] * mL[2] / 3;
        break;
    case SIXTHTURN:
        vol = mL[0] * mL[0] * mL[2] / 6;
        break;
    case HANTZSCHEWENDT:
        vol = mL[0] * mL[1] * mL[2] / 8;
        break;
    default:
        // Unknown
        vol = 1;
        break;
    }
    mVol = vol;
    return vol;
}

// filling the arrays
void   Cmb::build()
{
    this->buildek();
    this->buildalm();
    this->buildcmb();
}

void   Cmb::buildek()
{
    Counter ekcounter("Building ek:  ",(mMaxN*2+1)*(mMaxN*2+1),3);

    dcomp sp;
    Coordinate coo;

    // clearing ek first
    mEk.clear();
    
    if (mVerbose > 5)
        ekcounter.printini();
    
    for (int nx=-mMaxN; nx<=mMaxN; nx++) {
        for (int ny=-mMaxN; ny<=mMaxN; ny++) {
            for (int nz=-mMaxN; nz<=mMaxN; nz++) {
                if (
                     // not yet filled in
                    (mEk[nx][ny][nz] == dcomp(0, 0)) &&
                    // not too high
                    (kk(nx,ny,nz)*mLSS < mMaxKLSS)
                ) {
                    // If the ek is allready set, it means it is set
                    // earlier in the switch statement below,
                    // otherwise it is a random Gaussian.
                    sp = dcomp(
                        mRandom.ugaussian(),
                        mRandom.ugaussian()
                    );
                    sp *= CIRC * sqrt(mVol);

                    mEk[nx][ny][nz] = sp;

                    switch (mTopoType) {
                        case EUCLIDIAN:
                          /* ^*     ^      
                             e(k) = e(-k)  */
                            mEk[+nx][+ny][+nz] = sp;
                            mEk[-nx][-ny][-nz] = conj(sp);
                            if ((nx == 0) && (ny == 0) && (nz == 0)) {
                                // Special case for the 0 point.
                                // Doesn't matter though,
                                // since the power will be 0..
                                mEk[-nx][-ny][-nz] = dcomp(sp.real() * sqrt(2.0));
                            }
                            break;
                        case TORUS:
                            /* same as EUCLIDIAN, just less k's allowed */
                            mEk[+nx][+ny][+nz] = sp;
                            mEk[-nx][-ny][-nz] = conj(sp);
                            if ((nx == 0) && (ny == 0) && (nz == 0)) {
                                mEk[-nx][-ny][-nz] = dcomp(sp.real() * sqrt(2.0));
                            }
                            break;
                        case HALFTURN:
                            /* these look very different from the
                             * EUCLIDIAN case */
                            mEk[+nx][+ny][+nz] = sp;
                            mEk[+nx][+ny][-nz] = pow(-1., nz) * conj(sp);
                            if ( (nz==0) ) {
                                // Special 0 case
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.0) * sp.real(), 0);
                            }
                            break;
                        case QUARTERTURN:
                            mEk[+nx][+ny][+nz] = sp;
                            mEk[+nx][+ny][-nz] = pow(-1., nz) * conj(sp);
                            if (nz == 0) {
                                // special 0 case
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.0) * sp.real(), 0);
                            }
                            break;
                        case THIRDTURN:
                            if (ny > nx) {
                                mEk[+nx][   +ny][+nz] = sp;
                                mEk[+ny][+ny-nx][-nz] = exp(dcomp(0, -2. * CIRC / 3. * nz)) * conj(sp);
                            } else if (nx >= ny) {
                                mEk[+nx   ][+ny][+nz] = sp;
                                mEk[+nx-ny][+nx][-nz] = exp(dcomp(0, -CIRC / 3. * nz)) * conj(sp);
                            }
                            break;
                        case SIXTHTURN:
                            // shouldn't this be more like the third turn space ?
                            mEk[+nx][+ny][+nz] = sp;
                            mEk[+nx][+ny][-nz] = pow(-1., nz) * conj(sp);
                            if (nz == 0) {
                                // special 0 case    
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.0) * sp.real(), 0);
                            }
                            break;
                        case HANTZSCHEWENDT:
                            mEk[+nx][+ny][+nz] = sp;
                            if ((nx > 0) && (ny > 0)) {
                                mEk[+nx][+ny][-nz] = pow(-1., nx-nz) * conj(sp);
                                if ((nz == 0) && (nx % 2 == 0))
                                    mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                                if ((nz == 0) && (nx % 2 == 1))
                                    mEk[+nx][+ny][+nz] = dcomp(0, sqrt(2.) * sp.imag());
                            }
                            if ((ny > 0) && (nz > 0)) {
                                if ((nx == 0) && (ny % 2 == 0))
                                    mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                                if ((nx == 0) && (ny % 2 == 1))
                                    mEk[+nx][+ny][+nz] = dcomp(0, sqrt(2.) * sp.imag());
                            }
                            if ((nx > 0) && (nz > 0)) {
                                if ((ny == 0) && (nz % 2 == 0))
                                    mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                                if ((ny == 0) && (nz % 2 == 1))
                                    mEk[+nx][+ny][+nz] = dcomp(0, sqrt(2.) * sp.imag());
                            }
                            if ((nx == 0) && (ny == 0) && (nz > 0))
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                            if ((nx == 0) && (ny > 0) && (nz == 0))
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                            if ((nx > 0) && (ny == 0) && (nz == 0))
                                mEk[+nx][+ny][+nz] = dcomp(sqrt(2.) * sp.real(), 0);
                          break;
                        default:
                          break;
                    } // switch mTopoType
                } // if not filled in
            } // for z
            if (mVerbose > 5)
                ekcounter.update();
        } // for y
    } // for x
    ekcounter.quit();
    return;
}

void   Cmb::buildalm()
{
    /* The ek are converted to alm, which are the coefficients
       for the spherical harmonics.
     
            circ³ ⎲ ┌────┐      ⎲  [Γ]s ^
      a   = ───── ⎳ │P(k)  G(k) ⎳ ξ     e
       lm     V   k ⎷ Φ     l   s  klm   k   */

    mAlm.clear();
    Counter almcounter( "Building alm: ", xiicount(), 11);

    dcomp aXi;
    double kkk;
    double chi;
    double * bes;
    bes = new double[mMaxL+1];

    dcomp aPlus = dcomp(0, 0);
    dcomp aPowerK = dcomp(0, 0);
    dcomp aBessel = dcomp(0, 0);
    dcomp aEk = dcomp(0, 0);

    if (mVerbose > 5) {
        almcounter.printini();
    }
    
    // loop over all nx,ny,nz
    for (int nx=-mMaxN; nx<=mMaxN; nx++) {
        for (int ny=-mMaxN; ny<=mMaxN; ny++) {
            for (int nz=-mMaxN; nz<=mMaxN; nz++) {

                // Determining the absolute value of the k vector
                kkk = kk(nx, ny, nz) ;

                // Power of that wavenumber
                aPowerK = sqrt(powerk(kkk));

                // Radial component of the CMB coordinates
                chi=kkk * mLSS;

                // Calculating the bessel functions
                gsl_sf_bessel_jl_array(mMaxL, chi, bes);

                // loop over all l, m
                for (int l=0; l<=mMaxL; l++){
                    aBessel = bes[l];

                    for (int m=-l; m<=l; m++){

                        aPlus = dcomp(1, 0);
                        aPlus *= aPowerK;
                        aPlus *= aBessel;

                        aXi  = xii(nx, ny, nz, l, m);
                        aPlus *= aXi;

                        aEk = mEk[nx][ny][nz];
                        aPlus *= aEk;
                        
                        mAlm[l][m] += aPlus;

                    } // m
                } // l

                if ((mVerbose > 5) && ( xiitest(nx,ny,nz) ) ) {
                    almcounter.update();
                }

            } // nz
        } // ny
    } // nx
    
    almcounter.quit();
    
    delete[] bes;
    
    // Normalize
    for (int l=0; l<=mMaxL; l++){
        for (int m=-l; m<=l; m++){
            mAlm[l][m] *= pow(CIRC, 3) / mVol;
        }
    }    
    
    return;
}

void   Cmb::buildcmb()
{
    // filling the cmb

    // clearing it
    for (Cmbdata::iterator cmbEl = mCmbdata.begin();
         cmbEl != mCmbdata.end();
         ++cmbEl)
    {
        cmbEl->temperature = 0;
    }

    Counter cmbcounter("Building CMB: ", (mMaxL+1) * (mMaxL+1), 3);
    if (mVerbose > 5)
        cmbcounter.printini();

    // looping over all l
    for (int l=0; l<=mMaxL; l++)
    {
        // looping over all m
        for (int m=-l; m<=l; m++)
        {
            for (Cmbdata::iterator cmbEl = mCmbdata.begin();
                 cmbEl != mCmbdata.end();
                 ++cmbEl)
            {
                 cmbEl->temperature += mAlm[l][m] * spham(l,m,cmbEl->coordinate);
            }
            if (mVerbose > 5)
                cmbcounter.update();
        }
    }
    cmbcounter.quit();

    return;
}

Alm & Cmb::alm()
{
    return mAlm;
}

Random & Cmb::random()
{
    return mRandom;
}

void   Cmb::showparams()
{
    // Prints the parameters of the universe
    cout << "Universe parameters:" << endl;
    cout << " Topology:  ";
    switch (mTopoType) {
        case EUCLIDIAN:   cout << "Euclidian";   break;
        case TORUS:       cout << "Torus";       break;
        case HALFTURN:    cout << "Halfturn";    break;
        case QUARTERTURN: cout << "Quarterturn"; break;
        case THIRDTURN:   cout << "Thirdturn";   break;
        case SIXTHTURN:   cout << "Sixthturn";   break;
        case HANTZSCHEWENDT: cout << "Hantzsche-Wendt"; break;
        default:          cout << "unknown";     break;
    }
    cout << endl;
    for (int i=0; i<3; ++i){
        cout << " FD parameter "<<i<<": "<<mL[i]<<endl;    
    }
    cout <<     " FD volume     : " << mVol << endl;
    cout <<     " LSS distance  : " << mLSS << endl;
    cout << endl;
    cout <<     " max L         : " << mMaxL << endl;
    if(mMaxN == 0) calculateMaxN();
    cout <<     " max N         : " << mMaxN << endl;
    cout <<     " cmbres        : " << mCmbdata.resolution() << endl;
    cout <<     " max K*LSS     : " << mMaxKLSS << endl;
    cout <<     " uberL         : " << mUberL << endl;
    return;
}

Cmbdata & Cmb::cmbdata(){
    return mCmbdata;
}
