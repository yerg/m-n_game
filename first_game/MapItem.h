#ifndef MAPITEM_H
#define MAPITEM_H

#include "Munchkin.h"
class Munchkin;
enum CardGroup;

class MapItem {											//Base interface
protected:
	int itemWidth, itemHeight, x, y;
	Munchkin * view;
	MapItem(Munchkin *v, int x, int y);
	virtual void Settings();

public:

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
	ButtonItem(Munchkin *v, int x, int y) : MapItem(v, x, y) {}
	virtual void Settings();
public:
	virtual void OnClickR() {}
	virtual void Draw();
};

class ReadyButton : public ButtonItem{
public:
	ReadyButton(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class GenderButton : public ButtonItem{
	int playerNumber;
public:
	GenderButton(Munchkin *v, int x, int y, int pl);
	virtual void OnClickL(){}
};

class LevelIcon : public ButtonItem{
	int playerNumber;
public:
	LevelIcon(Munchkin *v, int x, int y, int pl);
	virtual void OnClickL(){}
};

class PhaseIcon : public ButtonItem{
public:
	PhaseIcon(Munchkin *v, int x, int y);
	virtual void OnClickL(){}
};

class EventIcon : public ButtonItem{
public:
	EventIcon(Munchkin *v, int x, int y);
	virtual void OnClickL(){}
};
class PlayersTurnIcon : public ButtonItem{
	Image* image2;
public:
	PlayersTurnIcon(Munchkin *v, int x, int y);
	virtual void OnClickL(){}
	virtual void Draw();
};

class FoldButton : public ButtonItem {				
public:
	FoldButton(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class SellButton : public ButtonItem {				
public:
	SellButton(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class Ability1Button : public ButtonItem {				
public:
	Ability1Button(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class Ability2Button : public ButtonItem {				
public:
	Ability2Button(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class DiceIcon : public ButtonItem {
	int playerNumber;
public:
	DiceIcon(Munchkin *v, int x, int y,int pl);
	virtual void OnClickL(){}
	virtual void Draw();
};

class UpPlayer : public ButtonItem {
public:
	UpPlayer(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class DownPlayer : public ButtonItem {
public:
	DownPlayer(Munchkin *v, int x, int y);
	virtual void OnClickL();
};

class PlayerNumberIcon : public ButtonItem {
	int playerNumber;
public:
	PlayerNumberIcon(Munchkin *v, int x, int y, int pl);
	virtual void OnClickL(){}
};

class BindedToVector : public ButtonItem {				//Abstract
protected:
	int playerNumber;
	CardGroup vectorName;
	BindedToVector(Munchkin *v, int x, int y, CardGroup vn, int p) : ButtonItem(v, x, y), vectorName(vn), playerNumber(p) {}
	virtual void Settings();
public:

	void SetPlayer(int p){playerNumber=p;}
};

class GroupButton : public BindedToVector{				//Hand, equip, desk
public:
	GroupButton(Munchkin *v, int x, int y, CardGroup vn, int p);
	virtual void OnClickL();
};

class UpButton : public BindedToVector{
public:
	UpButton(Munchkin *v, int x, int y, CardGroup vn, int p);
	virtual void OnClickL();
};

class DownButton : public BindedToVector{
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