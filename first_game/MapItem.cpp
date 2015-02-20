#include "MapItem.h"


MapItem::MapItem(Munchkin *v){
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
	/////////////////////////
}

void BindedToVector::Settings(){
	ButtonItem::Settings();
	if (playerNumber != view->cp) {
		playerNumber = view->ep;
	}
}

GroupButton::GroupButton(Munchkin *v, CardGroup vn, int p) : BindedToVector(v,vn,p){
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
}

UpButton::UpButton(Munchkin *v, CardGroup vn, int p) : BindedToVector(v,vn,p){
	propH=0.25;
	propW=0.5;
	image=view->up;
}

void UpButton::OnClickL(){
	--view->plr[playerNumber].i[vectorName];
}

DownButton::DownButton(Munchkin *v, CardGroup vn, int p) : BindedToVector(v,vn,p){
	propH=0.25;
	propW=0.5;
	image=view->down;
}

void DownButton::OnClickL(){
	++view->plr[playerNumber].i[vectorName];
}

void GroupButton::OnClickL(){
	////////////////////
}

void CardItem::OnClickR(){
	view->ZoomCard(id);
}

void CardItem::OnClickL(){
	///////////////////////
}
