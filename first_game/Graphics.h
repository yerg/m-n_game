#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Project.h"
#include "SDL_image.h"
#include "Image.h"
class Image;

class Graphics
{
private:
	SDL_Renderer* Renderer;
public:
	SDL_Window* mainWindow;
	Graphics(int width, int height, const char *title);
	Image* NewImage(char* file);
	Image* NewImage(char* file, int r, int g, int b);
	bool DrawImage(Image* img, int x, int y);
	bool DrawImage(Image* img, int x, int y, int startX, int startY, int endX, int endY, int w, int h);
	bool DrawImage(Image* img, int x, int y, int startX, int startY, int endX, int endY, double scale=1.0);
	void Flip();
};

#endif