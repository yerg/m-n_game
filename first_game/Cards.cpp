#include "Cards.H"

void StrategyWin::Handle(ModelData*d, int gainCard, int gainLevel){
	if (d->inCombat.size()==1) {
		d->plr[d->currentInCombat].level+=gainLevel;
		GiveTreasure(*d, gainCard , d->currentInCombat);
	}
}
Card& Card::operator=(const Card& rhs) {
	d=rhs.d;
	cType=rhs.cType;
	map=rhs.map;
	sa=rhs.sa;
	sc=rhs.sc;
	se=rhs.se;
	sw=rhs.sw;
	ss=rhs.ss;
	beast=rhs.beast;
	item=rhs.item;
	return *this;
}
void Card::Win() const {
	if (beast||sw) {
		if (sw) {
			if(beast) {
				sw->Handle(d,beast->GainCard(),beast->GainLevel());
			} else {
				sw->Handle(d,0,0);
			}
		} else {
			if (d->inCombat.size()==1) {
				d->plr[d->currentInCombat].level+=beast->GainLevel();
				GiveTreasure(*d, beast->GainCard(), d->currentInCombat);
			}
		}
	} else throw "StrategyAct bad access";
}
int Card::Combat() const {
	if ((beast)||(item)||(sc)) {
		int i=0;
		if (beast) i+=beast->Level();
		if (item) i+=item->Combat();
		if (sc) i+=sc->Handle(d,map);
		return i;
	} else {
		throw "Combat calculation bad access";
	}

}

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
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		if (d->plr[pl].level<9) ++d->plr[pl].level;
	}
};
struct ALevelDown : StrategyAct {
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		if (d->plr[pl].level>1) --d->plr[pl].level;
	}
};
struct ATwoLevelDown : StrategyAct {
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		if (d->plr[pl].level>3) d->plr[pl].level-=2; else d->plr[pl].level=1;
	}
};
struct AChangeClass : StrategyAct {
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		std::vector<int>::iterator vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(), [map](const int &c){return (CLASS|MULTICLASS)&(*map)[c].CardType();} );
		int startPos;

		if (vi!=d->plr[pl].deck[EQUIP].end()) {
			startPos=d->rd.size();
			do {
				d->rd.push_back(*vi);
				d->plr[pl].deck[EQUIP].erase(vi);
				vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[map](const int &c){return (CLASS|MULTICLASS)&(*map)[c].CardType();} );
			} while (vi!=d->plr[pl].deck[EQUIP].end());

			std::vector<int>::reverse_iterator rvi=std::find_if(d->rd.rbegin()+startPos,d->rd.rend(),[map](const int &c){return CLASS&(*map)[c].CardType();});
			if (rvi!=d->rd.rend()){
				d->plr[pl].deck[EQUIP].push_back(*rvi);
				d->rd.erase(rvi.base()-1);
			}
		}
	}
};
struct AChangeRace : StrategyAct {
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		std::vector<int>::iterator vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[map](const int &c){return (RACE|MULTIRACE)&(*map)[c].CardType();} );
		int startPos;

		if (vi!=d->plr[pl].deck[EQUIP].end()) {
			startPos=d->rd.size();
			do {
				d->rd.push_back(*vi);
				d->plr[pl].deck[EQUIP].erase(vi);
				vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[map](const int &c){return (RACE|MULTIRACE)&(*map)[c].CardType();} );
			} while (vi!=d->plr[pl].deck[EQUIP].end());

			std::vector<int>::reverse_iterator rvi=std::find_if(d->rd.rbegin()+startPos,d->rd.rend(),[map](const int &c){return RACE&(*map)[c].CardType();} );
			if (rvi!=d->rd.rend()){
				d->plr[pl].deck[EQUIP].push_back(*rvi);
				d->rd.erase(rvi.base()-1);
			}
		}
	}
};
struct AChangeSex : StrategyAct {
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		d->plr[pl].gender=!d->plr[pl].gender;
	}
};
struct ALoseYourRace : StrategyAct{
	void Handle(ModelData*d, std::map<int, Card>* const map, int pl){
		std::vector<int>::iterator vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[map](const int &c){return (RACE|MULTIRACE)&(*map)[c].CardType();} );
			while (vi!=d->plr[pl].deck[EQUIP].end()){
				d->rd.push_back(*vi);
				d->plr[pl].deck[EQUIP].erase(vi);
				vi=std::find_if(d->plr[pl].deck[EQUIP].begin(),d->plr[pl].deck[EQUIP].end(),[map](const int &c){return (RACE|MULTIRACE)&(*map)[c].CardType();} );
			}
	}
};

