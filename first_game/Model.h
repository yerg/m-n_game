#ifndef MODEL_H
#define MODEL_H
#include "CardGroup.h"
#include "Project.h"


class Model {
	mutable bool badLock;
	int totalplayers;
	int plrTurn;
	Phase phase;
	std::vector <int> phaseAdjust;
	std::vector<Player> plr;

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;
	


	void GiveDoor(int n, int pl);
	void GiveTreasure(int n, int pl);
	void GiveToAll(int nd, int nt);
	void StartGame();
	void DoCombat();


	void Lock() const;
public:
	Model(int n) : totalplayers(n), badLock(false) {StartGame();}
	Snapshot GetData(int cp) const;
	void TryMove(CardPosition from, CardPosition to, int cp);
	void EndPhase(Phase phaseToClose, int cp);
};

#endif