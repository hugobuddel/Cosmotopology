#ifndef NEGVECTOR_H_
#define NEGVECTOR_H_

#include <vector>
#include <iostream>

template <typename T>
class NegVector : public std::vector<T>
{
/**
 * @class NegVector
 * @brief A Vector class that allows negative indices for Alm values.
 */

private:
    int mBegin;  // Largest value that can be indexed.
    
public:
    NegVector();
    virtual ~NegVector();
    T &operator[](int index);
    T &operator[](int index) const;
    void resize(int n);
    int size();
    int size(int n) {
        resize(n);
        return size();
    }
    void clear();
};

// Constructor, set empty vector.
template <typename T>
NegVector<T>::NegVector()
  :
    mBegin(-1)
{
}

// Destructor
template <typename T>
NegVector<T>::~NegVector()
{
}

// Resize so vector goes from -n,..,0,..,n
template <typename T>
void NegVector<T>::resize(int n)
{
    mBegin = n;
    std::vector<T>::resize(2*mBegin+1);
}

// Return the size (not the real one though)
// returns -1 if vector is not initialised
template <typename T>
int NegVector<T>::size()
{
    return mBegin;
}

// clear everything
template <typename T>
void NegVector<T>::clear()
{
    mBegin = -1;
    std::vector<T>::clear();    
}

// Return the element
template <typename T>
T &NegVector<T>::operator [](int index)
{
    return std::vector<T>::operator[](index+mBegin);
}

// Return the element
template <typename T>
T &NegVector<T>::operator [](int index) const
{
    return std::vector<T>::operator[](index+mBegin);
}

#endif /*NEGVECTOR_H_*/
