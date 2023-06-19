#include "Counter_i.h"

Counter::Counter(std::string pInitString, double pMaximum, int pColor)
  :
    mInitString(pInitString),
    mMaximumInTicks(pMaximum),
    mTicksPartialBlock(0),
    mTicks(0),
    
    // 
    mTicksPerBlock(mMaximumInTicks / 50),
    mTicksPerBlockPart(mMaximumInTicks / 400),
    mColor(pColor),
    mBold(0)
{
    // starting time
    time(&mCBegin);

    // convert color
    if (mColor != 42){
        if (mColor > 8){
            mColor = 30 + mColor - 8;
            mBold = 1;
        } else {
            mColor = 30 + mColor;
            mBold  = 1;    
        }
    }
}

Counter::~Counter()
{
}

void Counter::printini()
{
    cout << mInitString;
    // Space between the brackets is exactly 50 characters long.
    cout << "[0                      50                      100]" << endl;
    for (unsigned int i=0; i<mInitString.length(); i++) cout << " ";
    cout << "  \033[A\033[s\033[0,0H" << flush;
    setcolor();
}

void Counter::setcolor()
{
    if (mColor != 42) cout << "\033[" << mColor << "m";
    cout << flush;
}

void Counter::quit()
{
    // reset color
    cout << "\033[u";
    if (mColor != 42)
        cout << "\033[m";
    cout << endl;
}

void Counter::update()
{
    mTicksPartialBlock += 1;
    mTicks++;
    time(&mCNow);
    double seconds = difftime(mCNow, mCBegin);
    int seconds2go = (int)(seconds * (mMaximumInTicks - mTicks) / mTicks) + 1;
    int minutes2go = seconds2go / 60;
    seconds2go %= 60;
    int hours2go = minutes2go / 60;
    minutes2go %= 60; 
    
    // Move the cursor
    cout << "\033[u\033[D\033[s\033[0,0H" << flush;
    
    // Output full bars
    while (mTicksPartialBlock >= mTicksPerBlock) {
        mTicksPartialBlock -= mTicksPerBlock;
        std::cout << "\033[u";
        setcolor();
        cout << "█\033[s\033[0,0H" << std::flush;
    }
    
    double qq = mTicksPartialBlock;
    // ticks not yet in a full block (from 0 to 7)
    int ticksLeft = 0;
    while (qq > mTicksPerBlockPart){
        ticksLeft++;
        qq -= mTicksPerBlockPart;
    }
        
    std::cout << "\033[u";

    // Different strings to potentially use
    // "▁▂▃▄▅▆▇█" "▏▎▍▌▋▊▉█" "|/-\\|/-\\"

    switch (ticksLeft){
    case 0:
        // empty
        cout << "\033[C";
        break;
    case 1:
        cout << "▏";
        break;
    case 2:
        cout << "▎";
        break;
    case 3:
        cout << "▍";
        break;
    case 4:
        cout << "▌";
        break;
    case 5:
        cout << "▋";
        break;
    case 6:
        cout << "▊";
        break;
    case 7:
        cout << "▉";
        break;
    case 8: // should not happen
    default:
        cout << "█";
        break;
    }

    // Show time to go below counter
    cout << "\033[s" << flush;
    cout << "\n\033[" << mInitString.length() + 1;
    cout << "C\033[m";
    cout << hours2go << "h" << minutes2go << "m" <<seconds2go;
    cout << "s\033[K";
    cout << "\033[0;0H" << flush;

}
