#include "Project.h"
const int WINDOWH = 482;


int MunchkinWindow (void* a);

struct Arguments{
	int totalplayers, cp;
	std::shared_ptr<ModelHandler> model;
};

void InitVideo() {
	int retval = SDL_Init(SDL_INIT_EVERYTHING);
	if (retval) std::cout<<std::endl<<SDL_GetError();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}

int main(int argc, char* argv[]) {
	Arguments arg, arg1;
	arg.totalplayers=2;
	arg.model=std::make_shared<ModelHandler>(arg.totalplayers);
	arg.cp=1;

	SDL_Thread* Player2 = SDL_CreateThread(MunchkinWindow,"Player2",&arg);
	
	arg1=arg;
	arg1.cp=0;
	SDL_Delay(5000);
	MunchkinWindow(&arg1);

	SDL_WaitThread(Player2,NULL);
	std::cout<<IMG_GetError()<<std::endl<<SDL_GetError();
	SDL_Quit();
	return 0;
}

int MunchkinWindow (void* a){
	InitVideo();
	Game game;
	return game.Execute(new Munchkin((*(Arguments*)a).model,(*(Arguments*)a).totalplayers,(*(Arguments*)a).cp),WINDOWH*2,WINDOWH,"Munchkin");
}