#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse
                                ) : audioThumb(1000, formatManagerToUse, cacheToUse), 
                                    fileLoaded(false), 
                                    position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));// Clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // Draw an outline around the component

    g.setColour(juce::Colours::orange);
    if (fileLoaded) 
    {
        // Draw the waveforms
        g.setColour(juce::Colours::rosybrown);
        audioThumb.drawChannel(g,
            getLocalBounds(), // Area
            0, // Start time
            audioThumb.getTotalLength(), // Length of file as end time
            0,
            1.0f
        );

        // Draw the playhead
        g.setColour(juce::Colours::floralwhite);
        g.fillRect(position * getWidth(), 0, 2, getHeight());

        // Display the name of currently playing track on the waveform
        g.setColour(juce::Colours::floralwhite);
        g.setFont(16.0f);
        g.drawText(nowPlaying, getLocalBounds(),
            juce::Justification::centred, true);
    }
    else 
    {
        // File is not loaded 
        g.setColour(juce::Colours::rosybrown);
        g.setFont(20.0f);
        g.drawText("Load File to Channel to Begin...", getLocalBounds(),
            juce::Justification::centred, true);
    }
}

void WaveformDisplay::setPositionRelative(double pos) 
{
    if (pos != position) 
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded)
    {
        audioThumb.clear();
        fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
        if (fileLoaded)
        {
            std::string justFile = audioURL.toString(false).toStdString();
            std::size_t startFilePos = justFile.find_last_of("/");
            std::size_t startExtPos = justFile.find_last_of(".");
            std::string extn = justFile.substr(startExtPos + 1, justFile.length() - startExtPos);
            std::string file = justFile.substr(startFilePos + 1, justFile.length() - startFilePos - extn.size() - 2);

            nowPlaying = file;
            repaint();
        }
        else
        {
        }
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) 
{
    repaint();
}