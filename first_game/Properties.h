#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <vector>
static const int FOE=0;
static const int HELP=1;
static const int FIRSTPLAYER=2;

enum CardType {
	DOOR=0,
	TREASURE=1,
	INSTANTUSE=1<<2,
	BATTLEUSE=1<<3,
	PLAYERUSE=1<<4,
	EQUIPPABLE=1<<5,

	STRATEGYESCAPE=1<<6,
	STRATEGYWIN=1<<7,
	STRATEGYPREPARATION=1<<8,
	STRATEGYCOMBAT=1<<9,

	BEAST=1<<11|DOOR,
	CURSE=1<<12|DOOR|PLAYERUSE,
	CLASS=1<<13|DOOR,
	RACE=1<<14|DOOR,
	MULTICLASS=1<<15|DOOR,
	MULTIRACE=1<<16|DOOR,
};

enum Forbid {
	FORBIDLESS=0,
	MALE=1,
	FEMALE=1<<1,
	HUMAN=1<<2,
	DWARF=1<<3,
	ELF=1<<4,
	HALFLING=1<<5,
	NOTHUMAN=DWARF|ELF|HALFLING,
	NOTDWARF=HUMAN|ELF|HALFLING,
	NOTELF=HUMAN|DWARF|HALFLING,
	NOTHALFLING=HUMAN|DWARF|ELF,
	CLASSLESS=1<<6,
	WARRIOR=1<<7,
	CLERIC=1<<8,
	WIZARD=1<<9,
	THIEF=1<<10,
	ANYCLASS=WARRIOR|CLERIC|WIZARD|THIEF,
	NOTWARRIOR=CLASSLESS|CLERIC|WIZARD|THIEF,
	NOTCLERIC=CLASSLESS|CLERIC|WIZARD|THIEF,
	NOTWIZARD=CLASSLESS|WARRIOR|CLERIC|THIEF,
	NOTTHIEF=CLASSLESS|WARRIOR|CLERIC|WIZARD,
	FORBID=~0
};

enum Slot {
	SLOTLESS=0,
	HAT=1,
	SHOES=1<<1,
	ONEHANDED=1<<2,
	TWOHANDED=1<<3,
	CHEST=1<<4,
	BIG=1<<5
};

inline CardType operator|(CardType a, CardType b) {return static_cast<CardType>(static_cast<int>(a) | static_cast<int>(b));}
inline Forbid operator|(Forbid a, Forbid b) {return static_cast<Forbid>(static_cast<int>(a) | static_cast<int>(b));}
inline Slot operator|(Slot a, Slot b) {return static_cast<Slot>(static_cast<int>(a) | static_cast<int>(b));}

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