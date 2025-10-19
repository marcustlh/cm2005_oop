#pragma once

#include <JuceHeader.h>

class WaveformDisplay  : public juce::Component, 
                         public juce::ChangeListener
{
    public:
        WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                        juce::AudioThumbnailCache& cacheToUse);
        ~WaveformDisplay() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        void changeListenerCallback(juce::ChangeBroadcaster* source) override;

        void loadURL(juce::URL audioURL);
        void setPositionRelative(double pos);

    private:

        juce::AudioThumbnail audioThumb;
        double position;
        std::string nowPlaying;
        bool fileLoaded;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
