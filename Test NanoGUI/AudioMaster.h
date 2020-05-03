#pragma once
#include <irrklang/irrKlang.h>
#include <string>

class AudioMaster {
private:
	irrklang::ISoundEngine* SoundEngineMain;
	irrklang::ISoundEngine* SoundEngineEffects = irrklang::createIrrKlangDevice();

public:
	AudioMaster() {}
	~AudioMaster() {}

	void genMainEngine() {
		SoundEngineMain = irrklang::createIrrKlangDevice();
		SoundEngineMain->stopAllSounds();
	}

	void PlaySound(irrklang::ISoundEngine* SoundEngine, std::string path, bool loop) {
		SoundEngine->play2D(path.c_str(),loop);
	}

	void StopSounds(irrklang::ISoundEngine* SoundEngine) {
		SoundEngine->stopAllSounds();
	}

	void PlayNewSound(irrklang::ISoundEngine* SoundEngine, std::string path, bool loop) {
		StopSounds(SoundEngine);
		PlaySound(SoundEngine, path, loop);
	}

	void SetEngineVolume(irrklang::ISoundEngine* SoundEngine, float vol) {
		SoundEngine->setSoundVolume((vol / 100.f));
	}

	irrklang::ISoundEngine* GetMainSoundEngine() { return this->SoundEngineMain; }
	irrklang::ISoundEngine* GetEffectsSoundEngine() { return this->SoundEngineEffects; }
};