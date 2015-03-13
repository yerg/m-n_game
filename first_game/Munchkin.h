#ifndef MUNCHKIN_H
#define MUNCHKIN_H


#include "Properties.h"
#include "Project.h"
#include "MapItem.h"
#include "Model.h"

class MapItem;
class ModelHandler;


class Munchkin : public Screen
{
	typedef std::vector< std::unique_ptr<MapItem> > vMap;

	struct FindCard{
		FindCard(int x, int y) : x_(x), y_(y){}
		bool operator()(const std::unique_ptr<MapItem> &a) const;
	private:
		int x_, y_;
	};

	class Int3{
		unsigned int _0;
		unsigned int _1;
		unsigned int _2;
	public:
		Int3():_0(0),_1(0),_2(0){}
		Int3(unsigned int a,unsigned int b,unsigned int c): _0(a), _1(b), _2(c){}
		unsigned int& operator[](const CardGroup &i){if (i==HAND) return _0; if (i==EQUIP) return _1; return _2;}
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
	Image* beast;
	Image* help;
	Image* ready;
	Image* lockedReady;
	(Image* level)[9];
	Image* male, * female;

	std::shared_ptr<ModelHandler> model;
	Snapshot snapshot;
	Phase phase;
	bool phaseClicked;
	
	std::vector<Int3> counter;
	int zoomed, cp, ep, totalplayers;
	
	CardPosition selectedCard;
	bool selected;
	
	int wW, wH, cW, cH, mapW, mapH;
	double cardRatio;

	vMap mapOfItems;
	

	void ShowSelect(int x, int y);
	void ShowCard(int id, int x, int y);
	void ZoomCard(int id);

	void FillMap();
	void FillLine(const CardGroup &vectorName, const int &playerNumber, const double &col);
	void ShowMap();
	void UpCount(const int &playerNumber, const CardGroup &cardGroup);
	void DownCount (const int &playerNumber, const CardGroup &cardGroup);
	void UpdateCounters();
	void Select(CardPosition newSelect);

	void StartSettings();
	void ReDraw();
	
	friend class MapItem;
	friend class ButtonItem; 
	friend class BindedToVector;	
	friend class ReadyButton;	
	friend class GenderButton;
	friend class LevelIcon;
	friend class UpButton;
	friend class DownButton;
	friend class GroupButton;
	friend class CardItem;
	friend class UpPlayer;
	friend class DownPlayer;
	friend class PlayerNumberIcon;
	friend class DiscardButton;
public:
	void Start();
	void Update();
	Munchkin(std::shared_ptr<ModelHandler> model, int totalplayers, int cp): model(model), totalplayers(totalplayers+FIRSTPLAYER), cp(cp+2){}

};



#endif