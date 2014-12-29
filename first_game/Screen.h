#ifndef SCREEN_H
#define SCREEN_H

#include "Project.h"

#include "Game.h"
class Game;
#include "Image.h"
class Image;
#include "Graphics.h"
class Graphics;

class Screen
{
protected:
	Game* game;
public:
	void SetController(Game* game);

	virtual void Start();
	virtual void Update();
	virtual void Destroy();
};



class Player{
public:
	std::vector<int> hand, equip, desk;
	unsigned int hi, ei, di;
	int level;
	bool gender;
	Player():hi(0),ei(0),di(0),level(1),gender(true){hand.reserve(10); equip.reserve(10); desk.reserve(10);};
};
typedef std::pair<std::vector<int>::iterator, std::pair<int,int>> pWMap;
typedef std::map<std::vector<int>::iterator, std::pair<int,int>> mWMap;

class Munchkin : public Screen
{
	struct FindCard{
		FindCard(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h){}
		bool operator()(const pWMap &a) const;
	private:
		int x_, y_, w_, h_;
	};
private:
	Input* input;
	Graphics* graphics;
	(Image* card_map)[17];
	Image* back;
	int cp, ep, totalplayers, zoomed;

	int wW,wH, cW, cH, mapW, mapH;
	double cardRatio;

	Player plr[2];

	void ShowCard(int id, int x, int y);
	void ShowBack(int id, int x, int y);
	void ZoomCard(int id);

	void FillMap();
	void FillLine(std::vector<int> &, unsigned int, double, mWMap &);
	void ShowMap();

	void StartSettings();
	void LoadingImage();

	void ReDraw();
	
	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;
	std::vector<int>::iterator iToMove;
	mWMap windowMap, backMap;
	
public:
	void Start();
	void Update();
};
#endif