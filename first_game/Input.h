#ifndef INPUT_H
#define INPUT_H

#include "Project.h"

typedef unsigned char byte;

struct POINT{
	long x;
	long y;
};

class Input
{
private:
	SDL_Event evt;
	SDL_Event userEvt;
public:
	Input();
	void Update();

	bool IsMouseButtonDown(byte key);
	bool IsMouseButtonUp(byte key);
	POINT GetButtonDownCoords();

	bool IsKeyDown(byte key);
	bool IsKeyUp(byte key);
	byte GetPressedKey();

//	void RegisterUserEvent();
//	void PushUserEvent();
//	bool IsUserEvent();

	bool IsExit();
};

#endif