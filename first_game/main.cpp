#include "Project.h"
const int GRID_SIZE = 300;
class Munchkin : public Screen
{
private:
	Input* input;
	Graphics* graphics;
	(Image* card_map)[17];
	int currplayer, totalplayers;
	void ShowCard(int id, int x, int y);
	void StartSettings();
	void LoadingImage();
	void SomeDraw();
public:
	void Start();
	void Update();
};
class TicTacToeScreen : public Screen
{
private:
	Input* input;
	Graphics* graphics;

	// ����������� ����, ��������� � �������
	Image* back;
	(Image* player)[2];
	// ������, ������������ ����, � ������� �����
	int grid[3][3], currplayer;
	void StartSettings()
	{
		// ����� ������� Graphics � Input � ������������� ��������� ����

		input = game->GetInput();
		graphics = game->GetGraphics();
	}

	void LoadingImage()
	{
		// �������� �����������

		back = graphics->NewImage("fon.bmp");
		player[0] = graphics->NewImage("X.bmp",255,255,255);
		player[1] = graphics->NewImage("O.bmp",255,255,255);

	}

	void Reset()
	{
		// ������� ���� � ����� ����

		currplayer = 1;

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				grid[i][j] = 0;

		graphics->DrawImage(back,0,0);
		graphics->Flip();
	}

	int GameOver()
	{
		// -1 - ���� �� ��������; 0 - �����; 1 - �������� ��������; 2 - �������� ������

		// �������� �� ���-������ ������
		for(int i = 1; i < 3; i++)
			// �� �����������
			if( (grid[0][0] == i && grid[1][0] == i && grid[2][0] == i) ||
				(grid[0][1] == i && grid[1][1] == i && grid[2][1] == i) ||
				(grid[0][2] == i && grid[1][2] == i && grid[2][2] == i) ||

				// �� ���������
				(grid[0][0] == i && grid[0][1] == i && grid[0][2] == i) ||
				(grid[1][0] == i && grid[1][1] == i && grid[1][2] == i) ||
				(grid[2][0] == i && grid[2][1] == i && grid[2][2] == i) ||

				// �� ���������
				(grid[0][0] == i && grid[1][1] == i && grid[2][2] == i) ||
				(grid[2][0] == i && grid[1][1] == i && grid[0][2] == i) )
				return i;

		// �������� �� �����
		int count = 0;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				if(grid[i][j] != 0)
					count++;
		// ��������� ��� ����
		if(count == 9)
			return 0;

		return -1;
	}

	void GameOverHandle(int over)
	{
		// ��������� ����� ����

		char* message;
	
		switch(over)
		{
		case 0:
			message = "Draw! Again?";
			break;
		case 1:
			message = "X wins! Again?";
			break;
		case 2:
			message = "0 wins! Again?";
			break;
			// ���� ���� �� ��������, ������������ �� �������
		default:
			return;
		}
		const SDL_MessageBoxButtonData buttons[] = {
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yep" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "No" },
		};
		const SDL_MessageBoxColorScheme colorScheme = {
			{	
			/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */			{ 255,   0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */				{   0, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */		{ 255, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */	{   0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */	{ 255,   0, 255 }
			}
		};
		const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION, NULL, "Game over", message,	SDL_arraysize(buttons),	buttons, &colorScheme};

		// ���������� ������������, ����� �� �� ������� ��� ���
		int buttonid;
		SDL_ShowMessageBox(&messageboxdata, &buttonid);
		if(buttonid)
			game->Exit();
		else
			Reset();
	}

public:
	void Start()
	{
		StartSettings();
		LoadingImage();
		Reset();
	}

	void Update()
	{
		// ���� ����� ������� ������� � ����
		if(input->IsExit())
			game->Exit();

		// ���� ������ ����� ������ ����
		// P.S. 1 - ���, 2 - ���, 3 - ���
		if(input->IsMouseButtonDown(1))
		{
			// ����� ���������� ������� ������
			int x = input->GetButtonDownCoords().x / (GRID_SIZE / 3),
				y = input->GetButtonDownCoords().y / (GRID_SIZE / 3);

			// ���� �� ������ �� ������ ������
			if(grid[x][y] == 0)
			{
				// ������������� ������� ��� ����� � ������
				grid[x][y] = currplayer;

				if(currplayer == 1)
					currplayer = 2;
				else
					currplayer = 1;

				// ������ ������
				graphics->DrawImage(back,0,0);

				// ������ �������� � ������
				for(int i = 0; i < 3; i++)
					for(int j = 0; j < 3; j++)
						if(grid[i][j] != 0)
							graphics->DrawImage(player[ grid[i][j]-1 ], i * (GRID_SIZE / 3), j * (GRID_SIZE / 3));

				// ������� �� �����
				graphics->Flip();

				// ������������ ����� ����
				GameOverHandle(GameOver());
			}
		}
	}
};
int main(int argc, char* argv[])
{
	Game game;

	return game.Execute(new Munchkin(),1200,600,"Munchkin");
}
const int MAP_W=3305; 
const int MAP_H=2056;

void Munchkin::ShowCard(int id, int x, int y){
	graphics->DrawImage(card_map[id/10],x,y,(id%5)*MAP_W/5,(id%10)>4 ? MAP_H/2 : 0,(id%5+1)*MAP_W/5,(id%10)>4 ? MAP_H : MAP_H/2);
}
void Munchkin::StartSettings()
{
	input = game->GetInput();
	graphics = game->GetGraphics();
}

void Munchkin::LoadingImage()
{
	for (int i=0; i<17; i++)
	{
		char filename[20]; char letter[2]={'a'+i,'\0'};
		strcpy(filename,"res\\");
		strcat(filename,letter);
		strcat(filename,".jpg");

		card_map[i]=graphics->NewImage(filename);
	}

}
void Munchkin::SomeDraw(){
	ShowCard(15,5,5);
	ShowCard(145,600,300);
	graphics->Flip();
}

void Munchkin::Start()
{
	StartSettings();
	LoadingImage();
	SomeDraw();
}
void Munchkin::Update()
{
	if(input->IsExit())
		game->Exit();
}
