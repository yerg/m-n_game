#ifndef CARDGROUP_H
#define CARDGROUP_H

#include <vector>
enum CardGroup {HAND,EQUIP,DESK};
enum Phase {BEGIN,KICKOPEN,COMBAT,CHARITY,DEATH};
struct CardPosition {
	CardGroup vectorName;
	int playerNumber;
	int position;
	bool operator==(CardPosition &second){
		if ((this->vectorName==second.vectorName)&&(this->playerNumber==second.playerNumber)&&(this->position==second.position)) return true;
		return false;
	}
//	CardPosition(CardGroup vectorName, int playerNumber, int position) : vectorName(vectorName), playerNumber(playerNumber), position(position){}
};

struct Player{
	std::vector<int> deck[3];
	int level;
	bool gender;
	Player():level(1),gender(true){deck[HAND].reserve(10); deck[EQUIP].reserve(10); deck[DESK].reserve(10);};
};

struct Snapshot{
	std::vector<Player> plr;
	std::vector<int> monster;
	std::vector<int>  helper;
	Phase phase;
	int plrTurn;
};
#endif