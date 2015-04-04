#ifndef CARDS_H
#define CARDS_H

#include "Properties.h"
#include "Project.h"

class Card;

struct StrategyDefeat {
	virtual void Handle(ModelData*d)=0;
};

class Beast{
	typedef std::shared_ptr<StrategyDefeat> SPDefeat;
	int level, gainCard, gainLevel;
	bool undead;
	SPDefeat sd;
public:
	Beast(StrategyDefeat* psd, int level, int gainCard, int gainLevel=1, bool undead=false) : level(level), gainCard(gainCard), gainLevel(gainLevel), undead(undead){sd=SPDefeat(psd);}
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
	int n;
	StrategyCombat () : n(0){}
	StrategyCombat (int n) : n(n){}
	virtual int Handle(ModelData*d, std::map<int, Card>* const map){
		return n;
	}
};
struct StrategyWin {
	virtual void Handle(ModelData*d, int gainCard, int gainLevel);
};

class Item {
	int gold, combat;
	Setting forbid;
	Slot slot;
public:
	Item(int gold, int combat=0, Slot slot=SLOTLESS, Setting forbid=FREESETTING) : gold(gold), combat(combat), slot(slot), forbid(forbid){}
	int Gold()const{return gold;}
	int Combat()const{return combat;}
	Setting Forbid()const{return forbid;}
	Slot Slot()const{return slot;}
};

class Card{ 
	typedef std::shared_ptr<Beast> SPBeast;
	typedef std::shared_ptr<Item> SPItem;
	typedef std::shared_ptr<StrategyAct> SPAct;
	typedef std::shared_ptr<StrategyCombat> SPCombat;
	typedef std::shared_ptr<StrategyEscape> SPEscape;
	typedef std::shared_ptr<StrategyWin> SPWin;
protected:
	ModelData* d;
	CardType cType;
	std::map<int, Card>* map;

	SPAct sa;
	SPCombat sc;
	SPEscape se;
	SPWin sw;
	SPBeast beast;
	SPItem item;

	Card (ModelData* d, std::map<int, Card>* map): d(d), map(map){}

	friend class Cards;
	friend class Model;

public:
	Card(){}
	Card& operator=(const Card& rhs);
	
	Card& operator=(Beast* rhs) {beast=SPBeast(rhs); cType|BEAST; return *this;}
	Card& operator=(StrategyCombat *rhs) {sc=SPCombat(rhs); cType|STRATEGYCOMBAT; return *this;}
	Card& operator=(StrategyEscape *rhs) {se=SPEscape(rhs); cType|STRATEGYESCAPE; return *this;}
	Card& operator=(StrategyAct *rhs) {sa=SPAct(rhs); cType|STRATEGYPREPARATION; return *this;}
	Card& operator=(StrategyWin *rhs) {sw=SPWin(rhs); cType|STRATEGYWIN; return *this;}
	Card& operator=(Item* rhs) {item=SPItem(rhs); cType|EQUIPPABLE|TREASURE; return *this;}
	Card& operator=(const CardType &rhs) {cType=rhs; return * this;}

	CardType CardType() const {return cType;}

	Slot Slot() const {if(item) return item->Slot(); else throw "Item Slot bad access";}
	Setting Forbid() const {if(item) return item->Forbid(); else throw "Item Forbid bad access";}
	int Gold() const {if(item) return item->Gold(); else throw "Item Gold bad access";}

	void Defeat() {if(beast) beast->Defeat(d); else throw "Beast Defeat bad access";}
	bool Undead()const{if(beast) return beast->Undead(); else throw "Beast Undead bad access";}
	int GainCard()const{if(beast) return beast->GainCard(); else throw "Beast GainCard bad access";}
	int GainLevel()const{if(beast) return beast->GainLevel(); else throw "Beast GainLevel bad access";}
	int Level()const{if(beast) return beast->Level(); else throw "Beast Level bad access";}

	int Combat();
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