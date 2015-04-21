#ifndef MODEL_H
#define MODEL_H
#include "Properties.h"
#include "Project.h"
#include "Cards.h"

class Card;

class Model {
	std::map<int, Card> *map;
	ModelData d;
	std::vector <int> phaseAdjust;

	CardPosition from;
	CardPosition to;
	std::vector<int>::iterator itF;
	std::vector<int>::iterator itT;
	std::vector<int> *pFrom;
	std::vector<int> *pTo;
	CardType fromType;
	int cp;

	void GiveToAll(int nd, int nt);
	void StartGame(int n);
	bool DoCombat();
	void Move(const CardPosition &from, const CardPosition &to);
	void InCombat(int pl);

	void SettingMove();
	void EquipMove();
	void DeskMove();
	void BeastMove();
	void UndeadMove();
	void FoldSetting();

	Model(int n) {StartGame(n);}
	Model(const Model&){}
	Snapshot GetData(int cp) const;
	void TryMove(CardPosition fromIn, CardPosition toIn, int cpIn);
	void EndPhase(Phase phaseToClose, int cpIn);

	friend class ModelHandler;
};

void GiveDoor(ModelData& d, int n, int pl);
void GiveTreasure(ModelData& d, int n, int pl);
Setting CheckSetting(ModelData& d, std::map<int, Card>* map, int pl);
void CheckEquip(ModelData& d, std::map<int, Card>* map, int pl);


class ModelHandler{
	std::unique_ptr<Model> model;
	mutable int semaphore;
	void Lock() const;
public:
	ModelHandler(int n);
	Snapshot GetData(const int &cp) const;
	void TryMove(const CardPosition &from, const CardPosition &to, const int &cp);
	void EndPhase(const Phase &phaseToClose, const int &cp);
};

#endif