#include "Model.H"

ModelHandler::ModelHandler(int n) : semaphore(false){
	model = std::unique_ptr<Model>(new Model(n));
}
Snapshot ModelHandler::GetData(const int &cp) const{
	Lock();
	Snapshot tmp=model->GetData(cp);
	--semaphore;
	return tmp;
}
void ModelHandler::TryMove(const CardPosition &from, const CardPosition &to, const int &cp){
	Lock();
	model->TryMove(from, to, cp);
	--semaphore;
}
void ModelHandler::EndPhase(const Phase &phaseToClose, const int &cp){
	Lock();
	model->EndPhase(phaseToClose, cp);
	--semaphore;
}
void ModelHandler::Lock() const{
	while(++semaphore>1){
		--semaphore;
		SDL_Delay(100+rand()%100);
	}
}

void GiveDoor(ModelData& d, int n, int pl){
	while (n){
		d.plr[pl].deck[HAND].push_back(d.doors.back());
		d.doors.pop_back();
		--n;
		if (d.doors.size()==1) {
			d.doors.insert(d.doors.end(),d.rd.begin(),d.rd.end());
			std::random_shuffle(d.doors.begin(),d.doors.end());
			d.rd.clear();
		}
	}
}
void GiveTreasure(ModelData& d, int n, int pl){
	while (n){
		d.plr[pl].deck[HAND].push_back(d.treasures.back());
		d.treasures.pop_back();
		--n;
		if (d.treasures.size()==1) {
			d.treasures.insert(d.treasures.end(),d.rt.begin(),d.rt.end());
			std::random_shuffle(d.treasures.begin(),d.treasures.end());
			d.rt.clear();
		}
	}
}
Setting CheckSetting(ModelData& d, std::map<int, Card>* map, int pl) {
	Setting stg=FREESETTING;
	CardType tmp=CardType(0); 
	int classes=0, races=0;
	stg= d.plr[pl].gender ? stg|MALE : stg|FEMALE;
	std::for_each(
		d.plr[pl].deck[EQUIP].begin(),
		d.plr[pl].deck[EQUIP].end(),
		[map, &tmp, &classes, &races](const int &c){
			tmp=tmp|(*map)[c].CardType(); 
			if ((*map)[c].CardType()&CLASS) ++classes;
			if ((*map)[c].CardType()&RACE) ++races;
	});

	if ( (classes>2) || ((classes==2) && (!(tmp&MULTICLASS))) || ((classes<1)&&(tmp&MULTICLASS)) ) throw "Bad classes math";
	if ( (races>2) || ((races==2)&&(!(tmp&MULTIRACE))) || ((races<1)&&(tmp&MULTIRACE)) ) throw "Bad races math";

	if (races==1 && tmp&MULTIRACE) {
		if (tmp&DWARFCARD){ stg=stg|SUPERDWARF;
		} else if (tmp&ELFCARD){ stg=stg|SUPERELF;
		} else if (tmp&HALFLINGCARD){ stg=stg|SUPERHALFLING; 
		} else { throw "Empty multiclass";}
	} else {
		if (tmp&DWARFCARD){ stg=stg|DWARF;
		} else if (tmp&ELFCARD){ stg=stg|ELF;
		} else if (tmp&HALFLINGCARD){ stg=stg|HALFLING; 
		} else stg=stg|HUMAN;
	}

	if (classes==1 && tmp&MULTICLASS) {
		if (tmp&WARRIORCARD){ stg=stg|SUPERWARRIOR;
		} else if (tmp&CLERICCARD){ stg=stg|SUPERCLERIC;
		} else if (tmp&WIZARDCARD){ stg=stg|SUPERWIZARD;
		} else if (tmp&THIEFCARD){ stg=stg|SUPERTHIEF; 
		} else throw "Empty multirace";
	} else {
		if (tmp&WARRIORCARD){ stg=stg|WARRIOR;
		} else if (tmp&CLERICCARD){ stg=stg|CLERIC;
		} else if (tmp&WIZARDCARD){ stg=stg|WIZARD;
		} else if (tmp&THIEFCARD){ stg=stg|THIEF; 
		} else  stg=stg|CLASSLESS;
	}

	return stg;
}
void CheckEquip(ModelData& d, std::map<int, Card>* map, int pl){
	Setting setting=CheckSetting(d,map,pl);
	Slot tmp=SLOTLESS;
	d.plr[pl].deck[EQUIP].erase(std::remove_if(d.plr[pl].deck[EQUIP].begin(), d.plr[pl].deck[EQUIP].end(),[&](int c)->bool{
		if (setting&DWARF) tmp=(Slot(~BIG))&tmp;
		bool b=false;
		if ((*map)[c].CardType()&ITEM) {
			if (((*map)[c].Slot()&tmp) || (setting&(*map)[c].Forbid())){
				d.plr[pl].deck[DESK].push_back(c);
				b=true;
			} 
		tmp=((TWOHANDED&(*map)[c].Slot()) ? tmp=TWOHANDED|ONEHANDED|tmp 
			: (ONEHANDED&(*map)[c].Slot()) ? tmp&TWOHANDED ? tmp=ONEHANDED|tmp : tmp=TWOHANDED|tmp 
			: tmp|(*map)[c].Slot());
		}
		return b;
	}),d.plr[pl].deck[EQUIP].end());
}

