#ifndef CARDS_H
#define CARDS_H

#include "Properties.h"
#include "Project.h"

class Card{
protected:
	ModelData* d;
	CardType cType;
	const std::map<int, Card*>* const map;
	Card (ModelData* d, const std::map<int, Card*>* const map): d(d), map(map){}
	friend class Cards;
	friend class Model;
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
	virtual void Handle(ModelData*d, int gainCard, int gainLevel);
};

struct StrategyDefeat {
	virtual void Handle(ModelData*d)=0;
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



class LevelUp : public Card {
public:
	LevelUp (ModelData* d, const std::map<int, Card*>* const map);
};

class Item : public Card {
	int gold;
public:
	Item (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|TREASURE;}
};

class Cleric : public Card {
public:
	Cleric (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|CLASS;}
};

class Dwarf : public Card {
public:
	Dwarf (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|RACE;}
};

class Elf : public Card {
public:
	Elf (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|RACE;}
};

class Halfling : public Card {
public:
	Halfling (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|RACE;}
};

class Thief : public Card{
public:
	Thief (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|CLASS;}
};

class Warrior : public Card {
public:
	Warrior (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|CLASS;}
};

class Wizard : public Card{
public:
	Wizard (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|CLASS;}
};

class Curse : public Card{
public:
	Curse (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|DOOR|CURSE;}
};

class InstantUse : public Card {
	virtual void Use()=0;
public:
	InstantUse (ModelData* d, const std::map<int, Card*>* const map) : Card(d,map) {cType|INSTANTUSE;}
};

class DivineIntervention : public InstantUse {
	virtual void Use();
public:
	DivineIntervention (ModelData* d, const std::map<int, Card*>* const map) : InstantUse(d,map) {cType|DOOR;}
};



class Cards{
	Cards();
	static std::map<int, Card*> c;
	static std::map<int, Card*>* GetMap(ModelData* d);
	friend class Model;
};

#endif