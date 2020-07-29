#include "Audio/Audio.h"
#include "Utility/Error.h"
//#include "d3dUtil.h"
using namespace irrklang;

AudioEngine::~AudioEngine()
{
	//while (engine != nullptr)
	//{
		engine->drop();
	//}
}

bool AudioEngine::init()
{
	engine = createIrrKlangDevice();
	//if (engine == nullptr)
	//{
	//	return false;//no error displayed left up to window class
	//}
	return true;
}

void AudioEngine::playSound2D(std::string filePath, bool looped)
{
	engine->play2D(filePath.c_str(), looped);
}

void AudioEngine::playSound3D(Sound& sound, float x, float y, 
	float z, bool looped)
{
	vec3df pos(x, y, z);
	engine->play3D(sound.getSource(), pos, looped);
}

Sound::Sound(AudioEngine& engine, std::string filePath)
{
	source = engine.getEngine()->addSoundSourceFromFile(filePath.c_str());
}

Sound::~Sound()
{
	source->drop();
}

void Sound::setVolume(float volume)
{
	source->setDefaultVolume(volume);
}
/*AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
	//shutdown media foundation
	ReleaseCOM(pReaderConfig);
	ReleaseCOM(pSourceReader);
	MFShutdown();

	//destroy master voice
	pMasterVoice->DestroyVoice();
	//stop audio engine
	pAudioDevice->StopEngine();
	ReleaseCOM(pAudioDevice);//not sure if this is legal
}

bool AudioEngine::init()
{
	HRESULT hr = NULL;

	hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
		return false;

	hr = MFCreateAttributes(&pReaderConfig, 1);
	if (FAILED(hr))
		return false;

	hr = pReaderConfig->SetUINT32(MF_LOW_LATENCY, true);
	if (FAILED(hr))
		return false;

	hr = XAudio2Create(&pAudioDevice);
	if (FAILED(hr))
		return false;

	hr = pAudioDevice->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr))
		return false;

	return true;
}

void AudioEngine::start()
{
	pAudioDevice->StartEngine();
}

void AudioEngine::stop()
{
	pAudioDevice->StopEngine();
}

bool AudioEngine::loadFile(const std::wstring& filename, std::vector<BYTE>& audioData, WAVEFORMATEX** wafeFormatEx, unsigned int& waveLength)
{
	HRESULT hr = NULL;
	//locals !REMEMBER TO DESTROY!
	IMFMediaType* pMediaType = nullptr;
	
	hr = pSourceReader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(hr))
		return false;
	hr = pSourceReader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
	if (FAILED(hr))
		return false;

	hr = MFCreateSourceReaderFromURL(filename.c_str(), 
		pReaderConfig, &pSourceReader);
	if (FAILED(hr))
		return false;
	
	hr = pSourceReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &pMediaType);
	if (FAILED(hr))
		return false;

	GUID majorType{};
	hr = pMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (majorType != MFMediaType_Audio)
		return false;

	GUID subType{};
	hr = pMediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{

	}
	else
	{
		//
	}
	IMFMediaType* pAudioType = nullptr;


	ReleaseCOM(pMediaType);
	return true;
}*/


