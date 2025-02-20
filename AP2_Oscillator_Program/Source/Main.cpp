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

    //sine process
    for (int i = 0; i < numSamples; i++)
    {
        WritePtr[i] = sine.process(100);
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
