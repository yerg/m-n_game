#include "Munchkin.h"


bool Munchkin::FindCard::operator()(const std::unique_ptr<MapItem> &a) const{
	return (x_ >= a->GetX()) && (x_ < a->GetX() + a->GetW()) && (y_ >= a->GetY()) && (y_ < a->GetY() + a->GetH());
}

void Munchkin::StartSettings()
{
	model->Attach(this);
#pragma warning (push) 
#pragma warning (disable:4996)
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
	imDesk=graphics->NewImage("res\\desk.bmp");
	imEquip=graphics->NewImage("res\\equip.bmp");
	imHand=graphics->NewImage("res\\hand.bmp");
	down=graphics->NewImage("res\\down.bmp");
	up=graphics->NewImage("res\\up.bmp");
	toMove=graphics->NewImage("res\\select.jpg",255,255,255);
	beast=graphics->NewImage("res\\beast.bmp");
	help=graphics->NewImage("res\\help.bmp");
	ready=graphics->NewImage("res\\ready.bmp");
	lockedReady=graphics->NewImage("res\\ready_grey.bmp");
	male=graphics->NewImage("res\\male.bmp");
	female=graphics->NewImage("res\\female.bmp");
	phaseIcon=graphics->NewImage("res\\phase.bmp");
	eventIcon=graphics->NewImage("res\\event.bmp");
	fold=graphics->NewImage("res\\fold.bmp");
	sell=graphics->NewImage("res\\sell.bmp");
	turn=graphics->NewImage("res\\turn.bmp");
	ability1=graphics->NewImage("res\\ability1.bmp");
	ability2=graphics->NewImage("res\\ability2.bmp");
	for (int i=0; i<8; i++)
	{
		char filename[20]; char letter[2]={'1'+i,'\0'};
		strcpy(filename,"res\\");
		strcat(filename, letter);
		strcat(filename, ".bmp");
		level[i]=graphics->NewImage(filename);
	}
	for (int i=0; i<9; i++)
	{
		char filename[20]; char letter[2]={'0'+i,'\0'};
		strcpy(filename,"res\\phase");
		strcat(filename, letter);
		strcat(filename, ".bmp");
		phaseIcons[i]=graphics->NewImage(filename);
	}
	for (int i=0; i<6; i++)
	{
		char filename[20]; char letter[2]={'1'+i,'\0'};
		strcpy(filename,"res\\dice");
		strcat(filename, letter);
		strcat(filename, ".bmp");
		dices[i]=graphics->NewImage(filename);
	}
	mapW=card_map[0]->GetWidth();
	mapH=card_map[0]->GetHeight();
	cardRatio=static_cast<double>(mapW/5)/static_cast<double>(mapH/2);
#pragma warning (pop)
}

void Munchkin::ShowSelect(int x, int y){
	graphics->DrawImage    (toMove,           x,               y, 0,             0,                               mapW/5, mapH/2,  cW,           cH);
}

