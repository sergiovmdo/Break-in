#pragma once
#include <string>

#include "Scenes.h"

using namespace std;


const string PASSWORD_LEVEL_2 = "LTKIU";
const string PASSWORD_LEVEL_3 = "BPILE";

class Password
{
public:
	static int checkPassword(const string password)
	{
		if (password == PASSWORD_LEVEL_2)
		{
			return GAME_SCENE_2;
		}
		else if (password == PASSWORD_LEVEL_3)
		{
			return GAME_SCENE_3;
		}
		else
		{
			return -1;
		}
	}

};
