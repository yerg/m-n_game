#ifndef CARDS_H
#define CARDS_H

#include "Properties.h"
#include "Project.h"

class Card{
protected:
	ModelData* d;
	CardType cType;
	std::map<int, Card*>* map;
	Card (ModelData* d, std::map<int, Card*>* map): d(d), map(map){}
	friend class Cards;
};

class StrategyPreparation {
	void Handle(){}
};

class StrategyEscape {

};

class StrategyCombat {

};

class StrategyWin {

};

class StrategyDefeat {

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
	Beast(ModelData* d, std::map<int, Card*>* map, StrategyDefeat* sd, int level, int gainCard, int gainLevel=1, bool undead=false);



/*	void SetPreparation(StrategyPreparation* spSet);
	void SetCombat(StrategyCombat* scSet);
	void SetEscape(StrategyEscape* seSet);
	void SetWin(StrategyWin* swSet);
	void SetDefeat(StrategyDefeat* sdSet); */ //saved for closed architecture
};



class Cards{
	std::map<int, Card*> c;
	Cards(ModelData* d);
	friend class Model;
};

#endif