/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

//==============================================================================
class SineOsc {
    public:
        SineOsc(double sampleRate, double amplitude, double phase)
        {
            mTs = 1 / sampleRate;
            mAmp = amplitude;
            mPhase = phase;
        }

    private:
        double mTs;
        double mAmp;
        double mPhase;
};



int main (int argc, char* argv[])
{

    // ..your code goes here!


    return 0;
}
