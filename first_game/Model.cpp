#include "Model.H"

void Model::GiveDoor(int n, int pl){
	while (n){
		plr[pl].deck[0].push_back(doors.back());
		doors.pop_back();
		--n;
		if (doors.size()==1) {
			doors.insert(doors.end(),rd.begin(),rd.end());
			std::random_shuffle(doors.begin(),doors.end());
			rd.clear();
		} else if(!doors.size()) {
			return;
		}
	}
}

void Model::GiveTreasure(int n, int pl){
	while (n){
		plr[pl].deck[0].push_back(treasures.back());
		treasures.pop_back();
		--n;
		if (treasures.size()==1) {
			treasures.insert(treasures.end(),rt.begin(),rt.end());
			std::random_shuffle(treasures.begin(),treasures.end());
			rt.clear();
		} else if(!treasures.size()) {
			return;
		}
	}
}
void Model::GiveToAll(int nd, int nt){
	for (int i=PLAYERS; i<totalplayers; i++) {
		GiveDoor(nd,i);
		GiveTreasure(nt,i);
	}
}

void Model::StartGame(){
	plr.resize(totalplayers);
	phaseAdjust.resize(totalplayers);
	phaseAdjust.assign(totalplayers, 0);
	srand(time(NULL));

	doors.reserve(95);
	rd.reserve(95);
	for(int i=0; i<95;i++) doors.push_back(i);
	random_shuffle(doors.begin(),doors.end());

	treasures.reserve(75);
	rt.reserve(75);
	for(int i=95; i<170;i++) treasures.push_back(i);
	random_shuffle(treasures.begin(),treasures.end());

	GiveToAll(4,4);
	plrTurn=0;
	phase=BEGIN;
}
void Model::Lock() const{
	while(badLock){
		SDL_Delay(50);
	}
	badLock=true;
}

Snapshot Model::GetData(int cp) const{
	Lock();
	Snapshot tmp;
	Player pl;
	for (int i=0; i<totalplayers; i++){
		pl.gender=plr[i].gender;
		pl.level=plr[i].level;
		pl.deck[EQUIP]=plr[i].deck[EQUIP];
		pl.deck[DESK]=plr[i].deck[DESK];
		if (cp==i){
			pl.deck[HAND]=plr[i].deck[HAND];
		} else {
			pl.deck[HAND].clear();
			for (std::vector<int>::const_iterator it=plr[i].deck[HAND].begin(); it!=plr[i].deck[HAND].end(); ++it){			
				pl.deck[HAND].push_back(*it < 95 ? -1 : -2);
			}
		}
		tmp.plr.push_back(pl);
		
	}
	badLock=false;
	return tmp;
}

void Model::TryMove(CardPosition from, CardPosition to, int cp){
	Lock();
	std::vector<int>::iterator itF;
	std::vector<int>::iterator itT;
	if (!((from==to)||(from.position>=plr.at(from.playerNumber).deck[from.vectorName].size())||((to.position>=plr.at(to.playerNumber).deck[to.vectorName].size())&&(to.position>0))))
	{
		itF=plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position;
		itT=plr.at(to.playerNumber).deck[to.vectorName].begin()+to.position+1;
	
		if ((from.playerNumber==to.playerNumber)&&(from.vectorName==to.vectorName)) {
			if (to.position<0){
				int a =*itF;
				plr.at(to.playerNumber).deck[to.vectorName].insert(itT,a);
				plr.at(from.playerNumber).deck[from.vectorName].erase(plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position+1);
			} else {
				std::swap(*itF,*(--itT));
			}
		} else {
	
			plr.at(to.playerNumber).deck[to.vectorName].insert(itT,*itF);
			plr.at(from.playerNumber).deck[from.vectorName].erase(itF);
		}
	}
	badLock=false;
}

void Model::EndPhase(Phase phaseToClose, int cp){
	Lock();
	std::vector<int>::iterator it;
	if (phaseToClose==phase) {
		switch (phase) {
		case BEGIN : 
			if (cp==plrTurn) {
								//TODO:place card at table, if it is monster, then go fight
				phase=KICKOPEN;
			}
			break;
		case KICKOPEN :
			phaseAdjust.at(cp)=1;
			it=find(phaseAdjust.begin(),phaseAdjust.end(),0);
			if (it!=phaseAdjust.end()){
				phase=CHARITY;
				phaseAdjust.assign(totalplayers,0);
			}	
			break;
		case COMBAT :
			phaseAdjust.at(cp)=1;
			it=find(phaseAdjust.begin(),phaseAdjust.end(),0);
			if (it!=phaseAdjust.end()){
				
				phaseAdjust.assign(totalplayers,0);
			}
			break;
		case CHARITY :
			
			break;
		case DEATH :
			
			break;
		}
	}
	badLock=false;
}