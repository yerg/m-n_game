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
	MapItem(Munchkin *v);
	int GetW() {return itemWidth;}
	int GetH() {return itemHeight;}
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
	ButtonItem(Munchkin *v) : MapItem(v){;}
	virtual void OnClickR() {;}

	virtual void Draw();
};

class BindedToVector : public ButtonItem {				//Abstract
protected:
	int playerNumber;
	CardGroup vectorName;

	virtual void Settings();

public:
	void SetPlayer(int p){playerNumber=p;}
	BindedToVector(Munchkin *v, CardGroup vn, int p) : ButtonItem(v), vectorName(vn), playerNumber(p){;}
};

class GroupButton : public BindedToVector{				//Hand, equip, desk
public:
	GroupButton(Munchkin *v, CardGroup vn, int p);
	virtual void OnClickL();
};

class UpButton :public BindedToVector{
public:
	UpButton(Munchkin *v, CardGroup vn, int p);
	virtual void OnClickL();
};

class DownButton :public BindedToVector{
public:
	DownButton(Munchkin *v, CardGroup vn, int p);
	virtual void OnClickL();
};


class CardItem : public MapItem {						//Card 
	int id, vectorNumber, playerNumber;

public:
	CardItem(Munchkin *v, int id) : MapItem(v), id(id) {;}
	virtual void OnClickL();
	virtual void OnClickR();
	virtual void Draw();
};

#endif