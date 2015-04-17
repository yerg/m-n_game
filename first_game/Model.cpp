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

	if (classes==1 && tmp&MULTICLASS) {
		if (tmp&DWARFCARD) stg=stg|SUPERDWARF;
		if (tmp&ELFCARD) stg=stg|SUPERELF;
		if (tmp&HALFLINGCARD) stg=stg|SUPERHALFLING; else throw "Empty multiclass";
	} else {
		if (tmp&DWARFCARD) stg=stg|DWARF;
		if (tmp&ELFCARD) stg=stg|ELF;
		if (tmp&HALFLINGCARD) stg=stg|HALFLING; else stg=stg|HUMAN;
	}

	if (classes==1 && tmp&MULTICLASS) {
		if (tmp&WARRIORCARD) stg=stg|SUPERWARRIOR;
		if (tmp&CLERICCARD) stg=stg|SUPERCLERIC;
		if (tmp&WIZARDCARD) stg=stg|SUPERWIZARD;
		if (tmp&THIEFCARD) stg=stg|SUPERTHIEF; else throw "Empty multirace";
	} else {
		if (tmp&WARRIORCARD) stg=stg|WARRIOR;
		if (tmp&CLERICCARD) stg=stg|CLERIC;
		if (tmp&WIZARDCARD) stg=stg|WIZARD;
		if (tmp&THIEFCARD) stg=stg|SUPERTHIEF; else stg=stg|CLASSLESS;
	}

	return stg;
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
	phaseAdjust.resize(d.totalplayers);
	phaseAdjust.assign(d.totalplayers, 0);
	srand(time(NULL));

	d.doors.reserve(95);
	d.rd.reserve(95);
	for(int i=0; i<95;i++) d.doors.push_back(i);
	random_shuffle(d.doors.begin(),d.doors.end());

	d.treasures.reserve(75);
	d.rt.reserve(75);
	for(int i=95; i<170;i++) d.treasures.push_back(i);
	random_shuffle(d.treasures.begin(),d.treasures.end());

	GiveToAll(4,4);
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
	if (d.event!=NULL) tmp.event=true; else tmp.event=false;
	tmp.dice=d.dice;
	return tmp;
}

void Model::TryMove(CardPosition fromIn, CardPosition toIn, int cp){
	from=fromIn;
	to=toIn;
	pFrom=&(d.plr.at(from.playerNumber).deck[from.vectorName]);
	pTo=&(d.plr.at(to.playerNumber).deck[to.vectorName]);


	if ((from==to)||(from.position>=pFrom->size())||((to.position>=pTo->size())&&(to.position>0))||(from.playerNumber!=cp)) return;
	bool permission=false;
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

		switch (d.phase) {
		case GAMESTART:
			permission=SettingPerm()||EquipPerm()||DeskPerm();
			break;
		case BEGIN:
			permission=SettingPerm()||EquipPerm()||DeskPerm();
			break;
		case KICKOPEN:
			if ((cp==d.plrTurn)&&(BeastPerm() )) {
				to.position=pTo->size()-1;
				InCombat(cp);
				permission=true;
			}
			break;
		case COMBAT:
			permission=SettingPerm();
			if(UndeadPerm()&&(fromType&STRATEGYPREPARATION)) {
				std::for_each(d.inCombat.begin(), d.inCombat.end(), [this](const int &pl){ (*map)[*itF].Act(pl);});
				to.position=pTo->size()-1;
				permission=true;
			}
			break;
		case ESCAPE:
			permission=true;
			break;
		case POSTESCAPE:
			permission=true;
			break;
		case OPENPICK:
			permission=SettingPerm()||EquipPerm()||DeskPerm();
			break;
		case CHARITY:
			permission=SettingPerm()||EquipPerm()||DeskPerm();
			break;
		}
		if (permission) Move(from,to); else return;
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

bool Model::SettingPerm(){
	return (fromType&(CLASS|RACE|MULTICLASS|MULTIRACE)) && (from.playerNumber==to.playerNumber) && (to.vectorName==EQUIP) && (from.vectorName==HAND) &&( 
		( (fromType&CLASS)&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return CLASS&(*map)[c].CardType();})==pTo->end()) ) || 
		( (fromType&RACE )&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return RACE &(*map)[c].CardType();})==pTo->end()) ) ||
		( (fromType&MULTICLASS)&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return MULTICLASS&(*map)[c].CardType();})==pTo->end())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return CLASS&(*map)[c].CardType();})!=pTo->end()) ) ||
		( (fromType&MULTIRACE )&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return MULTIRACE &(*map)[c].CardType();})==pTo->end())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return RACE &(*map)[c].CardType();})!=pTo->end()) ) );
}
bool Model::EquipPerm(){
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
						: tmp|(*map)[c].Slot());
				}
			});
			if (setting&DWARF) tmp=(Slot(~BIG))&tmp;
			if (!(tmp&slot)) return true;
		}
	}
	return false;
}
bool Model::DeskPerm(){
	if ((fromType&ITEM)&&(to.vectorName==DESK)&&(to.playerNumber==from.playerNumber)) return true;
	return false;
}
bool Model::BeastPerm(){
	if ((fromType&BEAST)&&(to.playerNumber==FOE)&&(to.vectorName==EQUIP)) return true;
	return false;
}
bool Model::UndeadPerm(){
	if (BeastPerm()&&(map->at(*itF).Undead())&&(std::find_if(pTo->begin(), pTo->end(), [this](const int &c){return (*map)[c].Undead();})!=pTo->end())) return true;
	return false;
}


void Model::Move(const CardPosition &from, const CardPosition &to){
	d.plr.at(to.playerNumber).deck[to.vectorName].insert(
		d.plr.at(to.playerNumber).deck[to.vectorName].begin()+to.position+1,
		*(d.plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position)
		);

	d.plr.at(from.playerNumber).deck[from.vectorName].erase(
		d.plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position
		);
}

void Model::EndPhase(Phase phaseToClose, int cp){
	std::vector<int>::iterator it;
	if (phaseToClose==d.phase) {
		switch (d.phase) {
		case GAMESTART :
			phaseAdjust.at(cp)=1;
			it=find(phaseAdjust.begin(),phaseAdjust.end(),0);
			if (it!=phaseAdjust.end()){
				d.phase=BEGIN;
				phaseAdjust.assign(d.totalplayers,0);
			}	
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
					d.phase=COMBAT;
				} else {
					d.phase=KICKOPEN;
				}
			}
			break;

		case KICKOPEN :
			phaseAdjust.at(cp)=1;
			it=find(phaseAdjust.begin(),phaseAdjust.end(),0);
			if (it!=phaseAdjust.end()){
				d.phase=CHARITY;
				phaseAdjust.assign(d.totalplayers,0);
			}	
			break;

		case COMBAT :
			phaseAdjust.at(cp)=1;
			it=find(phaseAdjust.begin(),phaseAdjust.end(),0);
			if (it!=phaseAdjust.end()){
				
				phaseAdjust.assign(d.totalplayers,0);
			}
			break;

		case CHARITY :
			
			break;
		}
	}
}