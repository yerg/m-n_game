#include "Munchkin.h"
const int MAP_W=3305; 
const int MAP_H=2056;

class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;

	(Image* card_map)[17];

	int currplayer, totalplayers;
	void ShowCard(int id, int x, int y){
		graphics->DrawImage(card_map[id/10],x,y,(id%5)*MAP_W/5,(id%10)>4 ? MAP_H/2 : 0,(id%5+1)*MAP_W/5,(id%10)>4 ? MAP_H : MAP_H/2);
	}
	void StartSettings()
	{
		input = game->GetInput();
		graphics = game->GetGraphics();
	}

	void LoadingImage()
	{
		for (int i=0; i<17; i++)
		{
			char filename[20]; char letter[1]={'a'+i};
			strcpy(filename,"res\\");
			strcat(filename,letter);
			strcat(filename,".jpg");
			
			card_map[i]=graphics->NewImage(filename);
		}
		
	}
	void SomeDraw(){
		ShowCard(15,5,5);
		ShowCard(145,600,300);
		graphics->Flip();
	}

public:
	void Start()
	{
		StartSettings();
		LoadingImage();
		SomeDraw();
	}
	void Update()
	{
			if(input->IsExit())
				game->Exit();
	}
};