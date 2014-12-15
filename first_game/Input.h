#ifndef _INPUT_H_
#define _INPUT_H_

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
public:
	void Update();

	bool IsMouseButtonDown(byte key);
	bool IsMouseButtonUp(byte key);
	POINT GetButtonDownCoords();

	bool IsKeyDown(byte key);
	bool IsKeyUp(byte key);
	byte GetPressedKey();

	bool IsExit();
};

#endif