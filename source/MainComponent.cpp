#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize (800, 600);

	// Some platforms require permissions to open input channels so request that here
	if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
		&& ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
	{
		juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
			[&](bool granted) { if (granted)  setAudioChannels(2, 2); });
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels (0, 2);
	}

	// Add application components and make them visible
	addAndMakeVisible(deckGUILeft);
	addAndMakeVisible(deckGUIRight);
	addAndMakeVisible(playlistComponent);

	// Add file formats enabled by JUCE
	formatManager.registerBasicFormats();

	// Add Labels and customize visuals for labels
	addAndMakeVisible(waveformLabel);
	waveformLabel.setText("Waveforms", juce::dontSendNotification);
	waveformLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	waveformLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(posLabel);
	posLabel.setText("Playback", juce::dontSendNotification);
	posLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	posLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(widgetLabel);

	widgetLabel.setText("Widgets\nControls", juce::dontSendNotification);
	widgetLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	widgetLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(playlistLabel);

	playlistLabel.setText("Drag Files here to add to Library", juce::dontSendNotification);
	playlistLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	playlistLabel.setJustificationType(juce::Justification::centred);
}

MainComponent::~MainComponent()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playlistComponent.prepareToPlay(samplesPerBlockExpected, sampleRate);

	playerLeft.prepareToPlay(samplesPerBlockExpected, sampleRate);
	playerRight.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.addInputSource(&playerLeft, false);
	mixerSource.addInputSource(&playerRight, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	playlistComponent.releaseResources();
	playerLeft.releaseResources();
	playerRight.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	double rowH = getHeight() / 10;
	double colW = getWidth() / 7;

	// Position labels to left side of screen
	waveformLabel.setBounds(0, 0, colW, rowH * 2);
	posLabel.setBounds(0, rowH * 2, colW, rowH * 1.5);
	widgetLabel.setBounds(0, rowH * 3.5, colW, rowH * 3.5);
	playlistLabel.setBounds(0, rowH * 7, colW, rowH * 3);

	// Add GUIs
	deckGUILeft.setBounds(colW, 0, colW * 3, rowH * 7);
	deckGUIRight.setBounds(colW * 4, 0, colW * 3, rowH * 7);

	// Add playlist
	playlistComponent.setBounds(colW, rowH * 7, colW * 6, rowH * 3.5);

}

//==============================================================================
