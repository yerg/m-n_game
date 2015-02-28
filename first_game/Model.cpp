#include "Model.H"

void Model::GiveCard(int nd, int nt, int pl){
	while (nd){
		plr[pl].deck[0].push_back(doors.back());
		doors.pop_back();
		--nd;
	}
	while (nt){
		plr[pl].deck[0].push_back(treasures.back());
		treasures.pop_back();
		--nt;
	}
}
void Model::GiveToAll(int nd, int nt){
	for (int i=0; i<totalplayers; i++) GiveCard(nd,nt,i);
}

void Model::StartGame(){
	plr.resize(totalplayers);
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
}

Snapshot Model::GetData(int cp){
	Snapshot tmp;
	Player pl;
	tmp.helper=helper;
	tmp.monster=monster;
	for (int i=0; i<totalplayers; i++){
		pl.gender=plr[i].gender;
		pl.level=plr[i].level;
		pl.deck[EQUIP]=plr[i].deck[EQUIP];
		pl.deck[DESK]=plr[i].deck[DESK];
		if (cp==i){
			pl.deck[HAND]=plr[i].deck[HAND];
		} else {
			pl.deck[HAND].clear();
			for (std::vector<int>::iterator it=plr[i].deck[HAND].begin(); it!=plr[i].deck[HAND].end(); ++it){			
				pl.deck[HAND].push_back(*it < 95 ? -1 : -2);
			}
		}
		tmp.plr.push_back(pl);
		
	}
	return tmp;
}

void Model::TryMove(CardPosition from, CardPosition to){
	std::vector<int>::iterator itF=plr.at(from.playerNumber).deck[from.vectorName].begin()+from.position;
	std::vector<int>::iterator itT=plr.at(to.playerNumber).deck[to.vectorName].begin()+to.position;

	if ((from.playerNumber==to.playerNumber)&&(from.vectorName==to.vectorName)) {
		std::swap(*itF,*itT);
	} else {
		plr.at(to.playerNumber).deck[to.vectorName].insert(itT,*itF);
		plr.at(from.playerNumber).deck[from.vectorName].erase(itF);
	}
}
