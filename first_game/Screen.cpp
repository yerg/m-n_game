#include "Screen.h"

void Screen::SetController(Game* game)
{
	this->game = game;
}

void Screen::Start()
{

}

void Screen::Update()
{

}

void Screen::Destroy()
{

}


const int MAP_W=1550; 
const int MAP_H=964;
const double PROPORTION=static_cast<double>(MAP_W/5)/static_cast<double>(MAP_H/2);

void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10],x,y,(id%5)*MAP_W/5,(id%10)>4 ? MAP_H/2 : 0,(id%5+1)*MAP_W/5,(id%10)>4 ? MAP_H : MAP_H/2,PROPORTION*static_cast<double>(wH/5.5),wH/5.5);
}
void Munchkin::ZoomCard(int id){
	graphics->DrawImage(card_map[id/10],wW-PROPORTION*wH,0,(id%5)*MAP_W/5,(id%10)>4 ? MAP_H/2 : 0,(id%5+1)*MAP_W/5,(id%10)>4 ? MAP_H : MAP_H/2,PROPORTION*wH,wH);
}

void Munchkin::ShowCurPlayer(){
	for(int i=0; (i<5)&&(i<plr[cp].hand.size()); i++){
		int at=(plr[cp].hi+i)%plr[cp].hand.size();
		ShowCard(plr[cp].hand.at(at), PROPORTION*static_cast<double>(wH/5.5)*0.1, (0.5+i)*(wH/5.5));
	}
}

void Munchkin::StartSettings()
{
	input = game->GetInput();
	graphics = game->GetGraphics();
	
}

void Munchkin::LoadingImage()
{
	for (int i=0; i<17; i++)
	{
		char filename[20]; char letter[2]={'a'+i,'\0'};
		strcpy(filename,"res\\");
		strcat(filename, letter);
		strcat(filename, ".jpg");

		card_map[i]=graphics->NewImage(filename);
	}

}
void Munchkin::SomeDraw(){
	SDL_GetWindowSize(graphics->mainWindow, &wW, &wH);
	cp=0;
	int a[]={64,132,12,1,66,64,153};
	plr[cp].hand.assign(a,a+sizeof(a)/sizeof(int));
	ShowCurPlayer();

	ZoomCard(59);
	graphics->Flip();
}

void Munchkin::Start()
{
	StartSettings();
	LoadingImage();
	SomeDraw();
}
void Munchkin::Update()
{
	if(input->IsExit())
		game->Exit();
}
