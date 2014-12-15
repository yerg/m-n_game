#include "Project.h"
const int GRID_SIZE = 300;
/*class TicTacToeScreen : public Screen
{
private:
	Input* input;
	Graphics* graphics;

	// Изображения фона, крестиков и ноликов
	Image* back;
	(Image* player)[2];
	// Массив, обозначающий поле, и текущий игрок
	int grid[3][3], currplayer;
	void StartSettings()
	{
		// Берем объекты Graphics и Input и устанавливаем заголовок окна

		input = game->GetInput();
		graphics = game->GetGraphics();
	}

	void LoadingImage()
	{
		// Загрузка изображений

		back = graphics->NewImage("fon.bmp");
		player[0] = graphics->NewImage("X.bmp",255,255,255);
		player[1] = graphics->NewImage("O.bmp",255,255,255);

	}

	void Reset()
	{
		// Очистка поля и вывод фона

		currplayer = 1;

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				grid[i][j] = 0;

		graphics->DrawImage(back,0,0);
		graphics->Flip();
	}

	int GameOver()
	{
		// -1 - игра не окончена; 0 - ничья; 1 - победили крестики; 2 - победили нолики

		// Проверка на чью-нибудь победу
		for(int i = 1; i < 3; i++)
			// По горизонтали
			if( (grid[0][0] == i && grid[1][0] == i && grid[2][0] == i) ||
				(grid[0][1] == i && grid[1][1] == i && grid[2][1] == i) ||
				(grid[0][2] == i && grid[1][2] == i && grid[2][2] == i) ||

				// По вертикали
				(grid[0][0] == i && grid[0][1] == i && grid[0][2] == i) ||
				(grid[1][0] == i && grid[1][1] == i && grid[1][2] == i) ||
				(grid[2][0] == i && grid[2][1] == i && grid[2][2] == i) ||

				// По диагонали
				(grid[0][0] == i && grid[1][1] == i && grid[2][2] == i) ||
				(grid[2][0] == i && grid[1][1] == i && grid[0][2] == i) )
				return i;

		// Проверка на ничью
		int count = 0;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				if(grid[i][j] != 0)
					count++;
		// Заполнено все поле
		if(count == 9)
			return 0;

		return -1;
	}

	void GameOverHandle(int over)
	{
		// Обработка конца игры

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
			// Если игра не окончена, возвращаемся из функции
		default:
			return;
		}
		const SDL_MessageBoxButtonData buttons[] = {
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yep" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "No" },
		};
		const SDL_MessageBoxColorScheme colorScheme = {
			{	
				{ 255,   0,   0 },
				{   0, 255,   0 },
				{ 255, 255,   0 },
				{   0,   0, 255 },
				{ 255,   0, 255 }
			}
		};
		const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION, NULL, "Game over", message,	SDL_arraysize(buttons),	buttons, &colorScheme};

		// Спрашиваем пользователя, хочет ли он сыграть еще раз
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
		// Если нажат красный крестик в окне
		if(input->IsExit())
			game->Exit();

		// Если нажата левая кнопка мыши
		// P.S. 1 - ЛКМ, 2 - СКМ, 3 - ПКМ
		if(input->IsMouseButtonDown(1))
		{
			// Берем координаты нажатой клетки
			int x = input->GetButtonDownCoords().x / (GRID_SIZE / 3),
				y = input->GetButtonDownCoords().y / (GRID_SIZE / 3);

			// Если мы нажали на пустую клетку
			if(grid[x][y] == 0)
			{
				// Устанавливаем крестик или нолик в клетку
				grid[x][y] = currplayer;

				if(currplayer == 1)
					currplayer = 2;
				else
					currplayer = 1;

				// Рисуем задник
				graphics->DrawImage(back,0,0);

				// Рисуем крестики и нолики
				for(int i = 0; i < 3; i++)
					for(int j = 0; j < 3; j++)
						if(grid[i][j] != 0)
							graphics->DrawImage(player[ grid[i][j]-1 ], i * (GRID_SIZE / 3), j * (GRID_SIZE / 3));

				// Выводим на экран
				graphics->Flip();

				// Обрабатываем конец игры
				GameOverHandle(GameOver());
			}
		}
	}
};*/
int main(int argc, char* argv[])
{
	Game game;

	return game.Execute(new Munchkin(),1200,600,"Munchkin");
	std::cout<<IMG_GetError()<<std::endl<<SDL_GetError();
}

