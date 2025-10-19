#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
class PlaylistComponent : public juce::Component, 
                          public juce::TableListBoxModel, 
                          public juce::Button::Listener, 
                          public juce:: FileDragAndDropTarget,
                          public juce:: TextEditor::Listener
{
    public:
        PlaylistComponent(juce::AudioFormatManager& formatManager);
        ~PlaylistComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

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

        Component* refreshComponentForCell(int rowNumber,
                                           int columnId,
                                           bool isRowSelected,
                                           Component* existingComponentToUpdate) override;

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
        void releaseResources();

        void buttonClicked(juce::Button* button) override;

        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        void filesDropped(const juce::StringArray& files, int x, int y) override;
        void textEditorTextChanged(juce::TextEditor&) override;

        std::vector<std::string> playListL;
        std::vector<std::string> playListR;

    private:

        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;

        // Playlist displayed as a table list
        juce::TableListBox tableComponent;

        // Vectors to store music file metadata
        std::vector<std::string> inputFiles;
        std::vector<std::string> interestedFiles;
        std::vector<std::string> trackTitles;
        std::vector<std::string> interestedTitle;
        std::vector<int> trackDurations;
        std::vector<int> interestedDuration;

        // Search bar and label to allow for searching functionality 
        juce::TextEditor searchBar;
        juce::Label searchLabel;

        //==============================================================================
        // User defined variables to process data
        void addToChannelList(std::string filepath, int channel);
        void getAudioLength(juce::URL audioURL);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
