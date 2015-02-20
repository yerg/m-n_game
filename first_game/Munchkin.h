#ifndef MUNCHKIN_H
#define MUNCHKIN_H

#include "Project.h"
#include "MapItem.h"
#include "Card.h"
class MapItem;
enum CardGroup {HAND,EQUIP,DESK};

class Player{
public:
	std::vector<int> deck[3];
	unsigned int i[3];
	int level;
	bool gender;
	Player():level(1),gender(true){i[HAND]=0; i[EQUIP]=0; i[DESK]=0; deck[HAND].reserve(10); deck[EQUIP].reserve(10); deck[DESK].reserve(10);};
};

class Munchkin : public Screen
{
	struct Properties{
		int x,y,  playerNumber;
		CardGroup vectorName;
		Properties(CardGroup vN, int pN):vectorName(vN),playerNumber(pN){}
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
	std::unordered_set< std::unique_ptr<MapItem> > mapOfItems;

	std::vector<Player> plr;

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;

	mCardMap::iterator iMapToMove;
	bool mayToMove;
	mCardMap cardMap;


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
	void LoadImages();

	void ReDraw();

	friend class MapItem;
	friend class ButtonItem; 
	friend class BindedToVector;
	friend class UpButton;
	friend class DownButton;
	friend class GroupButton;
	friend class CardItem;
public:
	void Start();
	void Update();
};



#endif