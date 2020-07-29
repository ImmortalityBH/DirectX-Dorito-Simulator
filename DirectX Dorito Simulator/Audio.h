#pragma once

#include <irrKlang.h>
#include <string>

class Sound;

class AudioEngine
{
public:
	AudioEngine() = default;
	AudioEngine(const AudioEngine&) = delete;
	AudioEngine& operator=(const AudioEngine&) = delete;
	~AudioEngine();

	bool init();
	void playSound2D(std::string filePath, bool looped = false);
	void playSound3D(Sound& sound, float x, float y, 
		float z, bool looped = false);

	irrklang::ISoundEngine* getEngine() const { return engine; }
private:
	irrklang::ISoundEngine* engine = nullptr;
};

class Sound
{
public:
	Sound(AudioEngine& engine, std::string filePath);
	~Sound();

	void setVolume(float volume);
	irrklang::ISoundSource* getSource() const { return source; }
private:
	irrklang::ISoundSource* source;
};
/*#include "Win.h"
#include <xaudio2.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid")

#include <string>
#include <vector>

/*class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	IXAudio2* getEffectDevice();
	IXAudio2MasteringVoice* getMusicVoice();

	bool init();
	void createDevice();
	void stop();
	void start();
	
private:
	bool audioAvailable;
	IXAudio2* pEffectDevice = nullptr;
	IXAudio2* pMusicDevice = nullptr;
	IXAudio2MasteringVoice* pEffectMasterVoice = nullptr;
	IXAudio2MasteringVoice* pMusicMasterVoice = nullptr;
};*/

/*class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	bool init();
	void start();
	void stop();
	bool loadFile(const std::wstring& filename, std::vector<BYTE>& audioData, WAVEFORMATEX** wafeFormatEx, unsigned int& waveLength);
private:
	IXAudio2* pAudioDevice = nullptr;
	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	IMFAttributes* pReaderConfig = nullptr;
	IMFSourceReader* pSourceReader = nullptr;
};*/

