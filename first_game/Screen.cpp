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
	graphics->DrawImage(card_map[id/10],   x,   y,   (id%5)*MAP_W/5,  (id%10)>4 ? MAP_H/2 : 0, MAP_W/5, MAP_H/2, PROPORTION*static_cast<double>(wH/5.5), wH/5.5);
}
void Munchkin::ZoomCard(int id){
	graphics->DrawImage(card_map[id/10], wW-PROPORTION*wH, 0, (id%5)*MAP_W/5, (id%10)>4 ? MAP_H/2 : 0, MAP_W/5, MAP_H/2, PROPORTION*wH, wH);
}

void Munchkin::ShowLine(std::vector<int> &v, unsigned int pos, double col){
	for(int i=0; (i<5)&&(i<v.size()); i++){
		ShowCard(v.at((pos+i)%v.size()), PROPORTION*static_cast<double>(wH/5.5)*col, (0.25+i)*(wH/5.5));
	}
}

void Munchkin::ShowBackLine(std::vector<int> &v, unsigned int pos, double col){
	for(int i=0; (i<5)&&(i<v.size()); i++){
		graphics->DrawImage(back, PROPORTION*static_cast<double>(wH/5.5)*col, (0.25+i)*(wH/5.5), 0, v.at((pos+i)%v.size())<95 ? 0 : MAP_H/2, MAP_W/5, MAP_H/2, PROPORTION*static_cast<double>(wH/5.5), wH/5.5);
	}
}

void Munchkin::ShowPlayer(){
	ShowLine(plr[cp].hand, plr[cp].hi, 0.1);
	ShowLine(plr[cp].equip, plr[cp].ei, 1.2);
	ShowLine(plr[cp].desk, plr[cp].di, 2.3);

	ShowLine(plr[cp].desk, plr[cp].di, 7.8);
	ShowLine(plr[cp].equip, plr[cp].ei, 8.9);
	ShowBackLine(plr[cp].hand, plr[cp].hi, 10.0);
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
	back=graphics->NewImage("res\\back.jpg");

}
void Munchkin::SomeDraw(){
	SDL_GetWindowSize(graphics->mainWindow, &wW, &wH);
	cp=0; ep=1;
	int a[]={132,12,1,66,64,153};
	plr[cp].hi=4;
	plr[cp].ei=3;
	plr[cp].di=4;
	plr[cp].hand.assign(a,a+sizeof(a)/sizeof(int));
	plr[cp].equip.assign(a,a+sizeof(a)/sizeof(int));
	plr[cp].desk.assign(a,a+4);
	ShowPlayer();
	ZoomCard(132);
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
