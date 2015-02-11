#ifndef MUNCHKIN_H
#define MUNCHKIN_H

#include "Project.h"

class Player{
public:
	std::vector<int> deck[3];
	unsigned int i[3];
	int level;
	bool gender;
	Player():level(1),gender(true){i[0]=0; i[1]=0; i[2]=0; deck[0].reserve(10); deck[1].reserve(10); deck[2].reserve(10);};
};

class Munchkin : public Screen
{
	struct Properties{
		int x,y, vectorNumber, playerNumber;
		Properties(int vN, int pN):vectorNumber(vN),playerNumber(pN){}
		Properties(){}
	};

	typedef std::pair<std::vector<int>::iterator, Properties> pCardMap;
	typedef std::map<std::vector<int>::iterator, Properties> mCardMap;

	struct FindCard{
		FindCard(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h){}
		bool operator()(const pCardMap &a) const;
	private:
		int x_, y_, w_, h_;
	};

private:
	Input* input;
	Graphics* graphics;
	(Image* card_map)[17];
	Image* back;
	Image* toMove;
	Image* imHand;
	Image* imEquip;
	Image* imDesk;
	Image* down;
	Image* up;
	int cp, ep, totalplayers, zoomed;

	int wW, wH, cW, cH, mapW, mapH;
	double cardRatio;

	std::vector<Player> plr;

	void ShowCard(int id, int x, int y);
	void ShowBack(int id, int x, int y);
	void ZoomCard(int id);
	void DragCard();

	void FillMap();
	void FillLine(Properties *properties, double col);
	void ShowMap();

	void GiveCard(int nd, int nt, int pl);
	void GiveToAll(int nd, int nt);

	void StartSettings();
	void LoadingImage();

	void ReDraw();

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;
	mCardMap::iterator iMapToMove;
	bool mayToMove;
	mCardMap cardMap;
	friend class MapObj;
	friend class CardObj;
public:
	void Start();
	void Update();
};


class MapObj{
protected:
	int objWidth;
	int objHeight;
	Munchkin * view;
	virtual void SetSize();
public:
	MapObj(Munchkin *v) {view=v;}
	int GetW() {return objWidth;}
	int GetH() {return objWidth;}
	virtual void OnClickL()=0;
	virtual void OnClickR()=0;
	virtual void Draw()=0;
	virtual void ResetSize(){SetSize();}
};

class CardObj : public MapObj {
	int id; 
public:
	CardObj(Munchkin *v) : MapObj(v) {SetSize();}
	void OnClickL();
	void OnClickR();
	void Draw();
};

class ButtonObj : public MapObj {
protected:
	Image *image;
	ButtonObj(Munchkin *v) : MapObj(v){SetSize();}
	double propH, propW;
	void SetSize();

public:
	void Draw();
	void OnClickR(){;}
};



#endif