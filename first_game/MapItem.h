#ifndef MAPITEM_H
#define MAPITEM_H

#include "Munchkin.h"
class Munchkin;
class MapItem{
protected:
	int itemWidth, itemHeight, x, y;
	Munchkin * view;
	virtual void SetSize()=0;
public:
	MapItem(Munchkin *v) {view=v;}

	int GetW() {return itemWidth;}
	int GetH() {return itemWidth;}

	virtual void ResetSize(){SetSize();}

	virtual void OnClickL()=0;
	virtual void OnClickR()=0;
	virtual void Draw()=0; 

};

class CardItem : public MapItem {
	int id, vectorNumber, playerNumber;
	void SetSize() {MapItem::SetSize();}
public:
	CardItem(Munchkin *v, int id) : MapItem(v), id(id) {SetSize();}
	void OnClickL();
	void OnClickR();
	void Draw();
};

class ButtonItem : public MapItem {
protected:
	Image *image;
	ButtonItem(Munchkin *v) : MapItem(v){SetSize();}
	double propH, propW;
	void SetSize();

public:
	void OnClickR(){;}
	void Draw();
};

class HandItem :public ButtonItem {

};



#endif