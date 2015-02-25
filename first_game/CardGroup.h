#ifndef CARDGROUP_H
#define CARDGROUP_H

enum CardGroup {HAND,EQUIP,DESK};
struct CardPosition {
	CardGroup vectorName;
	int playerNumber;
	int position;
	bool operator==(CardPosition &second){
		if ((this->vectorName==second.vectorName)&&(this->playerNumber==second.playerNumber)&&(this->position==second.position)) return true;
		return false;
	}
};

#endif