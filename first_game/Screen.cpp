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
	mapW=card_map[0]->GetWidth();
	mapH=card_map[0]->GetHeight();
	cardRatio=static_cast<double>(mapW/5)/static_cast<double>(mapH/2);


}

void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10],   x,   y,   (id%5)*mapW/5,  (id%10)>4 ? mapH/2 : 0, mapW/5, mapH/2, cardRatio*static_cast<double>(wH/5.5), wH/5.5);
}


void Munchkin::ZoomCard(int id){
	graphics->DrawImage(card_map[id/10], wW-cardRatio*wH, 0, (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0, mapW/5, mapH/2, cardRatio*wH, wH);
}

void Munchkin::ShowBackLine(std::vector<int> &v, unsigned int pos, double col){
	for(int i=0; (i<5)&&(i<v.size()); i++){
		graphics->DrawImage(back, cardRatio*static_cast<double>(wH/5.5)*col, (0.25+i)*(wH/5.5), 0, v.at((pos+i)%v.size())<95 ? 0 : mapH/2, mapW/5, mapH/2, cardRatio*static_cast<double>(wH/5.5), wH/5.5);
	}
}

void Munchkin::FillLine(std::vector<int> &v, unsigned int pos, double col){
	std::vector<int>::iterator it;
	for(int i=0; (i<5)&&(i<v.size()); i++){
		it=v.begin()+((pos+i)%v.size());
		windowMap[it]=std::make_pair(cardRatio*static_cast<double>(wH/5.5)*col, (0.25+i)*(wH/5.5));
	}
}

void Munchkin::FillMap(){
	windowMap.clear();

	FillLine(plr[cp].hand, plr[cp].hi, 0.1);
	FillLine(plr[cp].equip, plr[cp].ei, 1.2);
	FillLine(plr[cp].desk, plr[cp].di, 2.3);

	FillLine(plr[cp].desk, plr[cp].di, 7.8);
	FillLine(plr[cp].equip, plr[cp].ei, 8.9);
	FillLine(plr[cp].hand, plr[cp].hi, 10.0);

}

void Munchkin::ShowMap(){
	FillMap();
	for(std::map<std::vector<int>::iterator, std::pair<int,int>>::iterator it=windowMap.begin(); it != windowMap.end(); ++it){
		ShowCard(*it->first, it->second.first, it->second.second);
	}
}

bool Munchkin::FindCard::operator()(const std::pair<std::vector<int>::iterator, std::pair<int,int>>&a) const{
	return (x_ >= a.second.first)&& (x_ < a.second.first + w_) && (y_ >= a.second.second) && (y_ < a.second.second + h_);
}

void Munchkin::ReDraw(){
	SDL_GetWindowSize(graphics->mainWindow, &wW, &wH);
	ShowMap();

	if((zoomed>=0)&&(zoomed<170)) ZoomCard(zoomed);
	graphics->Flip();
}

void Munchkin::Start()
{
	StartSettings();
	LoadingImage();
	srand(time(NULL));
	doors.reserve(95);
	for(int i=0; i<95;i++) doors.push_back(i);
	treasures.reserve(75);
	for(int i=95; i<170;i++) doors.push_back(i);


	cp=0; ep=1;
	int a[]={132,12,1,66,64,153};
	plr[cp].hi=4;
	plr[cp].ei=3;
	plr[cp].di=4;
	plr[cp].hand.assign(a,a+sizeof(a)/sizeof(int));
	plr[cp].equip.assign(a,a+sizeof(a)/sizeof(int));
	plr[cp].desk.assign(a,a+4);


	ReDraw();
}

void Munchkin::Update()
{
	int x,y;
	if(input->IsExit()) game->Exit();
	if(input->IsMouseButtonDown(3)) {
		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;
		std::map<std::vector<int>::iterator, std::pair<int,int>>::iterator it = find_if(windowMap.begin(),windowMap.end(),FindCard(x,y,cardRatio*static_cast<double>(wH/5.5),(wH/5.5)));
		if (it!=windowMap.end()) zoomed=*(it->first);
	}
//	if(input->IsMouseButtonDown(1)) ;
	ReDraw();
}