struct CMalignMirror : StrategyCombat {
	int Handle(ModelData*d, std::map<int, Card>* const map){
		int i=0;
		std::for_each(
			d->plr[d->inCombat[d->currentInCombat]].deck[EQUIP].begin(),
			d->plr[d->inCombat[d->currentInCombat]].deck[EQUIP].end(),
			[map,&i](const int &c){
				if ((*map)[c].CardType()&EQUIPPABLE) i-=(*map)[c].Combat();
		});
		return i;
	}
};
struct CHornyHelmet : StrategyCombat {
	int Handle(ModelData*d, std::map<int, Card>* const map){
		return CheckSetting(*d,map,d->inCombat[d->currentInCombat])&ELF ? 2 : 0;
	}
};


std::map<int, Card> Cards::c;
std::map<int, Card>* Cards::GetMap(ModelData* d){
	Card card(d, &c);
	for (int i=0; i<170; i++){
		c.insert(std::pair<int,Card>(i,card));
	}
	for (int i=0; i<95; i++){
		c[i]=DOOR;
	}
	for (int i=95; i<170; i++){
		c[i]=TREASURE;
	}

	c[0]=CLERICCARD;
	c[1]=c[0];
	c[2]=c[0];
	c[3]=DWARFCARD;
	c[4]=c[3];
	c[5]=c[3];
	c[6]=ELFCARD;
	c[7]=c[6];
	c[8]=c[6];
	c[9]=HALFLINGCARD;
	c[10]=c[9];
	c[11]=c[9];
	c[12]=THIEFCARD;
	c[13]=c[12];
	c[14]=c[12];
	c[15]=WARRIORCARD;
	c[16]=c[15];
	c[17]=c[15];
	c[18]=WIZARDCARD;
	c[19]=c[18];
	c[20]=c[18];						//TODO ^^^
	c[21]=new AChangeClass; c[21]=CURSE;
	c[22]=new AChangeRace; c[22]=CURSE;
	c[23]=new AChangeSex; c[23]=new StrategyCombat(5); c[23]=DELETEAFTERCOMBAT; c[23]=CURSE;
	c[24]=CURSE;
	c[25]=new ATwoLevelDown; c[25]=CURSE;
	c[26]=CURSE;
	c[27]=CURSE;
	c[28]=new ALevelDown; c[28]=CURSE;
	c[29]=new ALevelDown; c[29]=CURSE;
	c[30]=CURSE;
	c[31]=CURSE;
	c[32]=CURSE;
	c[33]=CURSE;
	c[34]=CURSE;
	c[35]=CURSE;
	c[36]=CURSE;
	c[37]=new ALoseYourRace; c[37]=CURSE;
	c[38]=CURSE;
	c[39]=CURSE;
	c[40]=new Beast(new D2lvl,10,3);
	c[40]=new Beast(new D3872,10,3);
	c[41]=new Beast(new D2lvl,8,2);
	c[42]=new Beast(new D2lvl,12,3);
	c[43]=new Beast(new D2lvl,18,5,2);
	c[44]=new Beast(new D2lvl,1,1);
	c[45]=new Beast(new D2lvl,1,1);
	c[46]=new Beast(new D2lvl,8,2);
	c[47]=new Beast(new D2lvl,10,3);
	c[48]=new Beast(new D2lvl,2,1);
	c[49]=new Beast(new D2lvl,8,2);
	c[50]=new Beast(new D2lvl,2,1);
	c[51]=new Beast(new D2lvl,8,2);
	c[52]=new Beast(new D2lvl,4,2);
	c[53]=new Beast(new D2lvl,16,4,2);
	c[54]=new Beast(new D2lvl,14,4);
	c[55]=new Beast(new D2lvl,16,4,2,true);
	c[56]=new Beast(new D2lvl,1,1);
	c[57]=new Beast(new D2lvl,2,1);
	c[58]=new Beast(new D2lvl,6,2);
	c[59]=new Beast(new D2lvl,4,2);
	c[60]=new Beast(new D2lvl,1,1);
	c[61]=new Beast(new D2lvl,2,1,1,true);
	c[62]=new Beast(new D2lvl,10,3);
	c[63]=new Beast(new D2lvl,2,1);
	c[64]=new Beast(new D2lvl,6,2);
	c[65]=new Beast(new D2lvl,20,5,2);
	c[66]=new Beast(new D2lvl,1,1);
	c[67]=new Beast(new D2lvl,6,2);
	c[68]=new Beast(new D2lvl,6,2);
	c[69]=new Beast(new D2lvl,4,2);
	c[70]=new Beast(new D2lvl,18,4,2);	
	c[71]=new Beast(new D2lvl,14,4);
	c[72]=new Beast(new D2lvl,12,3);
	c[73]=new Beast(new D2lvl,4,2,1,true);
	c[74]=new Beast(new D2lvl,14,4);
	c[75]=new Beast(new D2lvl,12,3);
	c[76]=new Beast(new D2lvl,16,4,2,true);
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
	c[95]=new ALevelUp; c[95]=PLAYERUSE;
	c[96];
	c[97];
	c[98];
	c[99];
	c[100]=new ALevelUp; c[100]=PLAYERUSE;
	c[101]=new ALevelUp; c[95]=PLAYERUSE;
	c[102]=new ALevelUp; c[95]=PLAYERUSE;
	c[103]=new ALevelUp; c[95]=PLAYERUSE;
	c[104]=new ALevelUp; c[95]=PLAYERUSE;
	c[105];
	c[106];
	c[107]=new ALevelUp; c[95]=PLAYERUSE;
	c[108];
	c[109];
	c[110]=new Item(100,3); c[110]=BATTLEUSE;
	c[111]=new Item(200,5); c[111]=BATTLEUSE;
	c[112]=new Item(100,3); c[112]=BATTLEUSE;
	c[113]=new Item(100,3); c[113]=BATTLEUSE;
	c[114]=new Item(300,5); c[114]=BATTLEUSE;
	c[115]=new Item(200,2); c[115]=BATTLEUSE;
	c[116]=new Item(100,2); c[116]=BATTLEUSE;		//TODO: instant kill
	c[117]=new Item(100,2); c[117]=BATTLEUSE;
	c[118]=new Item(0,5);   c[118]=BATTLEUSE;
	c[119]=new Item(100,2); c[119]=BATTLEUSE;
	c[120]=EQUIPPABLE; c[120]=new Item(400,3,HAT,NOTHUMAN); c[120]=EQUIPPABLE;
	c[121]=EQUIPPABLE; c[121]=new Item(400,2,SHOES);
	c[122]=EQUIPPABLE; c[122]=new Item(800,4,TWOHANDED,NOTELF);
	c[123]=EQUIPPABLE; c[123]=new Item(400,3,ONEHANDED,MALE);
	c[124]=EQUIPPABLE; c[124]=new Item(400,2,ONEHANDED);
	c[125]=EQUIPPABLE; c[125]=new Item(600,3,TWOHANDED|BIG);
	c[126]=EQUIPPABLE; c[126]=new Item(400,3,ONEHANDED,NOTCLERIC);
	c[127]=EQUIPPABLE; c[127]=new Item(600,4,SLOTLESS,NOTTHIEF);
	c[128]=EQUIPPABLE; c[128]=new Item(400,3,ONEHANDED,NOTTHIEF);
	c[129]=EQUIPPABLE; c[129]=new Item(200,1,TWOHANDED);
	c[130]=EQUIPPABLE; c[130]=new Item(400,2,CHEST);
	c[131]=EQUIPPABLE; c[131]=new Item(400,3,ONEHANDED,FEMALE);
	c[132]=EQUIPPABLE; c[132]=new Item(600,4,ONEHANDED,NOTDWARF);
	c[133]=EQUIPPABLE; c[133]=new Item(200,1,HAT);
	c[134]=EQUIPPABLE; c[134]=new Item(600,1,HAT); c[134]=new CHornyHelmet;
	c[135]=EQUIPPABLE; c[135]=new Item(0,3,BIG|TWOHANDED);
	c[136]=EQUIPPABLE; c[136]=new Item(200,1,CHEST);
	c[137]=EQUIPPABLE; c[137]=new Item(400,3,SLOTLESS,NOTHALFLING);
	c[138]=EQUIPPABLE; c[138]=new Item(600,4,ONEHANDED,NOTCLERIC);
	c[139]=EQUIPPABLE; c[139]=new Item(600,3,BIG|CHEST,WIZARD);
	c[140]=EQUIPPABLE; c[140]=new Item(600,3,SLOTLESS,WARRIOR);
	c[141]=EQUIPPABLE; c[141]=new Item(400,3,HAT,NOTWIZARD);
	c[142]=EQUIPPABLE; c[142]=new Item(600,3,ONEHANDED,NOTELF);
	c[143]=EQUIPPABLE; c[143]=new Item(0,1,ONEHANDED);												//TODO: escape
	c[144]=EQUIPPABLE; c[144]=new Item(0,3);														// RIT ??
	c[145]=EQUIPPABLE; c[145]=new Item(600,4,ONEHANDED|BIG,NOTWARRIOR);
	c[146]=EQUIPPABLE; c[146]=new Item(400,3,CHEST,NOTDWARF);
	c[147]=EQUIPPABLE; c[147]=new Item(600,2,SLOTLESS,THIEF);
	c[148]=EQUIPPABLE; c[148]=new Item(200,1,CHEST);
	c[149]=EQUIPPABLE; c[149]=new Item(400,2,ONEHANDED);
	c[150]=EQUIPPABLE; c[150]=new Item(200,1);
	c[151]=EQUIPPABLE; c[151]=new Item(800,5,ONEHANDED,NOTWIZARD);
	c[152]=EQUIPPABLE; c[152]=new Item(400,3,BIG,NOTHALFLING);
	c[153]=EQUIPPABLE; c[153]=new Item(600,4,BIG|TWOHANDED,NOTHUMAN);
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