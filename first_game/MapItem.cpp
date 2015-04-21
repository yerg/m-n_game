#include "MapItem.h"


MapItem::MapItem(Munchkin *v, int x, int y) : x(x), y(y){
	view=v;
	Settings();
}

void MapItem::Settings(){
	itemHeight = view->wH / 5.5;
	itemWidth = view->cardRatio * itemHeight;
}

void ButtonItem::Settings(){
	MapItem::Settings();
	itemHeight *= propH;
	itemWidth *= propW;
}

void ButtonItem::Draw(){
	(*view).graphics->DrawImage(image,x,y,0,0,image->GetWidth(),image->GetHeight(),itemWidth,itemHeight);
}

void PlayersTurnIcon::Draw(){
	(*view).graphics->DrawImage(image,x,y,0,0,image->GetWidth(),image->GetHeight(),itemWidth,itemHeight);
	(*view).graphics->DrawImage(image2, x+itemWidth, y, 0,0, image2->GetWidth(), image2->GetHeight(), itemWidth, itemHeight);
}

void DiceIcon::Draw(){
	if (image!=NULL) ButtonItem::Draw();
}

EventIcon::EventIcon(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->snapshot.event ? view->eventIcon : view->phaseIcon;
	Settings();
}

PhaseIcon::PhaseIcon(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->phaseIcons[static_cast<int>(view->snapshot.phase)];
	Settings();
}

PlayersTurnIcon::PlayersTurnIcon(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=0.5;

	image=view->level[static_cast<int>(view->snapshot.plrTurn)-FIRSTPLAYER];
	image2=view->turn;
	Settings();
}

FoldButton::FoldButton(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->fold;
	Settings();
}

SellButton::SellButton(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->sell;
	Settings();
}

Ability1Button::Ability1Button(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->ability1;
	Settings();
}

Ability2Button::Ability2Button(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;

	image=view->ability2;
	Settings();
}

void FoldButton::OnClickL(){
	if (view->selected==true){
		CardPosition tmp;
		tmp.playerNumber=FOE;
		tmp.vectorName=HAND;
		tmp.position=-1;
		(*view).model->TryMove(view->selectedCard,tmp,view->cp);
		view->selected=false;
	}
}

void SellButton::OnClickL(){				
	if (view->selected==true){
		CardPosition tmp;
		tmp.playerNumber=FOE;
		tmp.vectorName=DESK;
		tmp.position=-1;
		(*view).model->TryMove(view->selectedCard,tmp,view->cp);
		view->selected=false;
	}
}

void Ability1Button::OnClickL(){
	if (view->selected==true){
		CardPosition tmp;
		tmp.playerNumber=HELP;
		tmp.vectorName=HAND;
		tmp.position=-1;
		(*view).model->TryMove(view->selectedCard,tmp,view->cp);
		view->selected=false;
	}
}

void Ability2Button::OnClickL(){			
	if (view->selected==true){
		CardPosition tmp;
		tmp.playerNumber=HELP;
		tmp.vectorName=DESK;
		tmp.position=-1;
		(*view).model->TryMove(view->selectedCard,tmp,view->cp);
		view->selected=false;
	}
}

DiceIcon::DiceIcon(Munchkin *v, int x, int y, int pl) : ButtonItem(v,x,y), playerNumber(pl){
	propH=0.5;
	propW=0.5;

	if (((*view).snapshot.dice>0)&&((*view).snapshot.dice<=6)){
		image=view->dices[(*view).snapshot.dice-1];
	}
	Settings();
}

ReadyButton::ReadyButton(Munchkin *v, int x, int y) : ButtonItem(v,x,y){
	propH=0.5;
	propW=1;
	
	image=view->phaseClicked ? view->lockedReady : view->ready;
	Settings();
}

void ReadyButton::OnClickL(){
	(*(*view).model).EndPhase(view->snapshot.phase,view->cp);
	view->phaseClicked=true;
}


