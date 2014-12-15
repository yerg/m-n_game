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
	SDL_Window* mainWindow;
public:

	Graphics(int width, int height, const char *title);
	Image* NewImage(char* file);
	Image* NewImage(char* file, int r, int g, int b);
	bool DrawImage(Image* img, int x, int y);
	bool DrawImage(Image* img, int x, int y, int startX, int startY, int endX, int endY);

	void Flip();
};

#endif