#pragma once

#include "Win.h"
#include <xaudio2.h>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	bool init();
private:
	IXAudio2* pAudioDevice = nullptr;
	IXAudio2MasteringVoice* pMasterVoice = nullptr;
};