GenderButton::GenderButton(Munchkin *v, int x, int y, int pl) : ButtonItem(v,x,y), playerNumber(pl){
	if ((*view).snapshot.plr[playerNumber].gender) 
	{
		image=view->male;
	} else {
		image=view->female;
	}
	propH=0.5;
	propW=0.5;
	Settings();
}

LevelIcon::LevelIcon(Munchkin *v, int x, int y, int pl) : ButtonItem(v,x,y), playerNumber(pl){
	if (((*view).snapshot.plr[playerNumber].level>0)&&((*view).snapshot.plr[playerNumber].level<10)){
		image=view->level[(*view).snapshot.plr[playerNumber].level-1];
	}
	propH=0.5;
	propW=0.5;
	Settings();
}

UpPlayer::UpPlayer(Munchkin *v, int x, int y) : ButtonItem(v,x,y) {
	propH=0.25;
	propW=0.5;
	image=view->up;
	Settings();
}

void UpPlayer::OnClickL(){
	++view->ep;
	if (view->ep >= view->totalplayers) view->ep=FIRSTPLAYER;
	if (view->ep == view->cp) OnClickL();
}

DownPlayer::DownPlayer(Munchkin *v, int x, int y) : ButtonItem(v,x,y) {
	propH=0.25;
	propW=0.5;
	image=view->down;
	Settings();
}

void DownPlayer::OnClickL(){
	--view->ep;
	if (view->ep<FIRSTPLAYER) view->ep=view->totalplayers-1;
	if (view->ep==view->cp) OnClickL();
}

PlayerNumberIcon::PlayerNumberIcon(Munchkin *v, int x, int y, int pl) : ButtonItem(v,x,y), playerNumber(pl){
	if ((playerNumber>0)&&(playerNumber<10)){
		image=view->level[playerNumber-FIRSTPLAYER];
	}
	propH=0.5;
	propW=0.5;
	Settings();
}

void BindedToVector::Settings(){
	ButtonItem::Settings();
	if ((playerNumber>HELP)&&(playerNumber != view->cp)) {
		playerNumber = view->ep;
	}
}

GroupButton::GroupButton(Munchkin *v, int x, int y, CardGroup vn, int p) : BindedToVector(v, x, y, vn, p){
	propH=0.25; 
	propW=1;
	switch (playerNumber){
	case FOE:
		image=view->beast;
		break;
	case HELP:
		image=view->help;
		break;
	default:
		switch (vectorName) {
		case HAND:
			image=view->imHand;
			break;
		case EQUIP:
			image=view->imEquip;
			break;
		case DESK:
			image=view->imDesk;
			break;
		}
	}
	Settings();
}

UpButton::UpButton(Munchkin *v, int x, int y, CardGroup vn, int p) : BindedToVector(v, x, y, vn, p){
	propH=0.25;
	propW=0.5;
	image=view->up;
	Settings();
}

void UpButton::OnClickL(){
	view->UpCount(playerNumber,vectorName);
}

DownButton::DownButton(Munchkin *v, int x, int y, CardGroup vn, int p) : BindedToVector(v, x, y, vn, p){
	propH=0.25;
	propW=0.5;
	image=view->down;
	Settings();
}

void DownButton::OnClickL(){
	view->DownCount(playerNumber,vectorName);
}

void GroupButton::OnClickL(){
	if (view->selected==true){
		CardPosition tmp;
		tmp.playerNumber=playerNumber;
		tmp.vectorName=vectorName;
		tmp.position=-1;
		(*view).model->TryMove(view->selectedCard,tmp,view->cp);
		view->selected=false;
	}
}

void CardItem::Draw(){
	view->ShowCard(view->snapshot.plr.at(pos.playerNumber).deck[pos.vectorName].at(pos.position),x,y);
	if ((view->selected)&&(pos==view->selectedCard)) view->ShowSelect(x,y);
}

void CardItem::OnClickR(){
	view->zoomed=view->snapshot.plr.at(pos.playerNumber).deck[pos.vectorName].at(pos.position);
}

void CardItem::OnClickL(){
	view->Select(pos);
}

