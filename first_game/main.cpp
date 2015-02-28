#include "Project.h"
const int WINDOWH = 482;

int main(int argc, char* argv[]) {
	Game game;

	return game.Execute(new Munchkin(),WINDOWH*2,WINDOWH,"Munchkin");
	std::cout<<IMG_GetError()<<std::endl<<SDL_GetError();
}

