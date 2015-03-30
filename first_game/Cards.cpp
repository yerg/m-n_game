#include "Cards.H"
struct D3872 : public StrategyDefeat{
	void Handle(ModelData*d){

	}
};

struct D2lvl : public StrategyDefeat{
	void Handle(ModelData*d){
		d->plr[d->currentInCombat].level-=2;
		if (d->plr[d->currentInCombat].level<=0) d->plr[d->currentInCombat].level=1;
	}
};

struct ALevelUp : StrategyAct {
	void Handle(ModelData*d, int pl){
		if (d->plr[pl].level<9) ++d->plr[pl].level;
	}
};

struct AChangeClass : StrategyAct {
	void Handle(ModelData*d, int pl){
		std::vector<int>::iterator vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[](const Card &c){return (CLASS|MULTICLASS)&c.CardType();});
		int startPos;
		if (vi!=d->plr[pl].deck[EQUIP].end()) {
			startPos=d->rd.size();
			do {
				d->rd.push_back(*vi);
				d->plr[pl].deck[EQUIP].erase(vi);
				vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[](const Card &c){return (CLASS|MULTICLASS)&c.CardType();} );
			} while (vi!=d->plr[pl].deck[EQUIP].end());
			std::vector<int>::reverse_iterator rvi=std::find_if(d->rd.rbegin()+startPos,d->rd.rend(),[](const Card &c){return (CLASS)&c.CardType();} );
			if (rvi!=d->rd.rend()){
				d->plr[pl].deck[EQUIP].push_back(*rvi);
				d->rd.erase(rvi.base()-1);
			}
		}
	}
};

struct AChangeRace : StrategyAct {
	void Handle(ModelData*d, int pl){
		std::vector<int>::iterator vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[](const Card &c){return (RACE|MULTIRACE)&c.CardType();});
		int startPos;
		if (vi!=d->plr[pl].deck[EQUIP].end()) {
			startPos=d->rd.size();
			do {
				d->rd.push_back(*vi);
				d->plr[pl].deck[EQUIP].erase(vi);
				vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[](const Card &c){return (RACE|MULTIRACE)&c.CardType();} );
			} while (vi!=d->plr[pl].deck[EQUIP].end());
			std::vector<int>::reverse_iterator rvi=std::find_if(d->rd.rbegin()+startPos,d->rd.rend(),[](const Card &c){return (RACE)&c.CardType();} );
			if (rvi!=d->rd.rend()){
				d->plr[pl].deck[EQUIP].push_back(*rvi);
				d->rd.erase(rvi.base()-1);
			}
		}
	}
};

namespace{
	D3872 d3872;
	D2lvl  d2lvl;
	ALevelUp aLevelUp;
	AChangeClass aChangeClass;
	AChangeRace aChangeRace;
}

void StrategyWin::Handle(ModelData*d, int gainCard, int gainLevel){
	if (d->inCombat.size()==1) {
		d->plr[d->currentInCombat].level+=gainLevel;
		GiveTreasure(*d, gainCard , d->currentInCombat);
	}
}


