#include <JuceHeader.h>
#include "DeckGUI.h"
#include <cmath>
#include "PlaylistComponent.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
				PlaylistComponent* _playlistComponent,
				juce::AudioFormatManager& formatManagerToUse,
				juce::AudioThumbnailCache& cacheToUse,
				int channelToUse
				) : player(_player),
				playlistComponent(_playlistComponent),
				waveformDisplay(formatManagerToUse, cacheToUse),
				channel(channelToUse)
{
	// Add buttons for each GUI 
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(nextButton);
	addAndMakeVisible(loop);

	// Adding listeners to buttons
	playButton.addListener(this);
	stopButton.addListener(this);
	nextButton.addListener(this);
	loop.addListener(this);

	// Add sliders for each GUI and customizing each of them
	addAndMakeVisible(posSlider);
	posSlider.addListener(this);
	posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	posSlider.setRange(0.0, 1.0);
	posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(volSlider);
	volSlider.addListener(this);
	volSlider.setRange(0.0, 1.0);
	volSlider.setValue(0.5); // set inital volume to half of max
	volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(volLabel);
	volLabel.setText("Volume", juce::dontSendNotification);
	volLabel.attachToComponent(&volSlider, false);
	volLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(speedSlider);
	speedSlider.addListener(this);
	speedSlider.setRange(0.5, 2, 0.1); // min range 0.5x speed, max range 2x speed
	speedSlider.setValue(1.0); // set default speed at 1x
	speedSlider.setTextValueSuffix(" x");
	speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 15);
	speedSlider.setMouseDragSensitivity(80);
	addAndMakeVisible(speedLabel);
	speedLabel.setText("Speed", juce::dontSendNotification);
	speedLabel.attachToComponent(&speedSlider, false);
	speedLabel.setJustificationType(juce::Justification::centred);


	// Add reverb balance (dry/wet) slider and label (R3*)
	addAndMakeVisible(reverbBalanceSlider);
	reverbBalanceSlider.addListener(this);
	reverbBalanceSlider.setRange(0.0, 1.0);
	reverbBalanceSlider.setValue(0.5); // set inital value to 0.5
	reverbBalanceSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	reverbBalanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(reverbBalanceLabel);
	reverbBalanceLabel.setText("Balance", juce::dontSendNotification);
	reverbBalanceLabel.attachToComponent(&reverbBalanceSlider, false);
	reverbBalanceLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(wetLabel);
	wetLabel.setText("Wet", juce::NotificationType::dontSendNotification);
	wetLabel.setEditable(false);
	
	addAndMakeVisible(dryLabel);
	dryLabel.setText("Dry", juce::NotificationType::dontSendNotification);
	dryLabel.setEditable(false);

	// Add reverb damping level and slider (R3*)
	addAndMakeVisible(reverbDampingSlider);
	reverbDampingSlider.addListener(this);
	reverbDampingSlider.setRange(0.0, 1.0);
	reverbDampingSlider.setValue(0.0); // set initial value to 0
	reverbDampingSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	reverbDampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(reverbDampingLabel);
	reverbDampingLabel.setText("Damping", juce::NotificationType::dontSendNotification);
	reverbDampingLabel.setEditable(false);

	// Add reverb room size slider and label (R3*)
	addAndMakeVisible(reverbRoomSizeSlider);
	reverbRoomSizeSlider.addListener(this);
	reverbRoomSizeSlider.setRange(0.0, 1.0); // min value 0.0, max value 1.0
	reverbRoomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(reverbRoomSizeLabel);
	reverbRoomSizeLabel.setText("Room Size", juce::dontSendNotification);
	reverbRoomSizeLabel.attachToComponent(&reverbRoomSizeSlider, false);
	reverbRoomSizeLabel.setJustificationType(juce::Justification::centred);

	// Set colour scheme for sliders
	getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::rosybrown); // Dial
	getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightslategrey); // Body
	getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightslategrey); // Body

	// Add waveform to each GUI
	addAndMakeVisible(waveformDisplay);

	// Add list of songs to be played next
	upNext.getHeader().addColumn("Up Next", 1, 100);
	upNext.setModel(this);
	addAndMakeVisible(upNext);

	// Start thread calling 10 times per second (once every 0.1 sec)
	startTimer(100);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::timerCallback()
{
	waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::paint(juce::Graphics& g)
{

}

void DeckGUI::resized()
{
	double rowH = getHeight() / 6;
	double colW = getWidth() / 4;

	playButton.setBounds(colW + 10, rowH * 5.5 + 5, colW - 20, rowH - 40);
	stopButton.setBounds(colW * 2 + 10, rowH * 5.5 + 5, colW - 20, rowH - 40);
	nextButton.setBounds(colW * 3 + 10, rowH * 5.5 + 5, colW - 20, rowH - 40);

	posSlider.setBounds(0, rowH * 2 - 10, getWidth(), rowH);

	volSlider.setBounds(0, rowH * 3, colW, rowH - 30);
	speedSlider.setBounds(colW, rowH * 3 - 15, colW * 1.25, rowH * 1.25);
	
	upNext.setBounds(colW * 2.5, rowH * 2.5, colW * 1.5 - 10, rowH * 1.5);
	loop.setBounds(colW * 2.5, rowH * 4, colW * 1.5 - 10, rowH - 20);

	reverbBalanceSlider.setBounds(0, rowH * 4, colW, rowH);
	dryLabel.setBounds(0, rowH * 5, colW, rowH * 0.25);
	wetLabel.setBounds(colW - 30, rowH * 5, colW, rowH * 0.25);

	reverbDampingSlider.setBounds(colW + 10, rowH * 4 + 5, colW, rowH);
	reverbDampingLabel.setBounds(colW + 20, rowH * 5, colW, 20);

	reverbRoomSizeSlider.setBounds(colW * 2.5, rowH * 5, colW, rowH - 40);

	waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
	if (button == &playButton)
	{
		player->start();
	}
	if (button == &stopButton)
	{
		player->stop();
	}
	if (button == &nextButton)
	{
		// Handling next button for left channel
		if (channel == 0 && playlistComponent->playListL.size() > 0) //handle only if there are songs added 
		{
			// Get URL to first song of Left playlist
			juce::URL fileURL = juce::URL{ juce::File{playlistComponent->playListL[0]} };
			// Load the first URL 
			player->loadURL(fileURL);
			// Display the waveforms
			waveformDisplay.loadURL(fileURL);
			// Pop the first URL of the Left playlist so it doesn't replay
			playlistComponent->playListL.erase(playlistComponent->playListL.begin());
		}

		// Handling next button for right channel
		if (channel == 1 && playlistComponent->playListR.size() > 0)
		{
			// Get URL to first song of playlist
			juce::URL fileURL = juce::URL{ juce::File{playlistComponent->playListR[0]} };
			// Load the first URL 
			player->loadURL(fileURL);
			// Display the waveforms
			waveformDisplay.loadURL(fileURL);
			// Pop the first URL of the Right playlist so it doesn't replay
			playlistComponent->playListR.erase(playlistComponent->playListR.begin()); //remove first element
		}

		// Buttons starts with indicating load. Once first songs have been loaded, we can change it to next 
		if (nextButton.getButtonText() == "LOAD")
		{
			nextButton.setButtonText("NEXT");
		}
		else
		{
			player->start(); // Starts player each time button labeled next is clicks
		}
	}
	if (button == &loop)
	{
		if (loop.getToggleState() == true)
		{
			player->setLoop();
		}
		else
		{
			player->unsetLoop();
		}
	}

	// Refresh up next table whenever a button is clicked
	upNext.updateContent();
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volSlider)
	{
		player->setGain(slider->getValue());
	}
	if (slider == &speedSlider)
	{
		player->setSpeed(slider->getValue());
	}
	if (slider == &posSlider)
	{
		player->setPositionRelative(slider->getValue());
	}
	if (slider == &reverbRoomSizeSlider)
	{
		player->setReverbRoomSize(slider->getValue());
	}
	if (slider == &reverbBalanceSlider)
	{
		player->setReverbBalance(slider->getValue());
	}
	if (slider == &reverbDampingSlider)
	{
		player-> setReverbDamping(slider->getValue());
	}
}

