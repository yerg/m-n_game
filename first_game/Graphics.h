#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Project.h"

class Image;

class Graphics
{
private:
	SDL_Renderer* renderer;
	SDL_Window* mainWindow;
public:
	Graphics(int width, int height, const char *title);
	~Graphics();
	Image* NewImage(char* file);
	Image* NewImage(char* file, int r, int g, int b);
	bool DrawImage(Image* img, int x, int y);
	bool DrawImage(Image* img, int x, int y, int startX, int startY, int srcW, int srcH, int w, int h);
	bool DrawImage(Image* img, int x, int y, int startX, int startY, int srcW, int srcH, double scale=1.0);
	void GetWindowSize(int &w, int &h) {SDL_GetWindowSize(mainWindow, &w, &h);}
	void Flip();
};

#endif