void Model::GiveToAll(int nd, int nt){
	for (int i=FIRSTPLAYER; i<d.totalplayers; i++) {
		GiveDoor(d, nd,i);
		GiveTreasure(d, nt,i);
	}
}
void Model::StartGame(int n){
	map=Cards::GetMap(&d);
	d.totalplayers=n+FIRSTPLAYER;
	d.plr.resize(d.totalplayers);
	phaseAdjust.assign(d.totalplayers, 0);
	cardCounter.assign(d.totalplayers, 0);
	
	srand(time(NULL));

	d.doors.reserve(95);
	d.rd.reserve(95);
	for(int i=0; i<95;i++) d.doors.push_back(i);
	random_shuffle(d.doors.begin(),d.doors.end());

	d.treasures.reserve(75);
	d.rt.reserve(75);
	for(int i=95; i<170;i++) d.treasures.push_back(i);
	random_shuffle(d.treasures.begin(),d.treasures.end());

	GiveToAll(9,16);
	d.escape=false;
	d.dice=0;
	d.plrTurn=FIRSTPLAYER;
	d.phase=GAMESTART;
	d.event=NULL;
}
Snapshot Model::GetData(int cp) const{
	Snapshot tmp;
	Player pl;
	for (int i=0; i<d.totalplayers; i++){
		pl.gender=d.plr[i].gender;
		pl.level=d.plr[i].level;
		pl.deck[EQUIP]=d.plr[i].deck[EQUIP];
		pl.deck[DESK]=d.plr[i].deck[DESK];
		if (cp==i){
			pl.deck[HAND]=d.plr[i].deck[HAND];
		} else {
			pl.deck[HAND].clear();
			for (std::vector<int>::const_iterator it=d.plr[i].deck[HAND].begin(); it!=d.plr[i].deck[HAND].end(); ++it){			
				pl.deck[HAND].push_back(*it < 95 ? -1 : -2);
			}
		}
		tmp.plr.push_back(pl);
		
	}
	tmp.phase=d.phase;
	tmp.plrTurn=d.plrTurn;
	tmp.currentInCombat=d.currentInCombat;
	tmp.inCombat=d.inCombat;
	if (d.event!=NULL) tmp.event=true; else tmp.event=false;
	tmp.dice=d.dice;
	tmp.escape=d.escape;
	return tmp;
}

void Model::TryMove(CardPosition fromIn, CardPosition toIn, int cpIn){
	from=fromIn;
	to=toIn;
	cp=cpIn;
	pFrom=&(d.plr.at(from.playerNumber).deck[from.vectorName]);
	pTo=&(d.plr.at(to.playerNumber).deck[to.vectorName]);


	if ((from==to)||(from.position>=pFrom->size())||((to.position>=pTo->size())&&(to.position>0))||(from.playerNumber!=cp)) return;
	fromType=map->at(pFrom->at(from.position)).CardType();
	itF=pFrom->begin()+from.position;
	itT=pTo->begin()+to.position+1;

	if ((from.playerNumber==to.playerNumber)&&(from.vectorName==to.vectorName)) {			//internal vector swaps;
		if (fromType&STRATEGYSWAP) ((*map)[pFrom->at(from.position)]).Swap(from, to);
		if (to.position<0){
			int a =*itF;
			pTo->insert(itT,a);
			pFrom->erase(pFrom->begin()+from.position+1);
		} else {
			std::swap(*itF,*(--itT));
		}

		
	} else {
		SettingMove();
		FoldSetting();
		switch (d.phase) {
		case GAMESTART:
			EquipMove();
			DeskMove();
			break;
		case BEGIN:
			EquipMove();
			DeskMove();
			break;
		case KICKOPEN:
			if (cp==d.plrTurn) {
				InCombat(cp);
				BeastMove();
			}
			break;
		case COMBAT:
			break;
		case ESCAPE:
			break;
		case POSTESCAPE:
			break;
		case OPENPICK:
			EquipMove();
			DeskMove();
			break;
		case CHARITY:
			CharityMove();
			if (d.plr[cp].deck[HAND].size()<=5) {
				if(d.plrTurn!=d.totalplayers) ++d.plrTurn; else d.plrTurn=FIRSTPLAYER;
				d.phase=BEGIN;
			}
			break;
		case END:
			EquipMove();
			DeskMove();
			break;
		}
	}
}

