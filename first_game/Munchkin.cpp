#include "Munchkin.h"


bool Munchkin::FindCard::operator()(const std::unique_ptr<MapItem> &a) const{
	return (x_ >= a->GetX()) && (x_ < a->GetX() + a->GetW()) && (y_ >= a->GetY()) && (y_ < a->GetY() + a->GetH());
}

void Munchkin::StartSettings()
{
	input = game->GetInput();
	graphics = game->GetGraphics();
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
	down=graphics->NewImage("res\\down.jpg");
	up=graphics->NewImage("res\\up.jpg");
	toMove=graphics->NewImage("res\\select.jpg",255,255,255);
	mapW=card_map[0]->GetWidth();
	mapH=card_map[0]->GetHeight();
	cardRatio=static_cast<double>(mapW/5)/static_cast<double>(mapH/2);
}


void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10],  x,               y,  (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0,          mapW/5,  mapH/2, cW,           cH);
}
void Munchkin::ShowBack(int id, int x, int y){
	graphics->DrawImage(back,             x,               y,  0,             id<95     ? 0      :    mapH/2,  mapW/5,  mapH/2, cW,           cH);
}
void Munchkin::ZoomCard(int id){
	graphics->DrawImage(card_map[id/10],  wW-cardRatio*wH, 0,  (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0,          mapW/5,  mapH/2, cardRatio*wH, wH);
}



void Munchkin::FillLine(Properties *pr, double col){ 
	std::vector<int>::iterator it;
	for(int i=0; (i<5)&&(i<plr[pr->playerNumber].deck[pr->vectorName].size()); i++){
		it=plr[pr->playerNumber].deck[pr->vectorName].begin()+((plr[pr->playerNumber].i[pr->vectorName]+i)%plr[pr->playerNumber].deck[pr->vectorName].size());
		mapOfItems.push_back(std::unique_ptr<MapItem> (new CardItem (this, cW*col,       (0.25+i)*cH, *it)));
	}
	mapOfItems.push_back(std::unique_ptr<MapItem> (new GroupButton  (this, cW*col,       0,           pr->vectorName,pr->playerNumber)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new UpButton     (this, cW*col,       5.25*cH,     pr->vectorName,pr->playerNumber)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new DownButton   (this, cW*(col+0.5), 5.25*cH,     pr->vectorName,pr->playerNumber)));
	delete pr;
}
void Munchkin::FillMap(){
	cardMap.clear();
	mapOfItems.clear();
	FillLine(new Properties(HAND, cp), 0.1);
	FillLine(new Properties(EQUIP,cp), 1.2);
	FillLine(new Properties(DESK, cp), 2.3);
	FillLine(new Properties(DESK, ep), 7.8);
	FillLine(new Properties(EQUIP,ep), 8.9);
	FillLine(new Properties(HAND, ep), 10.0);



}

void Munchkin::ShowMap(){
	FillMap();
	for(mCardMap::iterator it=cardMap.begin(); it != cardMap.end(); ++it){
		if ( (it->second.playerNumber==ep) && (!(it->second.vectorName)) ) {
			ShowBack(*it->first, it->second.x, it->second.y);
		} else {
			ShowCard(*it->first, it->second.x, it->second.y);
		}
	}
	if (mayToMove) {
		if (iMapToMove!=cardMap.end())	graphics->DrawImage(toMove, iMapToMove->second.x, iMapToMove->second.y, 0, 0, mapW/5, mapH/2, cW, cH);
	}
	for (std::vector<std::unique_ptr<MapItem> >::iterator it=mapOfItems.begin(); it!=mapOfItems.end(); ++it) (*it)->Draw();
}

void Munchkin::GiveCard(int nd, int nt, int pl){
	while (nd){
		plr[pl].deck[0].push_back(doors.back());
		doors.pop_back();
		--nd;
	}
	while (nt){
		plr[pl].deck[0].push_back(treasures.back());
		treasures.pop_back();
		--nt;
	}
}
void Munchkin::GiveToAll(int nd, int nt){
	for (int i=0; i<totalplayers; i++) GiveCard(nd,nt,i);
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
	plr[ep].i[0]=4;
	plr[ep].i[1]=3;
	plr[ep].i[2]=4;
	plr[ep].deck[0].assign(a,a+sizeof(a)/sizeof(int));
	plr[ep].deck[1].assign(a,a+sizeof(a)/sizeof(int));
	plr[ep].deck[2].assign(a,a+4);
	plr[cp].deck[1].assign(a,a+1);
	plr[cp].deck[2].assign(a,a+1);
	ReDraw();
}

void Munchkin::Update()
{
	int x=0, y=0;
	if(input->IsExit()) {
		game->Exit();
	}

	//Zoom
	if(input->IsMouseButtonDown(3)) { 

		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;

		vMap::iterator it = find_if(mapOfItems.begin(), mapOfItems.end(),FindCard(x,y));
		if (it!=mapOfItems.end()) (*it)->OnClickR();

	} 

	//Left-click
	if(input->IsMouseButtonDown(1)) {
		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;

		while(!(input->IsMouseButtonUp(1))){input->Update();} //Freeze until button up prevents recur of next chunk

/*		mCardMap::iterator it = find_if(cardMap.begin(),cardMap.end(),FindCard(x,y,cW,cH));
		//		if (it==windowMap.end()) it = find_if(backMap.begin(),backMap.end(),FindCard(x,y,cW,cH));

		if (it!=cardMap.end()) {
			if (iMapToMove==it){ 
				mayToMove=!mayToMove;
			} 
			else {
				if (mayToMove) {
					mayToMove=false;
					if ((iMapToMove->second.playerNumber!=it->second.playerNumber)||(iMapToMove->second.vectorName!=it->second.vectorName)){
						plr[it->second.playerNumber].deck[it->second.vectorName].insert(it->first, *iMapToMove->first);
						plr[iMapToMove->second.playerNumber].deck[iMapToMove->second.vectorName].erase(iMapToMove->first);
					}else{
						std::swap(*it->first,*iMapToMove->first);
					}

				} 
				else {
					mayToMove=true;
					iMapToMove=it;
				}
			}
		}*/
	}
	ReDraw();
}

