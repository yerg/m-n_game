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
	imDesk=graphics->NewImage("res\\desk.jpg");
	imEquip=graphics->NewImage("res\\equip.jpg");
	imHand=graphics->NewImage("res\\hand.jpg");
	downup=graphics->NewImage("res\\downup.jpg");
	toMove=graphics->NewImage("res\\select.jpg",255,255,255);
	mapW=card_map[0]->GetWidth();
	mapH=card_map[0]->GetHeight();
	cardRatio=static_cast<double>(mapW/5)/static_cast<double>(mapH/2);


}

void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10], x, y, (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0, mapW/5, mapH/2, cW, cH);
}

void Munchkin::ShowBack(int id, int x, int y){
	graphics->DrawImage(back, x, y, 0, id<95 ? 0 : mapH/2, mapW/5, mapH/2, cW, cH);
}

void Munchkin::ZoomCard(int id){
	graphics->DrawImage(card_map[id/10], wW-cardRatio*wH, 0, (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0, mapW/5, mapH/2, cardRatio*wH, wH);
}



void Munchkin::FillLine(std::vector<int> &v, unsigned int pos, double col, mWMap &mWMapInstance){
	std::vector<int>::iterator it;
	for(int i=0; (i<5)&&(i<v.size()); i++){
		it=v.begin()+((pos+i)%v.size());
		mWMapInstance[it]=std::make_pair(cW*col, (0.25+i)*(cH));
	}
}

void Munchkin::FillMap(){
	windowMap.clear();
	backMap.clear();
	FillLine(plr[cp].hand, plr[cp].hi, 0.1, windowMap);
	FillLine(plr[cp].equip, plr[cp].ei, 1.2, windowMap);
	FillLine(plr[cp].desk, plr[cp].di, 2.3, windowMap);

	FillLine(plr[ep].desk, plr[ep].di, 7.8, windowMap);
	FillLine(plr[ep].equip, plr[ep].ei, 8.9, windowMap);
	FillLine(plr[ep].hand, plr[ep].hi, 10.0, backMap);

}

void Munchkin::ShowMap(){
	FillMap();
	for(mWMap::iterator it=windowMap.begin(); it != windowMap.end(); ++it){
		ShowCard(*it->first, it->second.first, it->second.second);
	}
	for(mWMap::iterator it=backMap.begin(); it != backMap.end(); ++it){
		ShowBack(*it->first, it->second.first, it->second.second);
	}
	if (mayToMove) {
		iMapToMove=windowMap.find(iToMove);
		if (iMapToMove==windowMap.end()) iMapToMove=backMap.find(iToMove);
		if (iMapToMove!=backMap.end())	graphics->DrawImage(toMove, iMapToMove->second.first, iMapToMove->second.second, 0, 0, mapW/5, mapH/2, cW, cH);
	}
}

void Munchkin::GiveCard(int nd, int nt, int pl){
	while (nd){
		plr[pl].hand.push_back(doors.back());
		doors.pop_back();
		--nd;
	}
	while (nt){
		plr[pl].hand.push_back(treasures.back());
		treasures.pop_back();
		--nt;
	}
}
void Munchkin::GiveToAll(int nd, int nt){
	for (int i=0; i<totalplayers; i++) GiveCard(nd,nt,i);
}

bool Munchkin::FindCard::operator()(const pWMap &a) const{
	return (x_ >= a.second.first) && (x_ < a.second.first + w_) && (y_ >= a.second.second) && (y_ < a.second.second + h_);
}

void Munchkin::ReDraw(){
	graphics->GetWindowSize(wW, wH);
	cH=wH/5.5;
	cW=cardRatio*cH;
	ShowMap();
	if((zoomed>=0)&&(zoomed<170)) ZoomCard(zoomed);
	graphics->Flip();
}

void Munchkin::Start()
{
	StartSettings();
	LoadingImage();
	totalplayers=2;
	plr.resize(totalplayers);
	cp=0; ep=1;
	mayToMove=false;

	srand(time(NULL));

	doors.reserve(95);
	rd.reserve(95);
	for(int i=0; i<95;i++) doors.push_back(i);
	random_shuffle(doors.begin(),doors.end());

	treasures.reserve(75);
	rt.reserve(75);
	for(int i=95; i<170;i++) treasures.push_back(i);
	random_shuffle(treasures.begin(),treasures.end());
	GiveToAll(4,4);
	
	int a[]={132,12,1,66,64,153};
	plr[ep].hi=4;
	plr[ep].ei=3;
	plr[ep].di=4;
	plr[ep].hand.assign(a,a+sizeof(a)/sizeof(int));
	plr[ep].equip.assign(a,a+sizeof(a)/sizeof(int));
	plr[ep].desk.assign(a,a+4);
	ReDraw();
}

void Munchkin::Update()
{
	int x=0, y=0;
	if(input->IsExit()) {
		game->Exit();
	}
	if(input->IsMouseButtonDown(3)) {
		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;
		mWMap::iterator it = find_if(windowMap.begin(),windowMap.end(),FindCard(x,y,cW,cH));
		if (it!=windowMap.end()) zoomed = *(it->first);
	}
	if(input->IsMouseButtonDown(1)) {
		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;

		while(!(input->IsMouseButtonUp(1))){input->Update();}

		mWMap::iterator it = find_if(windowMap.begin(),windowMap.end(),FindCard(x,y,cW,cH));

		if (it==windowMap.end()) it = find_if(backMap.begin(),backMap.end(),FindCard(x,y,cW,cH));
		if (it!=backMap.end()) {
			if (iToMove!=it->first){ 
				mayToMove=true;
				iToMove=it->first;
			} else {
				mayToMove=!mayToMove;
			}
		}
	}
	ReDraw();
}
