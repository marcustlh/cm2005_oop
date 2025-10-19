#pragma once

#include <JuceHeader.h>


class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSec);
    void setPositionRelative(double pos);

    // Set reverb balance (dry/wet) for the audio
    void setReverbBalance(float balanceValue);

    // Set reverb damping for the audio
    void setReverbDamping(float damping);

    // Set reverb roomSize for the audio
    void setReverbRoomSize(float roomSize);

    void start();
    void stop();

    bool isLooping() const;

    // Functions to set the playback to loop or not to loop
    void setLoop();
    void unsetLoop();

    double getPositionRelative();
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    // Variables for manipulating the reverb of the resampleSource
    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    // Stores the reverb Parameters
    juce::Reverb::Parameters reverbParameters;

};
