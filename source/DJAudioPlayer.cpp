#include "DJAudioPlayer.h"
#include <deque>

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager
) : formatManager(_formatManager)
{
};

DJAudioPlayer::~DJAudioPlayer()
{

};

//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// Tells transport source to get ready
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	// Prepare to play the resampled resource
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	// Prepare to play the reverb source
	reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	reverbSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
	reverbSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
	// Take audio url and convert it into a input stream and place it into a reader
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr)
	{
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		// Parse audio source into transport source
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		// Pass ownership of pointer to class scope variable, retain access to it
		readerSource.reset(newSource.release());
	}
}

void DJAudioPlayer::setGain(double gain)
{
	if (gain < 0 || gain > 1.0)
	{
		std::cout << "DJAudioPlayer::setGain: gain should be between 0 and 1" << std::endl;
	}
	else
	{
		transportSource.setGain(gain);
	}
}

void DJAudioPlayer::setSpeed(double ratio)
{
	if (ratio < 0 || ratio > 10.0)
	{
		std::cout << "DJAudioPlayer::setSpeed: ratio should be between 0 and 10" << std::endl;
	}
	else
	{
		resampleSource.setResamplingRatio(ratio);
	}
}

void DJAudioPlayer::setPosition(double posInSec)
{
	transportSource.setPosition(posInSec);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
	if (pos < 0 || pos > 1.0) {
		std::cout << "DJAudioPlayer::setPositionRelative: relative pos should be between 0 and 1." << std::endl;
	}
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
	}
}

void DJAudioPlayer::setReverbBalance(float balanceValue)
{
	// Makes sure reverb value is not out of range
	if (balanceValue < 0 || balanceValue > 1.0f)
	{
		// Gives warning to user that the ratio is out of range
		DBG("DJAudioPlayer::setReverbBalance - Reverb value is out of range, should be between 0 - 1");
	}
	else // Reverb values is in range
	{
		if (balanceValue >= 0.0 && balanceValue <= 0.5f)
		{
			// Set dry level to max while increase the value of wet level 
			reverbParameters.dryLevel = 1.0f;
			reverbParameters.wetLevel += balanceValue * 2;
			reverbSource.setParameters(reverbParameters);
		}
		else if (balanceValue > 0.5f && balanceValue <= 1.0f)
		{
			// Set wet level to max while decresing the value of dry level 
			reverbParameters.wetLevel = 1.0f;
			reverbParameters.dryLevel -= (balanceValue - 0.5f) * 2;
			reverbSource.setParameters(reverbParameters);
		}
	}
};

void DJAudioPlayer::setReverbDamping(float damping)
{
	// Makes sure reverb value is not out of range
	if (damping < 0 || damping > 1.0f)
	{
		// Gives warning to user that the ratio is out of range
		DBG("DJAudioPlayer::setReverbDamping - Reverb value is out of range, should be between 0 - 1");
	}
	else // Reverb values is in range
	{
		// Sets damping level of reverb
		reverbParameters.damping = damping;
		// Sets the source parameters
		reverbSource.setParameters(reverbParameters);
	}
};

void DJAudioPlayer::setReverbRoomSize(float roomSize)
{
	// Makes sure reverb value is not out of range
	if (roomSize < 0 || roomSize > 1.0f)
	{
		// Gives warning to user that the ratio is out of range
		DBG("DJAudioPlayer::setReverbRoomSize - Reverb value is out of range, should be between 0 - 1");
	}
	else // Reverb values is in range
	{
		// Sets room size level of reverb
		reverbParameters.roomSize = roomSize;
		// Sets the source parameters
		reverbSource.setParameters(reverbParameters);
	}
};

void DJAudioPlayer::start()
{
	transportSource.start();
}
void DJAudioPlayer::stop()
{
	transportSource.stop();
}

bool DJAudioPlayer::isLooping() const
{
	return false;
}

void DJAudioPlayer::setLoop()
{
	if (readerSource != nullptr)
	{
		readerSource->setLooping(true);
	}
}

void DJAudioPlayer::unsetLoop()
{
	if (readerSource != nullptr)
	{
		readerSource->setLooping(false);
	}
}

double DJAudioPlayer::getPositionRelative()
{
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}