#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <vector>
static const int FOE=0;
static const int HELP=1;
static const int FIRSTPLAYER=2;

enum CardType {
	DOOR=0,
	TREASURE=1,
	BEAST=1<<1,
	CURSE=1<<2,
	CLASS=1<<3,
	RACE=1<<4,
	MULTICLASS=1<<5,
	MULTIRACE=1<<6,
	INSTANTUSE=1<<7,
	BATTLEUSE=1<<8,
	PLAYERUSE=1<<9,
	EQUIPPABLE=1<<10,
	WEARABBlE=1<<11,


};
inline CardType operator|(CardType a, CardType b) {return static_cast<CardType>(static_cast<int>(a) | static_cast<int>(b));}
enum CardGroup {HAND,EQUIP,DESK};
enum Phase {GAMESTART,BEGIN,KICKOPEN,PRECOMBAT,COMBAT,ESCAPE,POSTESCAPE,OPENPICK,CHARITY,DEATH};
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

class GameEvent {
public :
	virtual void TryMove(CardPosition from, CardPosition to, int cp)=0;
};

struct ModelData{
	int totalplayers;
	int plrTurn;
	Phase phase;
	int dice;
	GameEvent *event;
	std::vector<int> inCombat;
	int currentInCombat;
	std::vector<Player> plr;
	std::vector<int> doors,treasures,rd,rt;
};

#endif