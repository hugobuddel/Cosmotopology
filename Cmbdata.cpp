#include "Cmbdata_i.h"

// Constructor
Cmbdata::Cmbdata()
  :
    mResolution(0),
    mMinimum(NAN),
    mMaximum(NAN),
    mAverageTemp(NAN),
    mStandardDevTemp(NAN),
    mResAzimuth(0),
    mResZenith(0)
{
}

// Destructor
Cmbdata::~Cmbdata()
{
}

// resets the data
void Cmbdata::clear()
{
    resolution(0);
    mMinimum = NAN;
    mMaximum = NAN;
}

/* Getting and setting parameters */

// Get the current resolution
int Cmbdata::resolution()
{
    return mResolution;
}

// Set a new resolution
int Cmbdata::resolution(int pResolution)
{
    mResAzimuth = int(sqrt(pResolution*1.0));
    mResZenith  = pResolution/mResAzimuth;
    mResolution = mResAzimuth * mResZenith;

    CmbDataVector::resize(mResolution);

    fillCoords();

    return mResolution;    
}

// Get the maximum value of the (real part of the) cmb
double Cmbdata::maximum()
{
    if( isnan(mMaximum) )
    {
        fillMinMax();
    }
    return mMaximum;
}

// Get the minimum value of the (real part of the) cmb
double Cmbdata::minimum()
{
    if( isnan(mMaximum) )
    {
        fillMinMax();
    }
    return mMinimum;
}

// Do a reality check (i.e. sum the non real parts)
double Cmbdata::reality()
{
    double reality=0;
    for (Cmbdata::iterator cmbEl = this->begin();
         cmbEl != this->end();
         ++cmbEl)
    {
        reality+=imag(cmbEl->temperature)*cmbEl->weight;
    }    
    return reality;
}

/* Calculating Values */

// Calculate the coordinates corresponding to the dataset
void Cmbdata::fillCoords()
{
    // filling the coordinates..

    int thei;
    int iAzi, nAzi0, nAzi1, nAzi2;
    int iZen, nZen0, nZen1, nZen2;
    double zenith;
    double azimuth;
    for(iAzi = 0; iAzi < mResAzimuth; iAzi++)
    {
        for(iZen=0; iZen<mResZenith; iZen++)
        {
            double circ = 6.2831853071795862;
            thei = mResZenith * iAzi + iZen;
            // -1 because we want inclusive!
            // even though all these points are the same)
            zenith  = ((circ/2)*iZen) / (mResZenith-1);
            // exclusive!
            azimuth = ( circ   *iAzi) / (mResAzimuth);

            /* Triangles to be drawn like this:
              
                \    |     \    |
                -( iA, iZ)--(nA0,nZ0)-
                     |    \     |    \
                \    |     \    |
                -(nA2,nZ2)--(nA1,nZ1)-
                     |    \     |    \
            */


            nAzi0 = iAzi+1;
            if (nAzi0 >= mResAzimuth) nAzi0 = 0;
            nAzi1 = nAzi0;
            nAzi2 = iAzi;
            nZen0 = iZen;
            nZen1 = iZen+1;
            if (nZen1 >= mResZenith) nZen1 = mResZenith-1;
            nZen2 = nZen1;
            
            (*this)[thei].coordinate.sphericalRadius(1.0);
            (*this)[thei].coordinate.sphericalAzimuth(azimuth);
            (*this)[thei].coordinate.sphericalZenith(zenith);
            (*this)[thei].weight=sin((*this)[thei].coordinate.ze());
            (*this)[thei].temperature=dcomp(0,0);
            (*this)[thei].triangle[0] = &(*this)[mResZenith * nAzi0 + nZen0];
            (*this)[thei].triangle[1] = &(*this)[mResZenith * nAzi1 + nZen1];
            (*this)[thei].triangle[2] = &(*this)[mResZenith * nAzi2 + nZen2];
        } // iZen
    } // iAzi
}

// Calculate the minimum and maximum
void Cmbdata::fillMinMax()
{
    mMaximum=-100000;
    mMinimum= 100000;
    double temperature;
    double totalTemp = 0;
    double totalStdDev = 0;

    for (Cmbdata::iterator cmbEl = this->begin();
         cmbEl != this->end();
         ++cmbEl)
    {
        temperature = real(cmbEl->temperature);
        totalTemp += temperature;
        if (temperature > mMaximum) mMaximum = temperature;
        if (temperature < mMinimum) mMinimum = temperature;
    }
    mAverageTemp = totalTemp / (this->end() - this->begin());
    
    for (Cmbdata::iterator cmbEl = this->begin();
         cmbEl != this->end();
         ++cmbEl)
    {
        temperature =real(cmbEl->temperature);
        totalStdDev += (temperature - mAverageTemp) * (temperature - mAverageTemp);
    }
    mStandardDevTemp = sqrt(totalStdDev / (this->end() - this->begin()));
    
}

/* Input and Output functions */

// print out the cmb to cout
void Cmbdata::print(std::ostream *ost)
{
    double circ = 6.2831853071795862;
    cout.precision(6);
    *ost << "Printing cmbdata"<<endl;
    *ost << " cmbdatasize: " << size() << endl;
    *ost << " minimum:" << minimum() << " maximum:"<< maximum() << endl;
    for (Cmbdata::iterator cmbEl = this->begin();
         cmbEl != this->end();
         ++cmbEl)
    {
        *ost << "Point ze:" << cmbEl->coordinate.sphericalZenith()/circ
             << " az:" << cmbEl->coordinate.sphericalAzimuth()/circ
             << " dT:" << cmbEl->temperature
             << endl;
        *ost << "  t0: ze:" << cmbEl->triangle[0]->coordinate.sphericalZenith()/circ
             << " az:" << cmbEl->triangle[0]->coordinate.sphericalAzimuth()/circ
             << endl;
        *ost << "  t1: ze:" << cmbEl->triangle[1]->coordinate.sphericalZenith()/circ
             << " az:" << cmbEl->triangle[1]->coordinate.sphericalAzimuth()/circ
             << endl;
        *ost << "  t2: ze:" << cmbEl->triangle[2]->coordinate.sphericalZenith()/circ
             << " az:" << cmbEl->triangle[2]->coordinate.sphericalAzimuth()/circ
             << endl;
    }
    
    return;
}

