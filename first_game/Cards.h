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
	virtual bool Handle(ModelData*d, std::map<int, Card>* const map)=0;
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
struct StrategySwap {
	virtual void Handle(ModelData*d, std::map<int, Card>* const map, CardPosition from, CardPosition to)=0;
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
	typedef std::shared_ptr<StrategySwap> SSwap;

protected:
	ModelData* d;
	CardType cType;
	std::map<int, Card>* map;

	SPAct sa;
	SPCombat sc;
	SPEscape se;
	SPWin sw;
	SSwap ss;
	SPBeast beast;
	SPItem item;

	Card (ModelData* d, std::map<int, Card>* map): d(d), map(map), cType(FREETYPE){}

	friend class Cards;
	friend class Model;

public:
	Card(){}
	Card& operator=(const Card& rhs);
	
	Card& operator=(Beast* rhs) {beast=SPBeast(rhs); cType=cType|BEAST; return *this;}
	Card& operator=(StrategyCombat *rhs) {sc=SPCombat(rhs); cType=cType|STRATEGYCOMBAT; return *this;}
	Card& operator=(StrategyEscape *rhs) {se=SPEscape(rhs); cType=cType|STRATEGYESCAPE; return *this;}
	Card& operator=(StrategyAct *rhs) {sa=SPAct(rhs); cType=cType|STRATEGYPREPARATION; return *this;}
	Card& operator=(StrategyWin *rhs) {sw=SPWin(rhs); cType=cType|STRATEGYWIN; return *this;}
	Card& operator=(StrategySwap *rhs) {ss=SSwap(rhs); cType=cType|STRATEGYSWAP; return *this;}
	Card& operator=(Item* rhs) {item=SPItem(rhs); cType=cType|ITEM; return *this;}
	Card& operator=(const CardType &rhs) {cType=cType|rhs; return * this;}

	CardType CardType() const {return cType;}

	Slot Slot() const {if(item) return item->Slot(); else throw "Item Slot bad access";}
	Setting Forbid() const {if(item) return item->Forbid(); else throw "Item Forbid bad access";}
	int Gold() const {if(item) return item->Gold(); else throw "Item Gold bad access";}

	void Defeat() {if(beast) beast->Defeat(d); else throw "Beast Defeat bad access";}
	bool Undead()const{if(beast) return beast->Undead(); else throw "Beast Undead bad access";}
	int GainCard()const{if(beast) return beast->GainCard(); else throw "Beast GainCard bad access";}
	int GainLevel()const{if(beast) return beast->GainLevel(); else throw "Beast GainLevel bad access";}
	int Level()const{if(beast) return beast->Level(); else throw "Beast Level bad access";}
	void Swap(CardPosition from, CardPosition to)const{if(ss) return ss->Handle(d,map,from,to); else throw "StrategySwap bad access";}

	void Act(const int &pl)const{if(sa) return sa->Handle(d,map,pl); else throw "StrategyAct bad access";}


	int Combat() const;
//	bool Escape() {return se->Handle(d, map);}
//	void Win() const
};



class Cards{
	Cards();
	static std::map<int, Card> c;
	static std::map<int, Card>* GetMap(ModelData* d);
	friend class Model;
};

#endif