#include "Munchkin.h"
class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;

	int currplayer, totalplayers;
	
	void StartSettings()
	{
		input = game->GetInput();
		graphics = game->GetGraphics();
	}

	void LoadingImage()
	{

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
