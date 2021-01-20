#include "Audio.h"
#include <iostream>
#include <GL/freeglut_std.h>

Audio::Audio()
{
	soundEngine = createIrrKlangDevice();
	if (!soundEngine) {
		std::cout << "error starting sound engine" << std::endl;
	} // error starting up the engine

	soundEngine->setSoundVolume(0.6f);

	bindAudio("music/menu_2.wav", MENU);
	bindAudio("music/level_1.wav", LEVEL_1);
	bindAudio("music/menu_click.wav", MENU_CLICK);
	bindAudio("music/incorrect.wav", INCORRECT);
	bindAudio("music/alarma.wav", ALARM);
	bindAudio("music/level_passed.wav", LEVEL_PASSED);
	bindAudio("music/choque.wav", CHOQUE);
	bindAudio("music/level_2.wav", LEVEL_2);
	bindAudio("music/level_3.wav", LEVEL_3);
	bindAudio("music/theshire.wav", END_GAME);
	bindAudio("music/mariocoin.wav", COIN);
	bindAudio("music/death.wav", LOOSE_LIFE);
	bindAudio("music/endgame.wav", GAME_OVER);
	bindAudio("music/diamantes.wav", DIAMOND);
	bindAudio("music/keys.wav", KEY);
	bindAudio("music/money_bag.wav", BAG);
	bindAudio("music/rock_destroy.wav", ROCK);
	bindAudio("music/gunshot.wav", PUNYETAZO_PLACA_METAL);
	bindAudio("music/explosion.wav", EXPLOSION);
	bindAudio("music/wall_break.wav", WALL_BREAK);

	sounds[LEVEL_1]->setDefaultVolume(0.2f);
	sounds[LEVEL_2]->setDefaultVolume(0.2f);
	sounds[LEVEL_3]->setDefaultVolume(0.2f);
	sounds[ROCK]->setDefaultVolume(0.1f);
	sounds[CHOQUE]->setDefaultVolume(0.3f);
	sounds[ALARM]->setDefaultVolume(0.3f);
	sounds[PUNYETAZO_PLACA_METAL]->setDefaultVolume(0.3f);
	//sounds[WALL_BREAK]->setDefaultVolume(0.3f);

	soundEngine->setSoundVolume(0.6f);

	for (auto& i : playing) i = false;
}


Audio::~Audio()
{
	stopAll();
	/*for (int i = 0; i < NUM_SOUNDS; ++i) {
		sounds[i]->drop();
		sounds[i] = 0;
	}*/
	soundEngine->drop();
}

void Audio::bindAudio(const char * file, int i) {
	sounds[i] = soundEngine->addSoundSourceFromFile(file);
}

void Audio::play(int audio, bool loop) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (!playing[audio] && currentTime - lastPlayed[audio] > SOUND_REPLAY)
	{
		soundEngine->play2D(sounds[audio], loop);
		if (loop) playing[audio] = true;
		lastPlayed[audio] = currentTime;
	}	
}

void Audio::stopAll() {
	soundEngine->stopAllSounds();
	for (auto& i : playing) i = false;
}

void Audio::stop(int audio)
{
	soundEngine->stopAllSoundsOfSoundSource(sounds[audio]);
	playing[audio] = false;
}

void Audio::playMenu()
{
	for (int i = 0; i < NUM_SOUNDS; i++)
	{
		if (i != MENU && i != MENU_CLICK)
		{
			stop(i);
		}
	}
	if (!playing[MENU])
	{
		play(MENU, true);
	}
}

