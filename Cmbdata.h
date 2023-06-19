#ifndef CMBDATA_H_
#define CMBDATA_H_

#include <vector>
#include <string>
#include <complex>
#define dcomp std::complex <double>
#include "Coordinate.h"

struct CmbDataElement
{
    Coordinate coordinate;  // the coordinate of the point
    dcomp      temperature; // the temperature (should be real)
    double     weight;      // the surfaceweight (for not equal area tilings of the sphere)
    CmbDataElement * triangle[3]; // three other elements to create triangle with
};

#define CmbDataVector std::vector<CmbDataElement>

class Cmbdata : public CmbDataVector
{
/**
 * @class CmbData
 * @brief A container for the Cosmic Microwave Background data
 */

public:
    Cmbdata();
    virtual ~Cmbdata();

    // get/set resolution
    int resolution(); // gets resolution
    int resolution(int pResolution); // sets resolution
    
    // get the maximum/minimum cmb value
    double maximum();
    double minimum();
    
    // checks whether the CMB is real-valued
    double reality();

    void print(std::ostream *ost = &std::cout);

private:

    int       mResolution;  // the resolution parameter
    double    mMinimum;     // minimum value of the cmb (abs)
    double    mMaximum;     // maximum value of the cmb (abs)
    double    mAverageTemp; // average temp
    double    mStandardDevTemp; // standard deviation
    
    void      clear();
    void      fillCoords(); // fills the coordinates
    void      fillMinMax(); // fills the minimum and maximum values

    // implementation specific variables
    int       mResAzimuth;  // azimuthal resolution
    int       mResZenith;   // zenithal resolution

};

#endif /*CMBDATA_H_*/
