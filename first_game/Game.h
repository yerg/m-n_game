#ifndef GAME_H
#define GAME_H

#include "Project.h"

class Graphics;
class Input;
class Screen;

class Game
{
private:
	bool run;

	Graphics* graphics;
	Input* input;
	Screen* screen;

public:
	Game();
	int Execute(Screen* startscreen, int width, int height, const char *title);
	Graphics* GetGraphics();
	Input* GetInput();
	Screen* GetScreen();
	void SetScreen(Screen* screen);
	void Exit();
};

#endif