#include "Cards.H"
namespace{
	StrategyEscape sEscape;
	StrategyCombat sCombat;
	StrategyPreparation sPreparation;
	StrategyWin sWin;
}

Beast::Beast(ModelData* d, std::map<int, Card*>* map, StrategyDefeat* sd, int level, int gainCard, int gainLevel, bool undead) : Card(d,map), sd(sd), level(level), gainCard(gainCard), gainLevel(gainLevel), undead(undead){
	sc=&sCombat;
	sw=&sWin;
	sp=&sPreparation;
	se=&sEscape;
	cType=BEAST;
}

Cards::Cards(ModelData* d){
	class D3872 : public StrategyDefeat{

	} d3872;
	c[40]=new Beast(d,&c,&d3872,10,3);
}