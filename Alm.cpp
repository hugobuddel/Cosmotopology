#include "Alm_i.h"

Al::Al()
  :
    l(-1),
    status(0),
    aal(0)
{
    /* constructor, do nothing
     * not usable without calling setl() first.
     */
    l   = -1; // impossible l
}

Al::Al(int pl)
  :
    status(0)
{
    setl(pl);
}

void Al::setl(int pl)
{
    /* setting l and creating array with coefficients
     */
    l   = pl;
    init();
}

void Al::init()
{
    if (l==-1) l=MAXL;
    if (status==1) return;
    aal = new dcomp[2*l+1];
    for (int m=-l; m<=+l; ++m){
        aal[l+m]=dcomp(0,0);
    }
        
}

void Al::printall(){
    for (int m=-l; m<=+l; ++m){
        int s=l+m;
        cout << " l:"<<l<<" m:"<<m<<" s:"<<s<<" a:"<<aal[s]<<endl;
    }
}

Al::~Al()
{
    delete[] aal;
}

dcomp  Al::operator[](int m) const
{
    return aal[l+m];    
}

dcomp& Al::operator[](int m)
{
    return aal[l+m];
}


int Al::getl(){
    return l;
}





Alm::Alm()
  :
    mMaxl(-1),
    aalm(0),
    status(0)
{
}

int Alm::getl()
{
    return mMaxl;
}

Alm::Alm(int pMaxl)
  :
     status(0)
{
    setl(pMaxl);
}

void Alm::setl(int pMaxl)
{
    mMaxl = pMaxl;
    init();
}

Alm::~Alm()
{
    /* The Al's should be deleted automatically... */
}

Al &Alm::operator[](int l)
{
    return aalm[l];    
}

Al& Alm::operator[](int l) const
{
    return aalm[l];    
}

void Alm::init()
{
    if (mMaxl==-1) mMaxl=MAXL;
    // if (status==1) return;
    aalm = new Al[mMaxl+1];
    for (int l=0; l<=mMaxl; ++l)
    {
        aalm[l].setl(l);
    }
    
}

void Alm::clear()
{
    for (int l=0; l<=mMaxl; ++l)
        for (int m=-l; m<=l; ++m)
            (*this)[l][m] = dcomp(0,0);
}

double Alm::getPower(int l)
{
/*         1
 * C_l = ---- Sum (a_lm a_lm*)
 *       2l+1  m
 */
     double Cl = 0;
    for(int m=-l; m<=l; ++m)
    {
        Cl += real(aalm[l][m] * conj(aalm[l][m]));
    }
    return Cl;
}

void Alm::printPowers()
{
    cout << "#l,Cl"<<endl;
    for (int l=0; l<=mMaxl; ++l)
    {
        cout << l << "," << getPower(l) << endl;
    }
    return;
}

double Alm::realcheck()
{
    /* Is this alm real? */
    dcomp sp,spc,a1,a2;
    for (int l=0; l<=mMaxl; ++l)
    {
        for (int m=-l; m<=l; ++m)
        {
            a1=aalm[l][m];
            a2=aalm[l][-m];
            sp=a1-pow(-1.0,m)*conj(a2);
            spc+=sp;
        }
    }
    return abs(spc);
}

void Alm::printalm()
{
    cout.precision(6);
    for(int l=0; l<=mMaxl; ++l)
    {
        aalm[l].printall();
    }
}
