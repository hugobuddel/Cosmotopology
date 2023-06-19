#ifndef ALM_H_
#define ALM_H_

#include <iostream>
#include <fstream>

#include <complex>
#define dcomp std::complex <double>
#include <string>

#define MAXL 10

/* Class containing the alm values for a certain l 
 * can be accesed with negative values for m.
 * Example:
 *   Al al;
 *   al.setl(1);
 *   al[-1] = dcomp( 0.1, 0.2);
 *   al[ 0] = dcomp( 0.3, 0.0);
 *   al[ 1] = dcomp(-0.1, 0.2);
 * */
 
class Al
{
public:
    Al();              // constructor, not usefull before calling function below
    Al(int pMaxl);
    virtual ~Al();
    void setl(int l);  // setting l, initializing aal array
    void init();

    dcomp &operator[](int m);       // [] overloaders, so negative numbers are allowed;
    dcomp  operator[](int m) const;

    int    getl();     // returns the value of l
    void   printall(); // prints all l,m,a

private:
    int    l;          // l>=0
    dcomp *aal;        // size 2*l+1: -l, -l+1, .., -1, 0, 1, .., l
    int   status;    // 0:uninitialized, 1:initialised, >1:error
};



/* Class containing alm values
 * can be accesed with negative values for m.
 * Example:
 *   Alm alm(5);
 *   // m   l           a
 *   al[1][-1] = dcomp( 0.1, 0.2);
 *   al[1][ 0] = dcomp( 0.3, 0.0);
 *   al[1][ 1] = dcomp(-0.1, 0.2);
 * */

class Alm
{
public:
    Alm();          
    Alm(int pMaxl);
    virtual ~Alm();

    int  getl();
    void setl(int pMaxl);// sets pmaxl as mMaxl
    void init(); // initializes and clears the alm array aalm
    void clear(); // clears the alm

    Al& operator[](int l);       // should only be used with the [] operator..
    Al& operator[](int l) const; // ..from Al

    double getPower(int l);
    void printPowers();
    
    double realcheck(); // returns the deviation from a real (as in noncomplex) cmb, should be near system precission for doubles or lower.
    void printalm(); // prints all the coefficients

private:
    int   mMaxl;     // highest l
    Al   *aalm;      // array of objects containing the alm values for each l
    int   status;    // 0:uninitialized, 1:initialised, >1:error
};

#endif /*ALM_H_*/
