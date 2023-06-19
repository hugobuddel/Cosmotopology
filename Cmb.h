#ifndef CMB_H_
#define CMB_H_

#define CIRC 6.2831853071795862

#include <complex>
#define dcomp std::complex <double>

#include <vector>
#include <string>

#include "Alm.h"
#include "Counter.h"
#include "Coordinate.h"
#include "Cmbdata.h"
#include "Ek.h"
#include "Random.h"

class Cmb
{
public:
    enum topologies {
        EUCLIDIAN,     // is 0?
        TORUS,         // 1
        HALFTURN,      // 2
        QUARTERTURN,   // 3
        THIRDTURN,     // 4
        SIXTHTURN,     // 5
        HANTZSCHEWENDT // 6
    };

    /* although public, these variables should usually not be set by hand!
     * use the appropriate functions to set them. */

    /* Properties of the Universe */
    
    // the topology
    topologies  mTopoType;
    
    /* Properties of the Fundamental Domain (FD) */
    
    // parameters of the FD (mostly lengths)
    double      mL[3];
    
    // distance to Last Scattering Surface
    // (in the same units as the length parameters in mL)
    double      mLSS;
    
    // volume of the FD
    double      mVol;
    
    // maximum value of kkk*LSS that should be reached, about 20
    // (should be less then max of bessel), so kmax=mMaxKLSS/LSS and
    // since kmax=nMax/L we get nMax=kmax*L = mMaxKLSS*L/LSS
    double      mMaxKLSS;
    
    // the L for the formula above (of mMaxKLSS), about the max of mL[]
    double      mUberL;

    // maximum value of l for the spherical eigenvalues (Alm)
    int         mMaxL;
       
    // maximum value of n_x,n_y,n_z for the Cartesian
    // eigenvalues (Ek, ξ)
    int         mMaxN;
    
    // verbosity level
    int         mVerbose;
    
    // name of this 'universe'
    std::string mName;

    // random number generator
    Random      mRandom;
    
    // the a_lm values (spherical harmonics coefficients)
    Alm         mAlm;
    
    // ek stores the ξ values, the gaussian vectors that represent
    // the (random) coefficients for the linear eigenmodes.
    Ek          mEk;         
    
    // the real-valued data of the cmb
    Cmbdata     mCmbdata;

    /* Functions: */

    // Constructor and Destructor
    Cmb();
    virtual ~Cmb();


    /* Data members accesors */
    
    // get/set the maximum value of l
    int          maxl();          
    int          maxl(int maxl);
    
    // get/set the maximum value of n (should be ~maximum l value)
    int          maxn();
    int          maxn(int maxn);

    // get/set the maximum value of k*LSS
    double       maxKLSS();
    double       maxKLSS(double max);
    
    // get/set the topology
    topologies   topology();
    topologies   topology(topologies pTopoType);
    
    // returns a specific L value
    double       L(int ll);
    
    // sets one specific L value and return the volume of the FD
    double       L(int ll, double l);
    
    // sets all L values and return the volume of the FD
    double       L(double l0, double l1, double l2);
    
    // get/set the value of the uberL to calculate nMax
    double       uberL(double uL);
    double       uberL();
    
    // get/set the distance to the Surface of Last Scattering
    double       LSS();
    double       LSS(double pLSS);
    
    // calculate the maximum of n from uberL and maxKLSS and LSS
    int          calculateMaxN();
    
    // get/set the name of this CMB
    std::string  name();
    std::string  name(std::string pName);
    
    // calculates the volume of the Fundametal Domain (FD)
    double       volume();
    
    // set/get verbosity level
    int          verbose();
    int          verbose(int pVerbose);
    
    // get Cmbdata, Alm and RNG
    Cmbdata &    cmbdata();
    Alm &        alm();
    Random &     random();


    // build functions
    void      build();      // builds ek, alm and cmb
    void      buildek();    // builds ek
    void      buildalm();   // builds alm
    void      buildcmb();   // builds cmb


    // I/O functions
    void      showparams(); // prints out parameters of the CMB

private:
    // spherical harmonic
    dcomp  spham(int l, int m, Coordinate coor);

    // get the power of k value
    double powerk(double k);

    // test whether this ξ should be calculated
    int    xiitest(int nx, int ny, int nz);
    
    // count how many ξ's we will calculate
    int    xiicount();

    /*             [Γ]s 
      calculates  ξklm  */
    dcomp  xii(int nx, int ny, int nz, int l, int m);

    // calculate absolute value of k
    double kk(int nx, int ny, int nz);
};

#endif /*CMB_H_*/
