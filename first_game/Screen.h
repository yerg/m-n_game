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


class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;
	(Image* card_map)[17];
	int currplayer, totalplayers;
	int wW,wH;
	void ShowCard(int id, int x, int y);
	void ZoomCard(int id);
	void StartSettings();
	void LoadingImage();
	void SomeDraw();
	std::queue<int> doors,treasures,rd,rt;
	std::list<int> monster;
	std::list<int> helper;
	struct Player{
		std::list<int> hand, equip, desk;
		int level;
		bool gender;	
	};
public:
	void Start();
	void Update();
};
#endif