#include "MapItem.h"



void MapItem::SetSize(){
	itemHeight = view->wH / 5.5;
	itemWidth = view->cardRatio * itemHeight;
}

void CardItem::OnClickR(){
	view->ZoomCard(id);
}

void CardItem::OnClickL(){
	
}

void ButtonItem::SetSize(){
	MapItem::SetSize();
	itemHeight *= propH;
	itemWidth *= propW;
}