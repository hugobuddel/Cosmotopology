#include "Coordinate.h"

Coordinate::Coordinate()
  :
    circ(6.283185307179586476925286766559),
    mCartesianX(NAN),
    mCartesianY(NAN),
    mCartesianZ(NAN),
    mSphericalRadius(NAN),
    mSphericalZenith(NAN),
    mSphericalAzimuth(NAN)
    //mSpherLatitude(NAN),
{
    // initialize everything at NaN
}

Coordinate::~Coordinate()
{
}

double Coordinate::cartesianX()
{
    if (!isnan(mCartesianX))
    {
        return mCartesianX;
    }
    
    if (!isnan(mSphericalRadius))
    {
        convertSphericalToCartesian();
        return mCartesianX;
    }

    return NAN;    
}

double Coordinate::cartesianY()
{
    if (!isnan(mCartesianY))
    {
        return mCartesianY;
    }
    
    if (!isnan(mSphericalRadius))
    {
        convertSphericalToCartesian();
        return mCartesianY;
    }
    
    return NAN;    
}

double Coordinate::cartesianZ()
{
    if (!isnan(mCartesianZ))
    {
        return mCartesianZ;
    }
    
    if (!isnan(mSphericalRadius))
    {
        convertSphericalToCartesian();
        return mCartesianZ;
    }

    return NAN;    
}

double Coordinate::sphericalRadius()
{
    if (!isnan(mSphericalRadius))
    {
        return mSphericalRadius;
    }
    
    if (!isnan(mCartesianX))
    {
        convertCartesianToSpherical();
        return mSphericalRadius;
    }

    return NAN;    
}

double Coordinate::sphericalAzimuth()
{
    if (!isnan(mSphericalAzimuth))
    {
        return mSphericalAzimuth;
    }
    
    if (!isnan(mCartesianX))
    {
        convertCartesianToSpherical();
        return mSphericalAzimuth;
    }

    return NAN;    
}

double Coordinate::sphericalZenith()
{
    if (!isnan(mSphericalZenith))
    {
        return mSphericalZenith;
    }
    
    if (!isnan(mCartesianX))
    {
        convertCartesianToSpherical();
        return mSphericalZenith;
    }

    return NAN;    
}

double Coordinate::sphericalLatitude()
{
    return circ/4-sphericalZenith();
}

double Coordinate::sphericalAitoffX() {
    double longi = this->sphericalAzimuth();
    //if(2*longi > circ) longi-=circ;
    longi-=circ/2;
    double lati  = this->sphericalLatitude();
    double z = sqrt(1 + cos(lati) * cos(longi/2));
    double x = cos(lati) * sin(longi/2) / z;
    //double y = sin(lati) / z;
    return 2*x;
}

double Coordinate::sphericalAitoffY() {
    double longi = this->sphericalAzimuth();
    //if(2*longi > circ) longi-=circ;
    longi-=circ/2;
    double lati  = this->sphericalLatitude();
    double z = sqrt(1 + cos(lati) * cos(longi/2));
    //double x = cos(lati) * sin(longi/2) / z;
    double y = sin(lati) / z;
    return y;
}

double Coordinate::sphericalAitoffX2() {
    double longi = this->sphericalAzimuth();
    longi+=circ/2;
    if(2*longi > circ) longi-=circ;
    if(2*longi > circ) longi-=circ;
    double lati  = this->sphericalLatitude();
    double z = sqrt(1 + cos(lati) * cos(longi/2));
    double x = cos(lati) * sin(longi/2) / z;
    //double y = sin(lati) / z;
    return 2*x;
}

double Coordinate::sphericalAitoffY2() {
    double longi = this->sphericalAzimuth();
    longi+=circ/2;
    if(2*longi > circ) longi-=circ;
    if(2*longi > circ) longi-=circ;
    double lati  = this->sphericalLatitude();
    double z = sqrt(1 + cos(lati) * cos(longi/2));
    //double x = cos(lati) * sin(longi/2) / z;
    double y = sin(lati) / z;
    return y;
}

inline void Coordinate::clearCartesian()
{
    mCartesianX = NAN;
    mCartesianY = NAN;
    mCartesianZ = NAN;
}

inline void Coordinate::clearSpherical()
{
    mSphericalRadius  = NAN;
    mSphericalAzimuth = NAN;
    mSphericalZenith  = NAN;
}

Coordinate & Coordinate::cartesian(double pX, double pY, double pZ)
{
    mCartesianX = pX;
    mCartesianY = pY;
    mCartesianZ = pZ;
    clearSpherical();
    return *this;
}

double Coordinate::cartesianX(double p)
{
    mCartesianX = p;
    clearSpherical();
    return p;    
}

double Coordinate::cartesianY(double p)
{
    mCartesianY = p;
    clearSpherical();    
    return p;    
}

double Coordinate::cartesianZ(double p)
{
    mCartesianZ = p;
    clearSpherical();    
    return p;    
}

Coordinate & Coordinate::spherical(double pRadius, double pZenith, double pAzimuth)
{
    mSphericalRadius  = pRadius;
    mSphericalZenith  = pZenith;
    mSphericalAzimuth = pAzimuth;
    clearCartesian();
    return *this;
}

double Coordinate::sphericalRadius(double p)
{
    mSphericalRadius = p;
    clearCartesian();
    return p;    
}
double Coordinate::sphericalAzimuth(double p)
{
    mSphericalAzimuth = p;
    // in case the coordinate is only used to denote a direction
    if (mSphericalRadius == NAN) mSphericalRadius = 1;
    clearCartesian();
    return p;    
}
double Coordinate::sphericalZenith(double p)
{
    mSphericalZenith = p;
    if (mSphericalRadius == NAN) mSphericalRadius = 1;
    clearCartesian();
    return p;    
}

void Coordinate::convertCartesianToSpherical()
{
    double h;
    mSphericalRadius = sqrt(mCartesianX*mCartesianX + 
                            mCartesianY*mCartesianY +
                            mCartesianZ*mCartesianZ);
    (mSphericalRadius==0) ? h=0 : h=mCartesianZ/mSphericalRadius;
    mSphericalZenith  = acos( h );
    mSphericalAzimuth = atan2(mCartesianY, mCartesianX);
}

void Coordinate::convertSphericalToCartesian()
{
    mCartesianX = mSphericalRadius * 
                  sin( mSphericalZenith ) *
                  cos( mSphericalAzimuth );
    mCartesianY = mSphericalRadius * 
                  sin( mSphericalZenith ) *
                  sin( mSphericalAzimuth );
    mCartesianZ = mSphericalRadius *
                  cos( mSphericalZenith );
}
