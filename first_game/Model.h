#ifndef MODEL_H
#define MODEL_H
#include "CardGroup.h"
#include "Project.h"


class Model {

	int totalplayers;

	std::vector<Player> plr;

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;

	void GiveCard(int nd, int nt, int pl);
	void GiveToAll(int nd, int nt);
	void StartGame();
public:
	Model(int n) : totalplayers(n){StartGame();}
	Snapshot GetData(int cp);
	void TryMove(CardPosition from, CardPosition to);
};

#endif