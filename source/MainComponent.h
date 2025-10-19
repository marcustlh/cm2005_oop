#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
class MainComponent : public juce::AudioAppComponent
{
	public:
		//==============================================================================
		MainComponent();
		~MainComponent() override;

		//==============================================================================
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
		void releaseResources() override;

		//==============================================================================
		void paint(juce::Graphics& g) override;
		void resized() override;

	private:
		juce::AudioFormatManager formatManager;
		juce::AudioThumbnailCache thumbCache{100}; //store up to 100 waveforms

		int channelL = 0;
		int channelR = 1;

		PlaylistComponent playlistComponent{ formatManager };
		DJAudioPlayer playerLeft{ formatManager };
		DeckGUI deckGUILeft{ &playerLeft,&playlistComponent, formatManager, thumbCache, channelL };

		DJAudioPlayer playerRight{ formatManager };
		DeckGUI deckGUIRight{ &playerRight, &playlistComponent, formatManager, thumbCache, channelR };

		juce::Label waveformLabel;
		juce::Label posLabel;
		juce::Label widgetLabel;
		juce::Label playlistLabel;

		juce::MixerAudioSource mixerSource;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
