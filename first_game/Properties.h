#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <vector>
static const int FOE=0;
static const int HELP=1;
static const int PLAYERS=2;
enum CardType {BEAST};
//enum DoorType {EQUIP,USE};
//enum TreashureType {EQUIP,ONEOFF};
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
	Phase phase;
	int plrTurn;
};

struct ModelData{
	int totalplayers;
	int plrTurn;
	Phase phase;
	std::vector<int> inCombat;
	std::vector<Player> plr;
	std::vector<int> doors,treasures,rd,rt;
};

#endif