void Model::InCombat(int pl){
	d.inCombat.push_back(pl);
	d.phase=COMBAT;
	std::for_each(
		d.plr[FOE].deck[EQUIP].begin(),
		d.plr[FOE].deck[EQUIP].end(),
		[this,&pl](const int &c){ if((*map)[c].CardType()&STRATEGYPREPARATION) (*map)[c].Act(pl);}
	);
}

void Model::CharityMove(){
	int *currentCounter;
	if ((from.playerNumber=cp)&&(to.vectorName==HAND)&&(from.vectorName==HAND)){
		if (cardCounter[cp]>0){
			if (to.playerNumber==FOE) {
				FoldFrom();
			}
		} else {
			currentCounter=&cardCounter[to.playerNumber];
			if ((*currentCounter>0)&&(std::find_if(cardCounter.begin(),cardCounter.end(),[currentCounter](const int &i){return ((i>0)&&(*currentCounter>i));})==cardCounter.end())) {
				++(*currentCounter);
				Move(from,to);
			}
		}
	}
}
void Model::SettingMove(){
	if ((fromType&(CLASS|RACE|MULTICLASS|MULTIRACE)) && (from.playerNumber==to.playerNumber) && (to.vectorName==EQUIP) && (from.vectorName==HAND) &&( 
		( (fromType&CLASS)&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return CLASS&(*map)[c].CardType();})==pTo->end()) ) || 
		( (fromType&RACE )&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return RACE &(*map)[c].CardType();})==pTo->end()) ) ||
		( (fromType&MULTICLASS)&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return MULTICLASS&(*map)[c].CardType();})==pTo->end())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return CLASS&(*map)[c].CardType();})!=pTo->end()) ) ||
		( (fromType&MULTIRACE )&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return MULTIRACE &(*map)[c].CardType();})==pTo->end())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return RACE &(*map)[c].CardType();})!=pTo->end()) ) )) 
	{
		Move(from,to);
		CheckEquip(d,map,cp);
	}
}
void Model::EquipMove(){
	Setting setting;
	Slot slot, tmp=SLOTLESS;
	if ( (fromType&EQUIPPABLE)&&(to.vectorName==EQUIP)&&(from.playerNumber==to.playerNumber) ) {
		
		slot=map->at(*itF).Slot();
		setting=CheckSetting(d,map,from.playerNumber);
		
		if (!(setting&map->at(*itF).Forbid())) {
			std::for_each(pTo->begin(), pTo->end(), [this, &tmp](const int &c){
				if ((*map)[c].CardType()&ITEM){
					tmp = ((TWOHANDED&(*map)[c].Slot()) ? tmp=TWOHANDED|ONEHANDED|tmp 
						: (ONEHANDED&(*map)[c].Slot()) ? tmp&TWOHANDED ? tmp=ONEHANDED|tmp : tmp=TWOHANDED|tmp 
						: tmp|(*map)[c].Slot());}
			});
			if (setting&DWARF) tmp=(Slot(~BIG))&tmp;
			if (!(tmp&slot)) {
				Move(from,to);
			}
		}
	}
}
void Model::DeskMove(){
	if ((fromType&ITEM)&&(to.vectorName==DESK)&&(to.playerNumber==from.playerNumber)) Move(from,to);
}
void Model::BeastMove(){
	if ((fromType&BEAST)&&(to.playerNumber==FOE)&&(to.vectorName==EQUIP)){
		if(fromType&STRATEGYPREPARATION) {
			std::for_each(d.inCombat.begin(), d.inCombat.end(), [this](const int &pl){ (*map)[*itF].Act(pl);});
		}
		to.position=pTo->size()-1;
		Move(from,to);
	}
}
void Model::UndeadMove(){
	if ((fromType&BEAST)&&(map->at(*itF).Undead())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return (*map)[c].Undead();})!=pTo->end())) BeastMove();
}
void Model::FoldSetting(){
	if((fromType&(CLASS|RACE)) && (to.vectorName==HAND) && (to.playerNumber==FOE) && (from.vectorName==EQUIP)){
		FoldFrom();
		if (fromType&(CLASS)) {
			if (std::find_if(pFrom->begin(),pFrom->end(), [this](const int &c){return CLASS&(*map)[c].CardType();})==pFrom->end()) {
				for (std::vector<int>::iterator it=pFrom->begin(); it!=pFrom->end(); ++it) {
					if (MULTICLASS&(*map)[*it].CardType()) {
						d.rd.push_back(*it);
						d.plr.at(from.playerNumber).deck[from.vectorName].erase(it);
						return;
					}
				}
			}
		} else {
			if (std::find_if(pFrom->begin(),pFrom->end(), [this](const int &c){return RACE&(*map)[c].CardType();})==pFrom->end()) {
				for (std::vector<int>::iterator it=pFrom->begin(); it!=pFrom->end(); ++it) {
					if (MULTIRACE&(*map)[*it].CardType()) {
						d.rd.push_back(*it);
						d.plr.at(from.playerNumber).deck[from.vectorName].erase(it);
						return;
					}
				}
			}
		}
		CheckEquip(d,map,cp);
	}
}

