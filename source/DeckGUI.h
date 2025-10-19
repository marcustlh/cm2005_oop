#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::TableListBoxModel,
    public juce::Timer
{
    public:
        DeckGUI(DJAudioPlayer* player,
                PlaylistComponent* playerComponent,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse, 
                int channelToUse);
        ~DeckGUI() override;

        /**Customise input graphics*/
        void paint (juce::Graphics&) override;
        void resized() override;

        void buttonClicked(juce::Button * button);
        void sliderValueChanged(juce::Slider * slider) override;

        int getNumRows() override;

        void paintRowBackground(juce::Graphics&,
                                int rowNumber,
                                int width,
                                int height,
                                bool rowIsSelected) override;
        
        void paintCell(juce::Graphics&,
                       int rowNumber,
                       int columnId,
                       int width,
                       int height,
                       bool rowIsSelected) override;

        void timerCallback() override;

    private:
        juce::FileChooser fChooser{ "Select a file..." };
        juce::TextButton playButton{ "PLAY" };
        juce::TextButton stopButton{ "STOP" };
        juce::TextButton nextButton{ "LOAD" };
        juce::ToggleButton loop{ "LOOP" };

        juce::Slider volSlider;
        juce::Slider speedSlider;
        juce::Slider posSlider;

        // Add labels to sliders 
        juce::Label volLabel;
        juce::Label speedLabel;

        // Control visual theme
        juce::LookAndFeel_V4 lookandfeel;

        // Create player associated with the GUI
        DJAudioPlayer* player;
        //Create playlist component associated with the GUI
        PlaylistComponent* playlistComponent;

        // Create waveform visual
        WaveformDisplay waveformDisplay;

        // Create table containing list of upcoming songs in the playlist
        juce::TableListBox upNext;

        // Variable indicating channel associated with the GUI (0 = Left, 1 = Right)
        int channel;

        // Initialize reverb balance (dry/wet) slider (R3*)
        juce::Slider reverbBalanceSlider;
        juce::Label reverbBalanceLabel;
        juce::Label wetLabel;
        juce::Label dryLabel;

        // Initialize reverb damping slider and label (R3*)
        juce::Slider reverbDampingSlider;
        juce::Label reverbDampingLabel;

        // Initialize reverb roomsize slider and label (R3*)
        juce::Slider reverbRoomSizeSlider;
        juce::Label reverbRoomSizeLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
