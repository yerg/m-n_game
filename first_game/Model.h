#ifndef MODEL_H
#define MODEL_H
#include "CardGroup.h"
#include "Munchkin.h"
class Model {

	struct Player{
	
	public:
		std::vector<int> deck[3];
		int level;
		bool gender;
		Player():level(1),gender(true){deck[HAND].reserve(10); deck[EQUIP].reserve(10); deck[DESK].reserve(10);};
	};

	int totalplayers;

	std::vector<Player> plr;

	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;

	void GiveCard(int nd, int nt, int pl);
	void GiveToAll(int nd, int nt);
	void StartGame();
public:
	Model(){StartGame();}
	
	void TryMove(CardPosition from, CardPosition to);
};

#endif