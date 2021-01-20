#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#define SOUND_REPLAY 100
#define NUM_SOUNDS 19

enum AUDIOS {
	MENU, LEVEL_1, MENU_CLICK, INCORRECT, ALARM, LEVEL_PASSED, CHOQUE, LEVEL_2, LEVEL_3, END_GAME, COIN, LOOSE_LIFE, GAME_OVER, DIAMOND, KEY, BAG, ROCK, PUNYETAZO_PLACA_METAL, EXPLOSION, WALL_BREAK
};

class Audio
{
public:
	Audio();
	~Audio();

	static Audio &instance()
	{
		static Audio audio;

		return audio;
	}

	void bindAudio(const char* file, int i);
	void play(int audio, bool loop);
	void stopAll();
	void playMenu();
	void stop(int audio);

private:
	ISoundEngine* soundEngine;
	ISoundSource *sounds[NUM_SOUNDS];
	bool playing[NUM_SOUNDS];
	int lastPlayed[NUM_SOUNDS];
};

#endif // _AUDIO_INCLUDE

