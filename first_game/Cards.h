#ifndef CARDS_H
#define CARDS_H

#include "Properties.h"
#include "Project.h"

class Card{
protected:
	ModelData* d;
	CardType cType;
	StackType sType;
	const std::map<int, Card*>* const map;
	Card (ModelData* d, const std::map<int, Card*>* const map): d(d), map(map){}
	friend class Cards;
};

struct StrategyPreparation {
	virtual void Handle(ModelData*d){
		d->phase=COMBAT;
	}
};

struct StrategyEscape {
	virtual bool Handle(ModelData*d, const std::map<int, Card*>* const map){
		int i=rand()%6;
		return i>3;
	}
};

struct StrategyCombat {
	virtual int Handle(ModelData*d, const std::map<int, Card*>* const map){
		return true;
	}
};

struct StrategyWin {
	virtual void Handle(ModelData*d, int gainCard, int gainLevel){

	}
};

struct StrategyDefeat {
	virtual void Handle(ModelData*d);
};

class Beast : public Card{

	int level, gainCard, gainLevel;
	bool undead;
	StrategyPreparation* sp;
	StrategyCombat* sc;
	StrategyEscape* se;
	StrategyWin* sw;
	StrategyDefeat* sd;
public:
	Beast(ModelData* d, const std::map<int, Card*>* const map, StrategyDefeat* sd, int level, int gainCard, int gainLevel=1, bool undead=false);
	void Preparation() {sp->Handle(d);}
	bool Escape() {return se->Handle(d, map);}
	void Defeat() {sd->Handle(d);}
	int Combat() {return sc->Handle(d, map);}
	void Win() {sw->Handle(d, gainCard,gainLevel);}

	void SetPreparation(StrategyPreparation* spSet);
	void SetCombat(StrategyCombat* scSet);
	void SetEscape(StrategyEscape* seSet);
	void SetWin(StrategyWin* swSet);
};

class 

class Cards{
	std::map<int, Card*> c;
	Cards(ModelData* d);
	friend class Model;
};

#endif