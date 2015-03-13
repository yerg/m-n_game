#include "Cards.H"
namespace{
	StrategyEscape sEscape;
	StrategyCombat sCombat;
	StrategyPreparation sPreparation;
	StrategyWin sWin;
}

Beast::Beast(ModelData* d, const std::map<int, Card*>* const map, StrategyDefeat* sd, int level, int gainCard, int gainLevel, bool undead) : Card(d,map), sd(sd), level(level), gainCard(gainCard), gainLevel(gainLevel), undead(undead){
	sc=&sCombat;
	sw=&sWin;
	sp=&sPreparation;
	se=&sEscape;
	sType=DOOR;
	cType=BEAST;
}

Cards::Cards(ModelData* d){
	int i;
	struct D3872 : public StrategyDefeat{

	} d3872;
	struct D2lvl : public StrategyDefeat{
		void Handle(ModelData*d){
			d->plr[d->currentInCombat].level-=2;
			if (d->plr[d->currentInCombat].level<=0) d->plr[d->currentInCombat].level=1;
		}
	} d2lvl;
	c[i=40]=new Beast(d,&c,&d3872,10,3); //40
	c[++i]=new Beast(d, &c,&d2lvl,8,2);
	c[++i]=new Beast(d, &c,&d2lvl,12,3);
	c[++i]=new Beast(d, &c,&d2lvl,18,5,2);
	c[++i]=new Beast(d, &c,&d2lvl,1,1);
	c[++i]=new Beast(d, &c,&d2lvl,1,1);
	c[++i]=new Beast(d, &c,&d2lvl,8,2);
	c[++i]=new Beast(d, &c,&d2lvl,10,3);
	c[++i]=new Beast(d, &c,&d2lvl,2,1);
	c[++i]=new Beast(d, &c,&d2lvl,8,2);
	c[++i]=new Beast(d, &c,&d2lvl,2,1);
	c[++i]=new Beast(d, &c,&d2lvl,8,2);
	c[++i]=new Beast(d, &c,&d2lvl,4,2);
	c[++i]=new Beast(d, &c,&d2lvl,16,4,2);
	c[++i]=new Beast(d, &c,&d2lvl,14,4);
	c[++i]=new Beast(d, &c,&d2lvl,16,4,2,true);
	c[++i]=new Beast(d, &c,&d2lvl,1,1);
	c[++i]=new Beast(d, &c,&d2lvl,2,1);
	c[++i]=new Beast(d, &c,&d2lvl,6,2);
	c[++i]=new Beast(d, &c,&d2lvl,4,2);
	c[++i]=new Beast(d, &c,&d2lvl,1,1);
	c[++i]=new Beast(d, &c,&d2lvl,2,1,1,true);
	c[++i]=new Beast(d, &c,&d2lvl,10,3);
	c[++i]=new Beast(d, &c,&d2lvl,2,1);
	c[++i]=new Beast(d, &c,&d2lvl,6,2);
	c[++i]=new Beast(d, &c,&d2lvl,20,5,2);
	c[++i]=new Beast(d, &c,&d2lvl,1,1);
	c[++i]=new Beast(d, &c,&d2lvl,6,2);
	c[++i]=new Beast(d, &c,&d2lvl,6,2);
	c[++i]=new Beast(d, &c,&d2lvl,4,2);
	c[++i]=new Beast(d, &c,&d2lvl,18,4,2);	//70
	c[++i]=new Beast(d, &c,&d2lvl,14,4);
	c[++i]=new Beast(d, &c,&d2lvl,12,3);
	c[++i]=new Beast(d, &c,&d2lvl,4,2,1,true);
	c[++i]=new Beast(d, &c,&d2lvl,14,4);
	c[++i]=new Beast(d, &c,&d2lvl,12,3);
	c[++i]=new Beast(d, &c,&d2lvl,16,4,2,true);	//76
}