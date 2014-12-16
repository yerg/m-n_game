#include "Graphics.h"
Graphics::Graphics(int width, int height, const char *title)
{
	int retval = SDL_Init(SDL_INIT_EVERYTHING);
	if (retval) std::cout<<std::endl<<SDL_GetError();
	mainWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,	width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	Renderer = SDL_CreateRenderer(mainWindow, -1, 0);
}

Image* Graphics::NewImage(char* file)
{
	SDL_Surface* tmp = IMG_Load(file);
	Image* image = new Image();
	image->texture = SDL_CreateTextureFromSurface(Renderer, tmp);
	image->h=tmp->h; 
	image->w=tmp->w;
	SDL_FreeSurface(tmp);
	return image;
}

Image* Graphics::NewImage(char* file, int r, int g, int b)
{
	SDL_Surface* tmp = IMG_Load(file);
	Image* image = new Image();
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, r, g, b));
	image->texture = SDL_CreateTextureFromSurface(Renderer, tmp);
	image->h=tmp->h;
	image->w=tmp->w;
	SDL_FreeSurface(tmp);
	return image;
}

bool Graphics::DrawImage(Image* img, int x, int y)
{
	if(mainWindow == NULL || img->texture == NULL)
		return false;

	SDL_Rect Area;
	Area.x = x;
	Area.y = y;

	Area.w = img->h;
	Area.h = img->w;
	SDL_RenderCopy(Renderer, img->texture, NULL, &Area);
	return true;
}

bool Graphics::DrawImage(Image* img, int x, int y, int startX, int startY, int endX, int endY, int w, int h)
{
	if(mainWindow == NULL || img->texture == NULL)
		return false;

	SDL_Rect Area;
	Area.x = x;
	Area.y = y;
	Area.w = w;
	Area.h = h;

	SDL_Rect SrcArea;
	SrcArea.x = startX;
	SrcArea.y = startY;
	SrcArea.w = endX;
	SrcArea.h = endY;

	SDL_RenderCopy(Renderer, img->texture, &SrcArea, &Area);
	return true;
}
bool Graphics::DrawImage(Image* img, int x, int y, int startX, int startY, int endX, int endY, double scale=1.0) 
{
	int w = (endX-startX)*scale;
	int h = (endY-startY)*scale;
	DrawImage(img,x,y,startX,startY,endX,endY,w,h);
}

void Graphics::Flip()
{
	SDL_RenderPresent(Renderer);
	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	SDL_RenderClear(Renderer);
}