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
	int hi, ei, di;
	int level;
	bool gender;
	Player():hi(0),ei(0),di(0),level(1),gender(true){hand.reserve(10); equip.reserve(10); desk.reserve(10);};
	};
class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;
	(Image* card_map)[17];
	int cp, totalplayers;
	int wW,wH;
	Player plr[2];

	void ShowCard(int id, int x, int y);
	void ShowCurPlayer();
	void ZoomCard(int id);

	void StartSettings();
	void LoadingImage();

	void SomeDraw();
	
	std::vector<int> doors,treasures,rd,rt;
	std::vector<int> monster;
	std::vector<int> helper;	
	
public:
	void Start();
	void Update();
};
#endif