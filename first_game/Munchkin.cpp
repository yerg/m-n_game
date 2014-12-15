#include "Munchkin.h"

class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;

	(Image* card_map)[17];

	int currplayer, totalplayers;
	
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

public:
	void Start()
	{
		StartSettings();
		LoadingImage();
	}
	void Update()
	{

	}
};
