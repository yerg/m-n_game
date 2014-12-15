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


const int MAP_W=3305; 
const int MAP_H=2056;

void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10],x,y,(id%5)*MAP_W/5,(id%10)>4 ? MAP_H/2 : 0,(id%5+1)*MAP_W/5,(id%10)>4 ? MAP_H : MAP_H/2);
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
		strcat(filename,letter);
		strcat(filename,".jpg");

		card_map[i]=graphics->NewImage(filename);
	}

}
void Munchkin::SomeDraw(){

	ShowCard(15,5,5);
	ShowCard(145,600,300);
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
