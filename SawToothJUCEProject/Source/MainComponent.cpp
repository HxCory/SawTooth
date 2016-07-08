/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    :   currentSampleRate(0.0),
        currentFrequency(440.0),
        targetFrequency(currentFrequency),
        numSines(10),
        envParam(5),
        freqDelta(0.0),
        aCoefficients(numSines, 0.0),
        fourierFreqs(numSines, 0.0)
    {
        
        addAndMakeVisible(frequencySlider);
        frequencySlider.setRange(50.0, 3000.0);
        frequencySlider.setSkewFactorFromMidPoint(440.0);
        frequencySlider.setValue(currentFrequency, dontSendNotification);
        frequencySlider.addListener(this);

        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        //setAudioChannels (2, 2);
        setAudioChannels(0, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        currentSampleRate = sampleRate;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
    }

    double poly(double x)
    {
        return x - ((x * x * x) / 6) + ((x * x * x * x * x) / 120) - 
        ((x * x * x * x * x * x * x) / 5040);
    }

    double envelope (double b, double x)
    {
        return std::exp(-b*x);
    }

    double sinePoly (double A, double freq)
    {
        return A*poly(freq);
    }

    void resized() override
    {
        frequencySlider.setBounds (10, 10, getWidth() - 20, 20);

    }

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &frequencySlider)
        {
            if (currentSampleRate > 0.0)
            {
                updateFreq();
            }
        }
    }

    void updateFreq()
    {
        const double cyclesPerSample = frequencySlider.getValue() / currentSampleRate; 
        freqDelta = cyclesPerSample * 2.0 * double_Pi;
        fourierFreqs[0] = 2.0 * double_Pi * frequencySlider.getValue();
        for (int j = 1; j < numSines; j++)
        {
            fourierFreqs[j] = fourierFreqs[j-1] + freqDelta;
        }
    }

    void constructWave()
    {
        double wave = 0.0;
        for(int i = 0; i < numSines; i++)
        {
            wave += envelope(envParam, fourierFreqs[i])*sinePoly(aCoefficients[i], fourierFreqs[i]);
        }
    }


private:
    //==============================================================================

    Slider frequencySlider;
    Slider phaseSlider;
    double currentSampleRate;
    double currentFrequency, targetFrequency;
    double numSines;
    double envParam;
    double freqDelta;
    std::vector<double> currentAngles = {0.0, 0.0};
    std::vector<double> aCoefficients;
    std::vector<double> fourierFreqs;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
