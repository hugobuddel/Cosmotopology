#ifndef RANDOM_H_
#define RANDOM_H_

#include <iostream>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

class Random
{
/**
 * @class Random
 * @brief Create random numbers for initial conditions of the Universe
 */

public:
    Random();
    virtual ~Random();
    
    // sets the seed and initializes
    long int seed(long int pSeed);
    
    // gets the seed
    long int seed();
    
    // returns a number from a uniform distribution between 0 and 1
    double uniform();
    
    // returns a number from a normal distribution with sigma=1
    double ugaussian();
    
private:
    long int mRandomSeed; // random seed
    gsl_rng * mRan;       // random number generator
    bool mInitialized;    // 0 is uninitialized, 1 is initialized
    
};

#endif /*RANDOM_H_*/
