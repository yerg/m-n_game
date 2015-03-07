#include "Graphics.h"
Graphics::Graphics(int width, int height, const char *title)
{
	mainWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,	width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(mainWindow, -1, 0);
}

Graphics::~Graphics(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(mainWindow);
}

Image* Graphics::NewImage(char* file)
{
	SDL_Surface* tmp = IMG_Load(file);
	Image* image = new Image();
	image->texture = SDL_CreateTextureFromSurface(renderer, tmp);
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
	image->texture = SDL_CreateTextureFromSurface(renderer, tmp);
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
	SDL_RenderCopy(renderer, img->texture, NULL, &Area);
	return true;
}

bool Graphics::DrawImage(Image* img, int x, int y, int startX, int startY, int srcW, int srcH, int w, int h)
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
	SrcArea.w = srcW;
	SrcArea.h = srcH;

	SDL_RenderCopy(renderer, img->texture, &SrcArea, &Area);
	return true;
}
bool Graphics::DrawImage(Image* img, int x, int y, int startX, int startY, int srcW, int srcH, double scale) 
{
	int w = static_cast<double>(srcW)*scale;
	int h = static_cast<double>(srcH)*scale;
	return DrawImage(img,x,y,startX,startY,srcW,srcH,w,h);
}

void Graphics::Flip()
{
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}