void Model::Move(const CardPosition &moveFrom, const CardPosition &moveTo){
	d.plr.at(moveTo.playerNumber).deck[moveTo.vectorName].insert(
		d.plr.at(moveTo.playerNumber).deck[moveTo.vectorName].begin()+moveTo.position+1,
		*(d.plr.at(moveFrom.playerNumber).deck[moveFrom.vectorName].begin()+moveFrom.position)
		);

	d.plr.at(moveFrom.playerNumber).deck[moveFrom.vectorName].erase(
		d.plr.at(moveFrom.playerNumber).deck[moveFrom.vectorName].begin()+moveFrom.position
		);
}

bool Model::TryEnd(){
	phaseAdjust.at(cp)=1;
	iterPlayer=find(phaseAdjust.begin()+FIRSTPLAYER,phaseAdjust.end(),0);
	if (iterPlayer==phaseAdjust.end()){
		phaseAdjust.assign(d.totalplayers,0);
		return true;
	} else {
		return false;
	}
}
void Model::EndPhase(Phase phaseToClose, int cpIn){
	cp=cpIn;
	if (phaseToClose==d.phase) {
		switch (d.phase) {

		case GAMESTART :
			if (TryEnd()) d.phase=BEGIN;
			break;

		case BEGIN :
			//If cp pressed - open door
			if (cp==d.plrTurn) { 
				d.plr[FOE].deck[EQUIP].push_back(d.doors.back());
				d.doors.pop_back();
				if (d.doors.size()==1) {
					d.doors.insert(d.doors.end(),d.rd.begin(),d.rd.end());
					std::random_shuffle(d.doors.begin(),d.doors.end());
					d.rd.clear();
				}
				//Check cardType
				if (( map->at(*d.plr[FOE].deck[EQUIP].begin()) ).cType & BEAST) {
					InCombat(d.plrTurn);
				} else {
					d.phase=KICKOPEN;
				}
			}
			break;

		case KICKOPEN :
			if (TryEnd()) d.phase=END;
			break;

		case COMBAT :
			if (TryEnd()){
				if(DoCombat()) {
					for (int i=0;i<d.inCombat.size();++i){
						d.currentInCombat=i;
						std::for_each(d.plr[FOE].deck[EQUIP].begin(),d.plr[FOE].deck[EQUIP].end(),[this](const int& c){if((*map)[c].CardType()&(BEAST|STRATEGYCOMBAT)) (*map)[c].Win();});
					}
					ClearAfterCombat();
					d.phase=END;
				} else {
					d.dice=rand()%6+1;
					d.phase=ESCAPE;
					d.currentInCombat=0;
					iterBeastEscape=d.plr[FOE].deck[EQUIP].begin();
					iterBeastEscape=std::find_if(iterBeastEscape,d.plr[FOE].deck[EQUIP].end(),[this](const int& c){return static_cast<bool>((*map)[c].CardType()&BEAST);});
					if (iterBeastEscape!=d.plr[FOE].deck[EQUIP].end()){
							
					}
				}
			}
			break;

		case ESCAPE :
			if(TryEnd()) d.phase=POSTESCAPE;
			break;

		case POSTESCAPE :
			if (TryEnd()){
				iterBeastEscape=std::find_if(++iterBeastEscape,d.plr[FOE].deck[EQUIP].end(),[this](const int& c){return static_cast<bool>((*map)[c].CardType()&BEAST);});
				if (iterBeastEscape==d.plr[FOE].deck[EQUIP].end()) {
					d.currentInCombat++;
					if(d.currentInCombat>=d.inCombat.size()) {
						ClearAfterCombat();
						d.phase=END;
					} else {
						iterBeastEscape=d.plr[FOE].deck[EQUIP].begin();
						iterBeastEscape=std::find_if(iterBeastEscape,d.plr[FOE].deck[EQUIP].end(),[this](const int& c){return static_cast<bool>((*map)[c].CardType()&BEAST);});
						d.phase=ESCAPE;
					}
				} else {
					d.phase=ESCAPE;
				}
			}
			break;
		case END :
			if (cp==d.plrTurn) {
				if(d.plr[cp].deck[HAND].size()<=5){
					if(d.plrTurn!=d.totalplayers) ++d.plrTurn; else d.plrTurn=FIRSTPLAYER;
					d.phase=BEGIN;
				} else {
					CardCounterReset();
					d.phase=CHARITY;
				}
			}
			break;
		case CHARITY :
			if ((cp==d.plrTurn)&&(d.plr[cp].deck[HAND].size()<=5)) throw "Bad CHARITY Move block";
			break;

		}
	}
}

