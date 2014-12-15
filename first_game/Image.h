#ifndef IMAGE_H
#define IMAGE_H

#include "Project.h"

class Image
{
private:
	SDL_Texture* texture;
	int w;
	int h;
public:
	friend class Graphics;

	int GetWidth();
	int GetHeight();
};

#endif