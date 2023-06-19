#ifndef EK_H_
#define EK_H_

#include <complex>
#include "NegVector.h"

#define dcomp std::complex <double>
#define dcomp3D NegVector< NegVector< NegVector<dcomp> > >

class Ek : public dcomp3D
{

public:
    Ek();
    virtual ~Ek();

    // returns size
    int size();
    // sets size
    int size(int n);
    // alias
    int resize(int n){return size(n);};

    // clear ek
    void clear();

    // print, save and load functions
    void print(std::ostream *ost = &std::cout);    

};

#endif /*EK_H_*/
