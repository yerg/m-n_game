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

	DELETEAFTERCOMBAT=1<<10,

	BEAST=1<<11|DOOR,
	CURSE=1<<12|DOOR|PLAYERUSE,

	MULTIRACE=1<<13|DOOR,
	MULTICLASS=1<<14|DOOR,

	DWARFCARD=1<<15|DOOR,
	ELFCARD=1<<16|DOOR,
	HALFLINGCARD=1<<17|DOOR,
	//reserved =1<<18|DOOR,
	CLASS=DWARFCARD|ELFCARD|HALFLINGCARD,

	WARRIORCARD=1<<19|DOOR,
	CLERICCARD=1<<20|DOOR,
	WIZARDCARD=1<<21|DOOR,
	THIEFCARD=1<<22|DOOR,
	RACE=WARRIORCARD|CLERICCARD|WIZARDCARD|THIEFCARD,
};


enum Setting{
	FREESETTING=0,

	MALE=1,
	FEMALE=1<<1,

	HUMAN=1<<2,
	DWARF=1<<3,
	ELF=1<<4,
	HALFLING=1<<5,
	SUPERDWARF=1<<6,
	SUPERELF=1<<7,
	SUPERHALFLING=1<<8,

	NOTHUMAN=DWARF|ELF|HALFLING,
	NOTDWARF=HUMAN|ELF|HALFLING|SUPERELF|SUPERHALFLING,
	NOTELF=HUMAN|DWARF|HALFLING|SUPERDWARF|SUPERHALFLING,
	NOTHALFLING=HUMAN|DWARF|ELF|SUPERDWARF|SUPERELF,


	CLASSLESS=1<<9,
	WARRIOR=1<<10,
	CLERIC=1<<11,
	WIZARD=1<<12,
	THIEF=1<<13,
	SUPERWARRIOR=1<<14,
	SUPERCLERIC=1<<15,
	SUPERWIZARD=1<<16,
	SUPERTHIEF=1<<17,

	NOTWARRIOR=CLASSLESS|CLERIC|WIZARD|THIEF|SUPERCLERIC|SUPERWIZARD|SUPERTHIEF,
	NOTCLERIC=CLASSLESS|WARRIOR|WIZARD|THIEF|SUPERWARRIOR|SUPERWIZARD|SUPERTHIEF,
	NOTWIZARD=CLASSLESS|WARRIOR|CLERIC|THIEF|SUPERWARRIOR|SUPERCLERIC|SUPERTHIEF,
	NOTTHIEF=CLASSLESS|WARRIOR|CLERIC|WIZARD|SUPERWARRIOR|SUPERCLERIC|SUPERWIZARD,
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
inline Setting operator|(Setting a, Setting b) {return static_cast<Setting>(static_cast<int>(a) | static_cast<int>(b));}
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