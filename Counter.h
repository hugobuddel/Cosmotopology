#ifndef COUNTER_H_
#define COUNTER_H_

#include <string>
#include <ctime>

class Counter
{
public:
    // Constructor and destructor
    Counter(std::string pInitString, double pMaximum, int pColor=42);
    virtual ~Counter();
    
    // Start counter
    void printini();
    
    // Update counter
    void update();
    
    // Quit counter
    void quit();
    
private:
    // Set the color of the prompt
    void setcolor();
    
    // String that is shown before counter
    std::string mInitString;
    
    // maximum of the counter
    double mMaximumInTicks;
    double mTicksPartialBlock;
    double mTicksPerBlock;
    double mTicksPerBlockPart;
    double mTicks;
    
    // The color of the counter
    int    mColor;
    
    // Bright color?
    int    mBold;
    
    // Begin and current counter.
    time_t mCBegin;
    time_t mCNow;
};

#endif /*COUNTER_H_*/