void Munchkin::ShowCard(int id, int x, int y){
	if (id<0) {
		graphics->DrawImage(back,             x,               y, 0,             id==-1    ? 0      : mapH/2,     mapW/5,  mapH/2, cW,           cH);
	} else {
		graphics->DrawImage(card_map[id/10],  x,               y, (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0,          mapW/5,  mapH/2, cW,           cH);
	}
}

void Munchkin::ZoomCard(int id){
	if (id>=0){
		graphics->DrawImage(card_map[id/10],  11.1*cW, 0, (id%5)*mapW/5, (id%10)>4 ? mapH/2 : 0,          mapW/5,  mapH/2, cardRatio*wH, wH);
	} //wW-cardRatio*wH   as alternate argument 'x' (for allign to right border)
}

void Munchkin::FillLine(const CardGroup &vectorName, const int &playerNumber, const double &col){ 
	CardPosition card; 
	card.vectorName=vectorName;
	card.playerNumber=playerNumber;
	int size=snapshot.plr[playerNumber].deck[vectorName].size();
	for(int i=0; (i<5)&&(i<size); i++){
		card.position=(counter[playerNumber][vectorName]+i)%size;
		mapOfItems.push_back(std::unique_ptr<MapItem> (new CardItem     (this, cW*col,       (0.25+i)*cH, card)));
	}
	mapOfItems.push_back(std::unique_ptr<MapItem> (new GroupButton      (this, cW*col,       0,           vectorName, playerNumber)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new UpButton         (this, cW*col,       5.25*cH,     vectorName, playerNumber)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new DownButton       (this, cW*(col+0.5), 5.25*cH,     vectorName, playerNumber)));
}

void Munchkin::FillMap(){
	mapOfItems.clear();
	FillLine(HAND, cp, 0.1);
	FillLine(EQUIP,cp, 1.2);
	FillLine(DESK, cp, 2.3);
	mapOfItems.push_back(std::unique_ptr<MapItem> (new LevelIcon        (this, cW*3.3, 0.25*cH, cp)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new GenderButton     (this, cW*3.3, 0.75*cH, cp)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new PlayerNumberIcon (this, cW*3.3, 4.5 *cH, cp)));
	if(((snapshot.phase==ESCAPE)||(snapshot.phase==POSTESCAPE))&&(snapshot.inCombat[snapshot.currentInCombat]==cp)) {
		mapOfItems.push_back(std::unique_ptr<MapItem> (new PlayerNumberIcon (this, cW*3.3, 1.5 *cH, cp)));
	}
	FillLine(EQUIP, FOE, 3.9);
	FillLine(EQUIP, HELP, 5.0);
	mapOfItems.push_back(std::unique_ptr<MapItem> (new ReadyButton      (this, cW*6.1, 0.25*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new EventIcon        (this, cW*6.1, 0.75*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new PhaseIcon        (this, cW*6.1, 1.25*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new PlayersTurnIcon  (this, cW*6.1, 1.75*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new FoldButton       (this, cW*6.1, 2.75*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new SellButton       (this, cW*6.1, 3.25*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new Ability1Button   (this, cW*6.1, 3.75*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new Ability2Button   (this, cW*6.1, 4.25*cH)));

	mapOfItems.push_back(std::unique_ptr<MapItem> (new LevelIcon        (this, cW*7.3, 0.25*cH, ep)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new GenderButton     (this, cW*7.3, 0.75*cH, ep)));
	if(((snapshot.phase==ESCAPE)||(snapshot.phase==POSTESCAPE))&&(snapshot.inCombat[snapshot.currentInCombat]==ep)) {
		mapOfItems.push_back(std::unique_ptr<MapItem> (new PlayerNumberIcon (this, cW*7.3, 1.5 *cH, ep)));
	}
	mapOfItems.push_back(std::unique_ptr<MapItem> (new UpPlayer         (this, cW*7.3, 4.25*cH)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new PlayerNumberIcon (this, cW*7.3, 4.5 *cH, ep)));
	mapOfItems.push_back(std::unique_ptr<MapItem> (new DownPlayer       (this, cW*7.3, 5   *cH)));
	FillLine(DESK, ep, 7.8);
	FillLine(EQUIP,ep, 8.9);
	FillLine(HAND, ep, 10.0);
}

void Munchkin::ShowMap(){
	FillMap();
	
	for (std::vector<std::unique_ptr<MapItem> >::iterator it=mapOfItems.begin(); it!=mapOfItems.end(); ++it) (*it)->Draw();

}

void Munchkin::Select(CardPosition newSelect){
	if (selectedCard==newSelect) {
		
		selected=!selected;
	
	} else {

		if (selected){
			selected=false;
			model->TryMove(selectedCard, newSelect, cp);
  		} 

		else {
			selected=true;
			selectedCard=newSelect;
		}

	}
}

void Munchkin::DownCount(const int &playerNumber, const CardGroup &cardGroup){
	int size=snapshot.plr.at(playerNumber).deck[cardGroup].size();
	if (size<2) {
		counter.at(playerNumber)[cardGroup]=0;
		return;
	}
	if (counter.at(playerNumber)[cardGroup]+1>=size){
		counter.at(playerNumber)[cardGroup]=0;
	} else {
		++counter.at(playerNumber)[cardGroup];
	}
}

void Munchkin::UpCount(const int &playerNumber, const CardGroup &cardGroup){
	int size=snapshot.plr.at(playerNumber).deck[cardGroup].size();
	if (size<2) {
		counter.at(playerNumber)[cardGroup]=0;
		return;
	}
	if (counter.at(playerNumber)[cardGroup]==0) {
		counter.at(playerNumber)[cardGroup]=size-1;
	} else if (counter.at(playerNumber)[cardGroup]>=size){
		counter.at(playerNumber)[cardGroup]=size-2;
	} else {
		--counter.at(playerNumber)[cardGroup];
	}
}

void Munchkin::UpdateCounters(){
	for (int i=0; i<totalplayers; i++){
		for (int j=0; j<3; j++){
			if (snapshot.plr.at(i).deck[j].size()<2) {
				counter.at(i)[CardGroup(j)]=0;
			} else if (counter.at(i)[CardGroup(j)]>=snapshot.plr.at(i).deck[j].size()) {
				counter.at(i)[CardGroup(j)]=snapshot.plr.at(i).deck[j].size()-1;
			}
		}
	}
}

void Munchkin::ReDraw(){
	graphics->GetWindowSize(wW, wH);
	cH=wH/5.5;
	cW=cardRatio*cH;
	ShowMap();
	if((zoomed>=0)&&(zoomed<170)) ZoomCard(zoomed);
	graphics->Flip();
}
void Munchkin::RefreshData(){
	snapshot=model->GetData(cp);
	if (snapshot.phase!=phase) phaseClicked=false;
	phase=snapshot.phase;
	refresh=false;
}

void Munchkin::Start()
{
	StartSettings();
	counter.resize(totalplayers);
	
	Int3 a(0,0,0);
	std::fill(counter.begin(),counter.end(),a);
	selected=false;
	if (cp==FIRSTPLAYER) {
		ep=FIRSTPLAYER+1;
	} else {
		ep=FIRSTPLAYER;
	}
	phaseClicked=false;
	RefreshData();
	ReDraw();
}

void Munchkin::UpdateView(){
	refresh=true;
}

void Munchkin::Update()
{
	int x=0, y=0;
	if(input->IsExit()) {
		game->Exit();
	}

	
	if(input->IsMouseButtonDown(3)) {							//Zoom

		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;

		vMap::iterator it = find_if(mapOfItems.begin(), mapOfItems.end(),FindCard(x,y));
		if (it!=mapOfItems.end()) (*it)->OnClickR();

	} 

	
	if(input->IsMouseButtonDown(1)) {							//Left-click

		while(!(input->IsMouseButtonUp(1))){input->Update();}														//Freeze until button up prevents recur of next chunk
		
		x=input->GetButtonDownCoords().x;
		y=input->GetButtonDownCoords().y;

		vMap::iterator it = find_if(mapOfItems.begin(), mapOfItems.end(),FindCard(x,y));
		if (it!=mapOfItems.end()) (*it)->OnClickL();
	}
	if (refresh) RefreshData();
	ReDraw();
}

