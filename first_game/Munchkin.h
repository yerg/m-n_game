#ifndef MUNCHKIN_H
#define MUNCHKIN_H
#include "CardGroup.h"
#include "Project.h"
#include "MapItem.h"
#include "Model.h"
class MapItem;
class Model;


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
	typedef std::vector< std::unique_ptr<MapItem> > vMap;

	struct FindCard{
		FindCard(int x, int y) : x_(x), y_(y){}
		bool operator()(const std::unique_ptr<MapItem> &a) const;
	private:
		int x_, y_;
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

	std::unique_ptr<Model> model;

	int cp, ep, totalplayers, zoomed;
	int wW, wH, cW, cH, mapW, mapH;
	double cardRatio;
	
	vMap mapOfItems;
	
	std::vector<Player> plr;

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;

	CardPosition selectedCard;
	bool selected;

	void ShowCard(int id, int x, int y);
	void ShowBack(int id, int x, int y);
	void ZoomCard(int id);

	void FillMap();
	void FillLine(const CardGroup &vectorName, const int &playerNumber, const double &col);
	void ShowMap();

	void Select(CardPosition newSelect);
	void GiveCard(int nd, int nt, int pl);
	void GiveToAll(int nd, int nt);

	void StartSettings();
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