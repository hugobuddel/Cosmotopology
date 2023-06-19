#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <cmath>
#include <iostream>

/*
 * "Coordinate" - Hugo Buddelmeijer 20051001
 * This class contains a 3Dimensional coordinate, it can be
 * set and read in Cartesian or Spherical coordinates.
 * If it is set in one coordinate system, the
 * other coordinates are set to NaN. When a coordinate is
 * read from another system, the coordinates are converted.
 * This way coordinate transformations are only performed
 * when nescecary.
 */
class Coordinate
{
public:
    Coordinate();
    virtual ~Coordinate();

    // Cartesian
    Coordinate & cartesian(double pX, double pY, double pZ);
    double cartesianX();          // returns X
    double cartesianX(double pX); // sets X
    double cartesianY();
    double cartesianY(double pY);
    double cartesianZ();
    double cartesianZ(double pZ);
    
    // Spherical
    Coordinate & spherical(double pRadius, double pZenith, double pAzimuth);
    double sphericalRadius();           // returns radius
    double sphericalRadius(double pSr); // sets radius
    double sphericalZenith();           // returns zenith
    double sphericalZenith(double pSze);
    double sphericalAzimuth();          // returns azimuth
    double sphericalAzimuth(double pSaz);
    double sphericalLatitude();
    double sphericalLatitude(double pSla);
    double sphericalAitoffX();
    double sphericalAitoffY();
    double sphericalAitoffX2();
    double sphericalAitoffY2();
    
    void convertCartesianToSpherical();
    void convertSphericalToCartesian();
    
    // shorthands inline
    // carthesian
    double x(){return cartesianX();}
    double x(double px){return cartesianX(px);}
    double y(){return cartesianY();}
    double y(double py){return cartesianY(py);}
    double z(){return cartesianZ();}
    double z(double pz){return cartesianZ(pz);}
    // spherical
    double ra(){return sphericalRadius();}
    double ra(double pr){return sphericalRadius(pr);}
    double ze(){return sphericalZenith();}
    double ze(double pr){return sphericalZenith(pr);}
    double az(){return sphericalAzimuth();}
    double az(double pr){return sphericalAzimuth(pr);}

    void clearCartesian();
    void clearSpherical();
    
private:
    double circ; // two*pi

    // Cartesian
    double mCartesianX;
    double mCartesianY;
    double mCartesianZ;
    
    // Spherical
    double mSphericalRadius;
    double mSphericalZenith; //or colatitude or polar_angle
    double mSphericalAzimuth; // or longitude
    //double mSpherLatitude; // = circ/4 - zenith

};

#endif /*COORDINATE_H_*/