void Model::FoldFrom(){
	if(fromType&DOOR){
		d.rd.push_back(*itF);
	} else if (fromType&TREASURE) {
		d.rt.push_back(*itF);
	} else throw "Card is without DOOR\\TREASURE type";
	d.plr.at(from.playerNumber).deck[from.vectorName].erase(d.plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position);
}
void Model::CardCounterReset(){
	int minLevel=10;
	cardCounter.assign(d.totalplayers,0);
	for (int i=FIRSTPLAYER; i<d.totalplayers; i++) {
		if (d.plr[i].level<minLevel) minLevel=d.plr[i].level;
	}
	for (int i=FIRSTPLAYER; i<d.totalplayers; i++) {
		if (d.plr[i].level==minLevel) cardCounter[i]=1;
	}
}
void Model::ClearAfterCombat(){
	std::vector<int> *currentVector;
	for (d.currentInCombat=0; d.currentInCombat<d.inCombat.size();d.currentInCombat++) {
		currentVector=&d.plr[d.inCombat[d.currentInCombat]].deck[EQUIP];
		currentVector->erase(std::remove_if(currentVector->begin(),currentVector->end(),
			[this](const int& c)->bool{
				if((*map)[c].CardType()&DELETEAFTERCOMBAT) { d.rd.push_back(c); return true;} 
				else { return false;}
		}), currentVector->end());
	}
	std::for_each(
		d.plr[FOE].deck[EQUIP].begin(),
		d.plr[FOE].deck[EQUIP].end(),
		[this](const int& c){
			if((*map)[c].CardType()&DOOR){
				d.rd.push_back(c);
			} else if((*map)[c].CardType()&TREASURE) {
				d.rt.push_back(c);
			} else throw "Card without type (DOOR\TREASURE)";
	});
	d.plr[FOE].deck[EQUIP].clear();
	
	std::for_each(
		d.plr[HELP].deck[EQUIP].begin(),
		d.plr[HELP].deck[EQUIP].end(),
		[this](const int& c){
			if((*map)[c].CardType()&DOOR){
				d.rd.push_back(c);
			} else if((*map)[c].CardType()&TREASURE) {
				d.rt.push_back(c);
			} else throw "Card without type (DOOR\TREASURE)";
	});
	d.plr[HELP].deck[EQUIP].clear();
}
bool Model::DoCombat(){
	int score=0;
	bool warrior=false;
	for(std::vector<int>::iterator it=d.inCombat.begin();it!=d.inCombat.end();++it){
		std::for_each(d.plr[*it].deck[EQUIP].begin(), d.plr[*it].deck[EQUIP].end(), [this,&it,&score, &warrior](const int &c) {
			if ( (*map)[c].CardType()&(ITEM|STRATEGYCOMBAT) ) { score+=(*map)[c].Combat(); }
			score+=d.plr[*it].level;
			if ((WARRIOR|SUPERWARRIOR)&CheckSetting(d,map,*it)) warrior=true;
		});
	}
	std::for_each(d.plr[HELP].deck[EQUIP].begin(), d.plr[HELP].deck[EQUIP].end(), [&score,this](const int &c) {
		if ( (*map)[c].CardType()&(ITEM|STRATEGYCOMBAT|BEAST) ) { score+=(*map)[c].Combat(); }
	});
	std::for_each(d.plr[FOE].deck[EQUIP].begin(), d.plr[FOE].deck[EQUIP].end(), [&score,this](const int &c) {
		if ( (*map)[c].CardType()&(ITEM|STRATEGYCOMBAT|BEAST) ) { score-=(*map)[c].Combat(); }
	});
	if (warrior) ++score;
	return score>0;
}