std::map<int, Card> Cards::c;
std::map<int, Card>* Cards::GetMap(ModelData* d){
	Card card(d, &c);
	for (int i=0; i<170; i++){
		c.insert(std::pair<int,Card>(i,card));
	}

	c[0];
	c[1];
	c[2];
	c[3];
	c[4];
	c[5];
	c[6];
	c[7];
	c[8];
	c[9];
	c[10];
	c[11];
	c[12];
	c[13];
	c[14];
	c[15];
	c[16];
	c[17];
	c[18];
	c[19];
	c[20];
	c[21]=aChangeClass; c[21]=CURSE;
	c[22]=aChangeRace; c[22]=CURSE;
	c[23]=CURSE;
	c[24]=CURSE;
	c[25]=CURSE;
	c[26]=CURSE;
	c[27]=CURSE;
	c[28]=CURSE;
	c[29]=CURSE;
	c[30]=CURSE;
	c[31]=CURSE;
	c[32]=CURSE;
	c[33]=CURSE;
	c[34]=CURSE;
	c[35]=CURSE;
	c[36]=CURSE;
	c[37]=CURSE;
	c[38]=CURSE;
	c[39]=CURSE;
	c[40]=new Beast(&d2lvl,10,3);
	c[40]=new Beast(&d3872,10,3);
	c[41]=new Beast(&d2lvl,8,2);
	c[42]=new Beast(&d2lvl,12,3);
	c[43]=new Beast(&d2lvl,18,5,2);
	c[44]=new Beast(&d2lvl,1,1);
	c[45]=new Beast(&d2lvl,1,1);
	c[46]=new Beast(&d2lvl,8,2);
	c[47]=new Beast(&d2lvl,10,3);
	c[48]=new Beast(&d2lvl,2,1);
	c[49]=new Beast(&d2lvl,8,2);
	c[50]=new Beast(&d2lvl,2,1);
	c[51]=new Beast(&d2lvl,8,2);
	c[52]=new Beast(&d2lvl,4,2);
	c[53]=new Beast(&d2lvl,16,4,2);
	c[54]=new Beast(&d2lvl,14,4);
	c[55]=new Beast(&d2lvl,16,4,2,true);
	c[56]=new Beast(&d2lvl,1,1);
	c[57]=new Beast(&d2lvl,2,1);
	c[58]=new Beast(&d2lvl,6,2);
	c[59]=new Beast(&d2lvl,4,2);
	c[60]=new Beast(&d2lvl,1,1);
	c[61]=new Beast(&d2lvl,2,1,1,true);
	c[62]=new Beast(&d2lvl,10,3);
	c[63]=new Beast(&d2lvl,2,1);
	c[64]=new Beast(&d2lvl,6,2);
	c[65]=new Beast(&d2lvl,20,5,2);
	c[66]=new Beast(&d2lvl,1,1);
	c[67]=new Beast(&d2lvl,6,2);
	c[68]=new Beast(&d2lvl,6,2);
	c[69]=new Beast(&d2lvl,4,2);
	c[70]=new Beast(&d2lvl,18,4,2);	
	c[71]=new Beast(&d2lvl,14,4);
	c[72]=new Beast(&d2lvl,12,3);
	c[73]=new Beast(&d2lvl,4,2,1,true);
	c[74]=new Beast(&d2lvl,14,4);
	c[75]=new Beast(&d2lvl,12,3);
	c[76]=new Beast(&d2lvl,16,4,2,true);
	c[77];
	c[78];
	c[79]=MULTIRACE;
	c[80]=MULTIRACE;
	c[81];
	c[82];
	c[83];
	c[84];
	c[85]=MULTICLASS;
	c[86]=MULTICLASS;
	c[87];
	c[88];
	c[89];
	c[90];
	c[91];
	c[92];
	c[93];
	c[94];
	c[95]=aLevelUp; c[95]=PLAYERUSE;
	c[96];
	c[97];
	c[98];
	c[99];
	c[100]=aLevelUp; c[100]=PLAYERUSE;
	c[101]=aLevelUp; c[95]=PLAYERUSE;
	c[102]=aLevelUp; c[95]=PLAYERUSE;
	c[103]=aLevelUp; c[95]=PLAYERUSE;
	c[104]=aLevelUp; c[95]=PLAYERUSE;
	c[105];
	c[106];
	c[107]=aLevelUp; c[95]=PLAYERUSE;
	c[108];
	c[109];
	c[110];
	c[111];
	c[112];
	c[113];
	c[114];
	c[115];
	c[116];
	c[117];
	c[118];
	c[119];
	c[120];
	c[121];
	c[122];
	c[123];
	c[124];
	c[125];
	c[126];
	c[127];
	c[128];
	c[129];
	c[130];
	c[131];
	c[132];
	c[133];
	c[134];
	c[135];
	c[136];
	c[137];
	c[138];
	c[139];
	c[140];
	c[141];
	c[142];
	c[143];
	c[144];
	c[145];
	c[146];
	c[147];
	c[148];
	c[149];
	c[150];
	c[151];
	c[152];
	c[153];
	c[154];
	c[155];
	c[156];
	c[157];
	c[158];
	c[159];
	c[160];
	c[161];
	c[162];
	c[163];
	c[164];
	c[165];
	c[166];
	c[167];
	c[168];
	c[169];

	return &c;
}