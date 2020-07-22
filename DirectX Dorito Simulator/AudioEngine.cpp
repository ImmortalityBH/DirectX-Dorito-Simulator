#include "AudioEngine.h"
#include "Error.h"
#include "d3dUtil.h"

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
	ReleaseCOM(pAudioDevice);
}

bool AudioEngine::init()
{
	HRESULT hr;

	hr = XAudio2Create(&pAudioDevice);
	if (FAILED(hr))
		return false;

	hr = pAudioDevice->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr))
		return false;

	return true;
}
