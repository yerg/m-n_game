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

void Model::GiveDoor(int n, int pl){
	while (n){
		d.plr[pl].deck[0].push_back(d.doors.back());
		d.doors.pop_back();
		--n;
		if (d.doors.size()==1) {
			d.doors.insert(d.doors.end(),d.rd.begin(),d.rd.end());
			std::random_shuffle(d.doors.begin(),d.doors.end());
			d.rd.clear();
		}
	}
}

void Model::GiveTreasure(int n, int pl){
	while (n){
		d.plr[pl].deck[0].push_back(d.treasures.back());
		d.treasures.pop_back();
		--n;
		if (d.treasures.size()==1) {
			d.treasures.insert(d.treasures.end(),d.rt.begin(),d.rt.end());
			std::random_shuffle(d.treasures.begin(),d.treasures.end());
			d.rt.clear();
		}
	}
}
void Model::GiveToAll(int nd, int nt){
	for (int i=FIRSTPLAYER; i<d.totalplayers; i++) {
		GiveDoor(nd,i);
		GiveTreasure(nt,i);
	}
}

void Model::StartGame(int n){
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
	d.phase=BEGIN;
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
	return tmp;
}

void Model::TryMove(CardPosition from, CardPosition to, int cp){
	std::vector<int>::iterator itF;
	std::vector<int>::iterator itT;
	if ((from==to)||(from.position>=d.plr.at(from.playerNumber).deck[from.vectorName].size())||((to.position>=d.plr.at(to.playerNumber).deck[to.vectorName].size())&&(to.position>0))) return;
	{
		itF=d.plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position;
		itT=d.plr.at(to.playerNumber).deck[to.vectorName].begin()+to.position+1;
	
		if ((from.playerNumber==to.playerNumber)&&(from.vectorName==to.vectorName)) {
			if (to.position<0){
				int a =*itF;
				d.plr.at(to.playerNumber).deck[to.vectorName].insert(itT,a);
				d.plr.at(from.playerNumber).deck[from.vectorName].erase(d.plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position+1);
			} else {
				std::swap(*itF,*(--itT));
			}
		} else {
	
			d.plr.at(to.playerNumber).deck[to.vectorName].insert(itT,*itF);
			d.plr.at(from.playerNumber).deck[from.vectorName].erase(itF);
		}
	}
}

void Model::EndPhase(Phase phaseToClose, int cp){
	std::vector<int>::iterator it;
	if (phaseToClose==d.phase) {
		switch (d.phase) {
		case BEGIN : 
			if (cp==d.plrTurn) {
				
				//TODO:place card at table, if it is monster, then go fight
				d.phase=KICKOPEN;
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
		case DEATH :
			
			break;
		}
	}
}