/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <iostream>

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

    using namespace juce;

    WavAudioFormat format;
    AudioFormatManager manager;
    manager.registerBasicFormats();

    double numSamples = 44100;
    AudioBuffer<float> buffer(1, numSamples);
    buffer.clear();
    float* WritePtr = buffer.getWritePointer(0);


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
