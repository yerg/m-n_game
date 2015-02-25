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
	totalplayers=2;
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
}

void Model::TryMove(CardPosition from, CardPosition to){

}