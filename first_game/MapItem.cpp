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

void BindedToVector::Settings(){
	ButtonItem::Settings();
	if (playerNumber != view->cp) {
		playerNumber = view->ep;
	}
}

GroupButton::GroupButton(Munchkin *v, int x, int y, CardGroup vn, int p) : BindedToVector(v, x, y, vn, p){
	propH=0.25; 
	propW=1;

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
		tmp.position=0;
		(*view).model->TryMove(view->selectedCard,tmp);
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
