#include "Random_i.h"

Random::Random()
  :
    mInitialized(0),
    mRandomSeed(-42)
{
    mRan = gsl_rng_alloc(gsl_rng_default);
}

Random::~Random()
{
    gsl_rng_free(mRan);
}

long int Random::seed(long int pSeed)
{
    mRandomSeed = pSeed;
    const gsl_rng_type * rant;
    gsl_rng_env_setup();
    rant=gsl_rng_default;
    mRan = gsl_rng_alloc(rant);
    gsl_rng_set(mRan,pSeed); // seed should be entered..
    mInitialized = 1;
    return mRandomSeed;
}

long int Random::seed()
{
    return mRandomSeed;
}

double Random::ugaussian()
{
    if (mInitialized)
        return gsl_ran_ugaussian(mRan);
    else
        return -1;
}

double Random::uniform()
{
    if (mInitialized)
        return gsl_rng_uniform(mRan);
    else
        return -1;
}

