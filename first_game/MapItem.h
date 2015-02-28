#ifndef MAPITEM_H
#define MAPITEM_H

#include "Munchkin.h"
class Munchkin;
enum CardGroup;

class MapItem {											//Base interface
protected:
	int itemWidth, itemHeight, x, y;
	Munchkin * view;

	virtual void Settings();

public:
	MapItem(Munchkin *v, int x, int y);
	int GetW() const{return itemWidth;}
	int GetH() const{return itemHeight;}
	int GetX() const{return x;}
	int GetY() const{return y;}
	void Reset() {Settings();}

	virtual void OnClickL()=0;
	virtual void OnClickR()=0;
	virtual void Draw()=0;
};

class ButtonItem : public MapItem {						//Abstract button
protected:
	Image *image;
	double propH, propW;

	virtual void Settings();
public:
	ButtonItem(Munchkin *v, int x, int y) : MapItem(v, x, y) {}
	virtual void OnClickR() {}

	virtual void Draw();
};

class BindedToVector : public ButtonItem {				//Abstract
protected:
	int playerNumber;
	CardGroup vectorName;

	virtual void Settings();

public:
	void SetPlayer(int p){playerNumber=p;}
	BindedToVector(Munchkin *v, int x, int y, CardGroup vn, int p) : ButtonItem(v, x, y), vectorName(vn), playerNumber(p) {}
};

class GroupButton : public BindedToVector{				//Hand, equip, desk
public:
	GroupButton(Munchkin *v, int x, int y, CardGroup vn, int p);
	virtual void OnClickL();
};

class UpButton :public BindedToVector{
public:
	UpButton(Munchkin *v, int x, int y, CardGroup vn, int p);
	virtual void OnClickL();
};

class DownButton :public BindedToVector{
public:
	DownButton(Munchkin *v, int x, int y, CardGroup vn, int p);
	virtual void OnClickL();
};


class CardItem : public MapItem {						//Card 
	CardPosition pos;
public:
	CardItem(Munchkin *v, int x, int y, CardPosition pos) : MapItem(v, x, y), pos(pos) {}
	virtual void OnClickL();
	virtual void OnClickR();
	virtual void Draw();
};

#endif