int DeckGUI::getNumRows()
{
	// Number of rows in the table depends on the number of songs loaded to the respective channels
	if (channel == 0) // Left
	{
		return playlistComponent->playListL.size();
	}
	if (channel == 1) // Right
	{
		return playlistComponent->playListR.size();
	}
}

void DeckGUI::paintRowBackground(juce::Graphics& g,
	int rowNumber,
	int width,
	int height,
	bool rowIsSelected)
{
	// Set background of each row in up next table
	g.fillAll(juce::Colours::mediumspringgreen);
}

void DeckGUI::paintCell(juce::Graphics& g,
	int rowNumber,
	int columnId,
	int width,
	int height,
	bool rowIsSelected)
{
	std::string filepath = "";

	// Get file path depending on channel
	if (channel == 0) // Left
	{
		filepath = playlistComponent->playListL[rowNumber];
	}
	if (channel == 1) // Right
	{
		filepath = playlistComponent->playListR[rowNumber];
	}

	// Extract file name from path 
	std::size_t startFilePos = filepath.find_last_of("\\");
	std::size_t startExtPos = filepath.find_last_of(".");
	std::string extn = filepath.substr(startExtPos + 1, filepath.length() - startExtPos);
	std::string file = filepath.substr(startFilePos + 1, filepath.length() - startFilePos - extn.size() - 2);
	// Draw name to each cell
	g.drawText(file,
		1, rowNumber,
		width - 4, height,
		juce::Justification::centredLeft,
		true);
}
