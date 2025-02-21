/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#define _USE_MATH_DEFINES

#include <JuceHeader.h>
#include <iostream>
#include <math.h>

//==============================================================================
class SineOsc {
    public:
        SineOsc(double sampleRate, double amplitude, double phase)
        {
            mTs = 1 / sampleRate;
            mAmp = amplitude;
            mPhase = phase;
        }
        double process(double freq)
        {
            double newPhase = mPhase + freq * 2 * M_PI * mTs;
            mPhase = fmod(newPhase, 2 * M_PI);
            return mAmp * cos(mPhase);
        }

    private:
        double mTs;
        double mAmp;
        double mPhase;
};

class ADEnv {
    public:
        ADEnv(double sampleRate, double attackTime,
            double decayTime, double peakVal)
        {
            mPeakVal = peakVal;
            mAttackRate = peakVal / (attackTime * sampleRate);
            mDecayRate = peakVal / (decayTime * sampleRate);
            mSamples = (attackTime * sampleRate) + (decayTime * sampleRate);
            mValue = 0;
            mState = 0; //0 attack, 1 release, 2 finish
        }
        int numSamples()
        {
            return mSamples;
        }
        double process()
        {
            if (mState == 0)
            {
                mValue += mAttackRate;
                if (mValue >= mPeakVal)
                {
                    mState = 1;
                }
            }
            if (mState == 1)
            {
                mValue -= mDecayRate;
                if (mValue <= 0)
                {
                    mState = 2;
                }
            }
            return mValue;
        }

    private:
        double mPeakVal;
        double mAttackRate;
        double mDecayRate;
        double mValue;
        int mState;
        int mSamples;
};


int main (int argc, char* argv[])
{
    using namespace juce;

    WavAudioFormat format;
    AudioFormatManager manager;
    manager.registerBasicFormats();

    double numSamples = 44100;
    AudioBuffer<float> buffer(1, numSamples);
    buffer.clear();
    float* WritePtr = buffer.getWritePointer(0);

    SineOsc sine(numSamples, 0.5, 0);
    ADEnv env(numSamples, 0.1, 1, 1);

    //sine process
        //squarewave
    int nHarmonics = 10;
    double nyquist = numSamples / 2;
    std::vector<SineOsc> oscillators;
    for (int i = 0; i < nHarmonics; i++)
    {
        oscillators.push_back(SineOsc(numSamples, 0.5, -M_PI / 2));
    }
    for (int i = 0; i < numSamples; i++)
    {
        WritePtr[i] = 0;
        for (int j = 1; j < nHarmonics; j++)
        {
            int h = 2 * j - 1;
            WritePtr[i] = env.process() * (WritePtr[i] + (1.0 / h) * oscillators[j].process(h * 100.0));
        }
    }


    // Write file
    juce::File output("C:\\Users\\maxbu\\Desktop\\osc_test.wav");
    FileOutputStream* outputStream = new FileOutputStream(output);
    outputStream->setPosition(0); // in case it exists
    outputStream->truncate();
    AudioFormatWriter* writer =
        format.createWriterFor(outputStream, 44100, 1, 16, {}, 0);
    writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);
    delete writer;
    return 0;
}
