#ifndef CARDS_H
#define CARDS_H

#include "Properties.h"
#include "Project.h"

class Card;

struct StrategyDefeat {
	virtual void Handle(ModelData*d)=0;
};

class Beast{
	int level, gainCard, gainLevel;
	bool undead;
	StrategyDefeat* sd;
public:
	Beast(StrategyDefeat* sd, int level, int gainCard, int gainLevel=1, bool undead=false) : sd(sd), level(level), gainCard(gainCard), gainLevel(gainLevel), undead(undead){}
	void Defeat(ModelData* d) {sd->Handle(d);}
	bool Undead()const{return undead;}
	int GainCard()const{return gainCard;}
	int GainLevel()const{return gainLevel;}
	int Level()const{return level;}
};

struct StrategyAct {
	virtual void Handle(ModelData*d, std::map<int, Card>* const map, int pl)=0;
};
struct StrategyEscape {
	virtual bool Handle(ModelData*d, std::map<int, Card>* const map){
		int i=rand()%6;
		return i>3;
	}
};
struct StrategyCombat {
	virtual int Handle(ModelData*d, std::map<int, Card>* const map){
		return true;
	}
};
struct StrategyWin {
	virtual void Handle(ModelData*d, int gainCard, int gainLevel);
};

class Item {
	int gold;
	Forbid forbid;
	Slot slot;
public:
	Item(int gold, Slot slot=SLOTLESS, Forbid forbid=FORBIDLESS) : gold(gold), slot(slot), forbid(forbid){}
	int Gold()const{return gold;}
	Forbid Forbid()const{return forbid;}
	Slot Slot()const{return slot;}
};

class Card{ 
	typedef std::shared_ptr<Beast> SPBeast;
	typedef std::shared_ptr<Item> SPItem;
protected:
	ModelData* d;
	CardType cType;
	std::map<int, Card>* map;

	StrategyAct* sa;
	StrategyCombat* sc;
	StrategyEscape* se;
	StrategyWin* sw;
	SPBeast beast;
	SPItem item;

	Card (ModelData* d, std::map<int, Card>* map): d(d), map(map){}

	friend class Cards;
	friend class Model;

public:
	Card(){}
	Card& operator=(const Card& rhs);
	
	Card& operator=(Beast* rhs) {beast=SPBeast(rhs); cType|BEAST; return *this;}
	Card& operator=(StrategyCombat &rhs) {sc=&rhs; cType|STRATEGYCOMBAT; return *this;}
	Card& operator=(StrategyEscape &rhs) {se=&rhs; cType|STRATEGYESCAPE; return *this;}
	Card& operator=(StrategyAct &rhs) {sa=&rhs; cType|STRATEGYPREPARATION; return *this;}
	Card& operator=(StrategyWin &rhs) {sw=&rhs; cType|STRATEGYWIN; return *this;}
	Card& operator=(Item* rhs) {item=SPItem(rhs); cType|EQUIPPABLE|TREASURE; return *this;}
	Card& operator=(const CardType &rhs) {cType=rhs; return * this;}

	CardType CardType() const {return cType;}
//	void Preparation() {sa->Handle(d);}
//	bool Escape() {return se->Handle(d, map);}
//	int Combat() {return sc->Handle(d, map);}
//	void Win() {sw->Handle(d, gainCard,gainLevel);}
};



class Cards{
	Cards();
	static std::map<int, Card> c;
	static std::map<int, Card>* GetMap(ModelData* d);
	friend class Model;
};

#endif