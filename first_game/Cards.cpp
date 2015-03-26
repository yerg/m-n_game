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

namespace{
	StrategyEscape sEscape;
	StrategyCombat sCombat;
	StrategyPreparation sPreparation;
	StrategyWin sWin;
	D3872 d3872;
	D2lvl  d2lvl;
}

void StrategyWin::Handle(ModelData*d, int gainCard, int gainLevel){
	if (d->inCombat.size()==1) {
		d->plr[d->currentInCombat].level+=gainLevel;
		GiveTreasure(*d, gainCard , d->currentInCombat);
	}
}

Beast::Beast(ModelData* d, const std::map<int, Card*>* const map, StrategyDefeat* sd, int level, int gainCard, int gainLevel, bool undead) : Card(d,map), sd(sd), level(level), gainCard(gainCard), gainLevel(gainLevel), undead(undead){
	sc=&sCombat;
	sw=&sWin;
	sp=&sPreparation;
	se=&sEscape;
	cType|BEAST|DOOR;
}

void DivineIntervention::Use(){
											//TODO
}

std::map<int, Card*> Cards::c;
std::map<int, Card*>* Cards::GetMap(ModelData* d){
	c[0]=new Cleric(d,&c);
	c[1]=new Cleric(d,&c);
	c[2]=new Cleric(d,&c);
	c[3]=new Dwarf(d,&c);
	c[4]=new Dwarf(d,&c);
	c[5]=new Dwarf(d,&c);
	c[6]=new Elf(d,&c);
	c[7]=new Elf(d,&c);
	c[8]=new Elf(d,&c);
	c[9]=new Halfling(d,&c);
	c[10]=new Halfling(d,&c);
	c[11]=new Halfling(d,&c);
	c[12]=new Thief(d,&c);
	c[13]=new Thief(d,&c);
	c[14]=new Thief(d,&c);
	c[15]=new Warrior(d,&c);
	c[16]=new Warrior(d,&c);
	c[17]=new Warrior(d,&c);
	c[18]=new Wizard(d,&c);
	c[19]=new Wizard(d,&c);
	c[20]=new Wizard(d,&c);
	c[21]=new Curse(d,&c);
	c[22]=new Curse(d,&c);
	c[23]=new Curse(d,&c);
	c[24]=new Curse(d,&c);
	c[25]=new Curse(d,&c);
	c[26]=new Curse(d,&c);
	c[27]=new Curse(d,&c);
	c[28]=new Curse(d,&c);
	c[29]=new Curse(d,&c);
	c[30]=new Curse(d,&c);
	c[31]=new Curse(d,&c);
	c[32]=new Curse(d,&c);
	c[33]=new Curse(d,&c);
	c[34]=new Curse(d,&c);
	c[35]=new Curse(d,&c);
	c[36]=new Curse(d,&c);
	c[37]=new Curse(d,&c);
	c[38]=new Curse(d,&c);
	c[39]=new Curse(d,&c);
	c[40]=new Beast(d,&c,&d3872,10,3);
	c[41]=new Beast(d, &c,&d2lvl,8,2);
	c[42]=new Beast(d, &c,&d2lvl,12,3);
	c[43]=new Beast(d, &c,&d2lvl,18,5,2);
	c[44]=new Beast(d, &c,&d2lvl,1,1);
	c[45]=new Beast(d, &c,&d2lvl,1,1);
	c[46]=new Beast(d, &c,&d2lvl,8,2);
	c[47]=new Beast(d, &c,&d2lvl,10,3);
	c[48]=new Beast(d, &c,&d2lvl,2,1);
	c[49]=new Beast(d, &c,&d2lvl,8,2);
	c[50]=new Beast(d, &c,&d2lvl,2,1);
	c[51]=new Beast(d, &c,&d2lvl,8,2);
	c[52]=new Beast(d, &c,&d2lvl,4,2);
	c[53]=new Beast(d, &c,&d2lvl,16,4,2);
	c[54]=new Beast(d, &c,&d2lvl,14,4);
	c[55]=new Beast(d, &c,&d2lvl,16,4,2,true);
	c[56]=new Beast(d, &c,&d2lvl,1,1);
	c[57]=new Beast(d, &c,&d2lvl,2,1);
	c[58]=new Beast(d, &c,&d2lvl,6,2);
	c[59]=new Beast(d, &c,&d2lvl,4,2);
	c[60]=new Beast(d, &c,&d2lvl,1,1);
	c[61]=new Beast(d, &c,&d2lvl,2,1,1,true);
	c[62]=new Beast(d, &c,&d2lvl,10,3);
	c[63]=new Beast(d, &c,&d2lvl,2,1);
	c[64]=new Beast(d, &c,&d2lvl,6,2);
	c[65]=new Beast(d, &c,&d2lvl,20,5,2);
	c[66]=new Beast(d, &c,&d2lvl,1,1);
	c[67]=new Beast(d, &c,&d2lvl,6,2);
	c[68]=new Beast(d, &c,&d2lvl,6,2);
	c[69]=new Beast(d, &c,&d2lvl,4,2);
	c[70]=new Beast(d, &c,&d2lvl,18,4,2);	
	c[71]=new Beast(d, &c,&d2lvl,14,4);
	c[72]=new Beast(d, &c,&d2lvl,12,3);
	c[73]=new Beast(d, &c,&d2lvl,4,2,1,true);
	c[74]=new Beast(d, &c,&d2lvl,14,4);
	c[75]=new Beast(d, &c,&d2lvl,12,3);
	c[76]=new Beast(d, &c,&d2lvl,16,4,2,true);
	c[77]=new
	c[78]=new DivineIntervention(d, &c);
	c[79]=new 
	c[80]=new
	c[81]=new
	c[82]=new
	c[83]=new
	c[84]=new
	c[85]=new
	c[86]=new
	c[87]=new
	c[88]=new
	c[89]=new
	c[90]=new
	c[91]=new
	c[92]=new
	c[93]=new
	c[94]=new
	c[95]=new Item(d, &c);
	c[96]=new Item(d, &c);
	c[97]=new Item(d, &c);
	c[98]=new Item(d, &c);
	c[99]=new Item(d, &c);
	c[100]=new Item(d, &c);
	c[101]=new Item(d, &c);
	c[102]=new Item(d, &c);
	c[103]=new Item(d, &c);
	c[104]=new Item(d, &c);
	c[105]=new Item(d, &c);
	c[106]=new Item(d, &c);
	c[107]=new Item(d, &c);
	c[108]=new Item(d, &c);
	c[109]=new Item(d, &c);
	c[110]=new Item(d, &c);
	c[111]=new Item(d, &c);
	c[112]=new Item(d, &c);
	c[113]=new Item(d, &c);
	c[114]=new Item(d, &c);
	c[115]=new Item(d, &c);
	c[116]=new Item(d, &c);
	c[117]=new Item(d, &c);
	c[118]=new Item(d, &c);
	c[119]=new Item(d, &c);
	c[120]=new Item(d, &c);
	c[121]=new Item(d, &c);
	c[122]=new Item(d, &c);
	c[123]=new Item(d, &c);
	c[124]=new Item(d, &c);
	c[125]=new Item(d, &c);
	c[126]=new Item(d, &c);
	c[127]=new Item(d, &c);
	c[128]=new Item(d, &c);
	c[129]=new Item(d, &c);
	c[130]=new Item(d, &c);
	c[131]=new Item(d, &c);
	c[132]=new Item(d, &c);
	c[133]=new Item(d, &c);
	c[134]=new Item(d, &c);
	c[135]=new Item(d, &c);
	c[136]=new Item(d, &c);
	c[137]=new Item(d, &c);
	c[138]=new Item(d, &c);
	c[139]=new Item(d, &c);
	c[140]=new Item(d, &c);
	c[141]=new Item(d, &c);
	c[142]=new Item(d, &c);
	c[143]=new Item(d, &c);
	c[144]=new Item(d, &c);
	c[145]=new Item(d, &c);
	c[146]=new Item(d, &c);
	c[147]=new Item(d, &c);
	c[148]=new Item(d, &c);
	c[149]=new Item(d, &c);
	c[150]=new Item(d, &c);
	c[151]=new Item(d, &c);
	c[152]=new Item(d, &c);
	c[153]=new Item(d, &c);
	c[154]=new Item(d, &c);
	c[155]=new Item(d, &c);
	c[156]=new Item(d, &c);
	c[157]=new Item(d, &c);
	c[158]=new Item(d, &c);
	c[159]=new Item(d, &c);
	c[160]=new Item(d, &c);
	c[161]=new Item(d, &c);
	c[162]=new Item(d, &c);
	c[163]=new Item(d, &c);
	c[164]=new Item(d, &c);
	c[165]=new Item(d, &c);
	c[166]=new Item(d, &c);
	c[167]=new Item(d, &c);
	c[168]=new Item(d, &c);
	c[169]=new Item(d, &c);
	return &c;
}