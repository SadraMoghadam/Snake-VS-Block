#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <SDL2/SDL_image.h>

using namespace std;

const int Screen_Width = 300;
const int Screen_Height = 570;

const int Max_Ball_Num = 30;
const int Blocks_Num = 6;
const int Extra_Balls_Num = 6;
const int Number_of_Blocks_Row = 3;
const int Number_of_Extra_Balls_Row = 3;
const int R = 5;
const int r = 7;
const int Block_Side = 49;


SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;
TTF_Font *Font = NULL;
SDL_Texture *Texture;

void Null_Blocks(int Blocks[Blocks_Num])
{
	for(int i = 0 ; i < Blocks_Num ; i++)
	{
		Blocks[i] = 0;
	}
}

void Null_Extra_Balls(int Extra_Balls[Extra_Balls_Num])
{
	for(int i = 0 ; i < Extra_Balls_Num ; i++)
	{
		Extra_Balls[i] = 0;
	}
}

void Null_Y_Position_Blocks(int Y_Position_Blocks[Number_of_Blocks_Row])
{ 
	for(int i = 0 ; i < Number_of_Blocks_Row ; i++)
	{
		Y_Position_Blocks[i] = 0;
	}
}

void Null_Y_Position_Extra_Balls(int Y_Position_Extra_Balls[Number_of_Extra_Balls_Row])
{
	for(int i = 0 ; i < Number_of_Extra_Balls_Row ; i++)
	{
		Y_Position_Extra_Balls[i] = 25;
	}
}

void New_Blocks(int Blocks[Blocks_Num] , int Blocks_Number)
{
	int Blocks_Active , Block_Position , Blocks_Numb;
	Null_Blocks(Blocks);
	Blocks_Active = rand();
	Blocks_Active = Blocks_Active % 6;
	for(int i = 0 ; i < Blocks_Active ; i++)
	{
		Block_Position = rand();
		Block_Position = Block_Position % 6;
		while(Blocks[Block_Position] != 0)
		{
			Block_Position = rand();
			Block_Position = Block_Position % 6;
		}
		Blocks_Numb = rand();
		Blocks_Numb = Blocks_Numb % (Blocks_Number - 1);
		Blocks_Numb++;
		Blocks[Block_Position] = Blocks_Numb;
	}
}

void New_Extra_Balls(int Extra_Balls[Extra_Balls_Num] , int Extra_Balls_Number)
{
	int Extra_Ball_Position , Extra_Balls_Numb;
	Null_Extra_Balls(Extra_Balls);
	Extra_Ball_Position = rand();
	Extra_Ball_Position = Extra_Ball_Position % 6;
	Extra_Balls_Numb = rand();
	Extra_Balls_Numb = Extra_Balls_Numb % (Extra_Balls_Number - 1);
	Extra_Balls_Numb++;
	Extra_Balls[Extra_Ball_Position] = Extra_Balls_Numb;
}

void Draw_Rectangle(int X_Position , int Y_Position , int Width , int Height , int Red , int Green , int Blue)
{
	SDL_Rect fillRect = {X_Position , Y_Position , Width , Height};
	SDL_SetRenderDrawColor(Renderer, Red , Green , Blue , 0xFF);
	SDL_RenderFillRect(Renderer, &fillRect);
	SDL_RenderPresent(Renderer);
}

void Draw_Circle(int X_Position , int Y_Position , int R , int Red , int Green , int Blue)
{
	filledCircleRGBA(Renderer, X_Position , Y_Position , R , Red , Green , Blue , 0xFF);
	SDL_RenderPresent(Renderer);
}

int Initialize_Balls_Active(int Balls_Active[Max_Ball_Num])
{
	for(int i = 0 ; i < Max_Ball_Num ; i++)
	{
		Balls_Active[i] = 0;
	}
}

int Initialize_Blocks_Active(int Blocks_Active[Number_of_Blocks_Row])
{
	for(int i = 0 ; i < Number_of_Blocks_Row ; i++)
	{
		Blocks_Active[i] = 0;
	}
}

int Initialize_Extra_Balls_Active(int Extra_Balls_Active[Number_of_Extra_Balls_Row])
{
	for(int i = 0 ; i < Number_of_Extra_Balls_Row ; i++)
	{
		Extra_Balls_Active[i] = 0;
	}
}

int Initialize_X_Position_Ball(int X_Position_Ball[Max_Ball_Num] , int X)
{
	for(int i = 0 ; i < Max_Ball_Num ; i++)
	{
		X_Position_Ball[i] = X;
	}
}

int Initialize_Y_Position_Ball(int Y_Position_Ball[Max_Ball_Num] , int Y)
{
	for(int i = 0 ; i < Max_Ball_Num ; i++)
	{
		Y_Position_Ball[i] = Y;
		Y = Y + (2 * R);
	}
}

bool Touch_Block(int i , int X_Position_Ball)
{
	if((X_Position_Ball + R > (((Block_Side + 1) * i) + 1)) && (X_Position_Ball + R < (((Block_Side + 1) * i) + 1) + Block_Side))
	{
		return true;
	}
	return false;
}

bool Touch_Extra_Ball(int i , int Y_Position_Extra_Ball , int X_Position_Ball)
{
	double distance_Square;
	distance_Square = (((50 * i) + 25) - X_Position_Ball) * (((50 * i) + 25) - X_Position_Ball) + (Y_Position_Extra_Ball - ((Screen_Height / 2) + 50)) * (Y_Position_Extra_Ball - ((Screen_Height / 2) + 50));
	if(distance_Square <= ((r + R) * (r + R)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Making_String(char String[5] , int Number)
{
	char C;
	int i = 0;
	while(Number != 0)
	{
		String[i] = (Number % 10) + '0';
		Number = Number / 10;
		i++;
	}
	String[i] = '\0';

	int r = 0;
	int s = i - 1;

	while(r < s)
	{
		C = String[r];
		String[r] = String[s];
		String[s] = C;
		r++;
		s--;
	}
}

void Draw_Text(int Number , int X_Position , int Y_Position , int Width , int Height , int Red , int Green , int Blue)
{
	char String[5];
	Making_String(String , Number);
	TTF_Init();
	Font = TTF_OpenFont("Font.ttf" , 100);
	SDL_Color Text_Color = {0 , 0 , 0};
	SDL_Surface *Text_Surface = TTF_RenderText_Solid(Font , String , Text_Color);
	Texture = SDL_CreateTextureFromSurface(Renderer , Text_Surface);
	SDL_SetRenderDrawColor(Renderer , Red , Green , Blue , 0xFF);
	SDL_Rect renderQuad = {X_Position , Y_Position , Width , Height};
	SDL_RenderCopyEx(Renderer , Texture , NULL , &renderQuad , 0.0 , NULL , SDL_FLIP_NONE);
	SDL_RenderPresent(Renderer);
	TTF_CloseFont(Font);
}

void Ball_Text(int X_Position , int Y_Position , int Width , int Height)
{
	TTF_Init();
	Font = TTF_OpenFont("Font.ttf" , 50);
	SDL_Color Text_Color = {255 , 255 , 255};
	SDL_Surface *Text_Surface = TTF_RenderText_Solid(Font , "Ball: " , Text_Color);
	Texture = SDL_CreateTextureFromSurface(Renderer , Text_Surface);
	SDL_SetRenderDrawColor(Renderer , 255 , 255 , 255 , 0xFF);
	SDL_Rect renderQuad = {X_Position , Y_Position , Width , Height};
	SDL_RenderCopyEx(Renderer , Texture , NULL , &renderQuad , 0.0 , NULL , SDL_FLIP_NONE);
	SDL_RenderPresent(Renderer);
}

void Ball_Number_Text(int Number , int X_Position , int Y_Position , int Width , int Height)
{
	char String[5];
	Making_String(String , Number);
	Draw_Rectangle(X_Position , Y_Position , Width , Height , 0 , 0 , 0);
	TTF_Init();
	Font = TTF_OpenFont("Font.ttf" , 50);
	SDL_Color Text_Color = {255 , 255 , 255};
	SDL_Surface *Text_Surface = TTF_RenderText_Solid(Font , String , Text_Color);
	Texture = SDL_CreateTextureFromSurface(Renderer , Text_Surface);
	SDL_SetRenderDrawColor(Renderer , 255 , 255 , 255 , 0xFF);
	SDL_Rect renderQuad = {X_Position , Y_Position , Width , Height};
	SDL_RenderCopyEx(Renderer , Texture , NULL , &renderQuad , 0.0 , NULL , SDL_FLIP_NONE);
	SDL_RenderPresent(Renderer);
}

void Score_Text(int X_Position , int Y_Position , int Width , int Height)
{
	TTF_Init();
	Font = TTF_OpenFont("Font.ttf" , 50);
	SDL_Color Text_Color = {255 , 255 , 255};
	SDL_Surface *Text_Surface = TTF_RenderText_Solid(Font , "Score: " , Text_Color);
	Texture = SDL_CreateTextureFromSurface(Renderer , Text_Surface);
	SDL_SetRenderDrawColor(Renderer , 255 , 255 , 255 , 0xFF);
	SDL_Rect renderQuad = {X_Position , Y_Position , Width , Height};
	SDL_RenderCopyEx(Renderer , Texture , NULL , &renderQuad , 0.0 , NULL , SDL_FLIP_NONE);
	SDL_RenderPresent(Renderer);
}

void Score_Number_Text(int Number , int X_Position , int Y_Position , int Width , int Height)
{
	char String[5];
	Making_String(String , Number);
	Draw_Rectangle(X_Position , Y_Position , Width , Height , 0 , 0 , 0);
	TTF_Init();
	Font = TTF_OpenFont("Font.ttf" , 50);
	SDL_Color Text_Color = {255 , 255 , 255};
	SDL_Surface *Text_Surface = TTF_RenderText_Solid(Font , String , Text_Color);
	Texture = SDL_CreateTextureFromSurface(Renderer , Text_Surface);
	SDL_SetRenderDrawColor(Renderer , 255 , 255 , 255 , 0xFF);
	SDL_Rect renderQuad = {X_Position , Y_Position , Width , Height};
	SDL_RenderCopyEx(Renderer , Texture , NULL , &renderQuad , 0.0 , NULL , SDL_FLIP_NONE);
	SDL_RenderPresent(Renderer);
}

int main()
{
	srand(time(0));
	int Balls_Active[Max_Ball_Num];
	int Blocks_Active[Number_of_Blocks_Row];
	int Extra_Balls_Active[Number_of_Extra_Balls_Row];
	int X_Position_Ball[Max_Ball_Num];
	int Y_Position_Ball[Max_Ball_Num];
	int Blocks_1[Blocks_Num];
	int Blocks_2[Blocks_Num];
	int Blocks_3[Blocks_Num];
	int Extra_Balls_1[Blocks_Num];
	int Extra_Balls_2[Blocks_Num];
	int Extra_Balls_3[Blocks_Num];
	int Y_Position_Blocks[Number_of_Blocks_Row];
	int Y_Position_Extra_Balls[Number_of_Extra_Balls_Row];

	const int Vx = 6;
	const int Vy = 5;
	const int Full_Color = 255;
	const int Null_Color = 0;
	int Hardness = 5;
	int First_Balls_Active = 4;
	int Score = 1;
	int X = 0;
	bool Quit = false;
	bool Show = true;

	Initialize_Balls_Active(Balls_Active);
	Initialize_Blocks_Active(Blocks_Active);
	Initialize_Extra_Balls_Active(Extra_Balls_Active);
	Initialize_X_Position_Ball(X_Position_Ball , Screen_Width / 2);
	Initialize_Y_Position_Ball(Y_Position_Ball , (Screen_Height / 2) + 50);

	Null_Blocks(Blocks_1);
	New_Blocks(Blocks_1 , First_Balls_Active + Hardness);

	Null_Blocks(Blocks_2);
	New_Blocks(Blocks_2 , First_Balls_Active + Hardness);

	Null_Blocks(Blocks_3);
	New_Blocks(Blocks_3 , First_Balls_Active + Hardness);

	Null_Extra_Balls(Extra_Balls_1);
	New_Extra_Balls(Extra_Balls_1 , 2);

	Null_Extra_Balls(Extra_Balls_2);
	New_Extra_Balls(Extra_Balls_2 , 2);

	Null_Extra_Balls(Extra_Balls_3);
	New_Extra_Balls(Extra_Balls_3 , 2);

	Null_Y_Position_Blocks(Y_Position_Blocks);
	Null_Y_Position_Extra_Balls(Y_Position_Extra_Balls);

	Blocks_Active[0] = 1;
	for(int i = 0 ; i < First_Balls_Active ; i++)
	{
		Balls_Active[i] = 1;
	}

	SDL_Event e;

	Window = SDL_CreateWindow("Project S", SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED , Screen_Width , Screen_Height , SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1 , SDL_RENDERER_ACCELERATED);

	while(!Quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				Quit = true;
			}
		}
		if(Show == true)
		{
			for(int i = 0 ; i < First_Balls_Active ; i++)
			{
				Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Full_Color , Full_Color , Full_Color);
			}
			/*for(int i = First_Balls_Active ; i < Max_Ball_Num ; i++)
			{
				Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Null_Color , Null_Color , Null_Color);
			}*/
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_1[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
					Draw_Text(Blocks_1[i] , (((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
				}
				if(Blocks_1[i] == 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
				}
			}
			Draw_Rectangle(0 , 500 + Vy , Screen_Width , 5 , Full_Color , Full_Color , Full_Color);
			Ball_Text(5 , 520 , 70 , 40);
			Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
			Score_Text(Screen_Width - 100 , 520 , 70 , 40);
			Score_Number_Text(Score , Screen_Width - 25 , 520 , 20 , 40);
		}
		Show = false;
		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_LEFT:
				X_Position_Ball[0] = X_Position_Ball[0] - Vx;
				Draw_Circle((X_Position_Ball[0] + Vx) , Y_Position_Ball[0] , R , Null_Color , Null_Color , Null_Color);
				Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
				SDL_Delay(5);
				break;

				case SDLK_RIGHT:
				X_Position_Ball[0] = X_Position_Ball[0] + Vx;
				Draw_Circle((X_Position_Ball[0] - Vx) , Y_Position_Ball[0] , R , Null_Color , Null_Color , Null_Color);
				Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
				SDL_Delay(5);
				break;
			}
		}
		for(int i = 1 ; i < Max_Ball_Num ; i++)
		{
			if(Balls_Active[i] == 1)
			{
				if(X_Position_Ball[i - 1] != X_Position_Ball[i])
				{
					Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Null_Color , Null_Color , Null_Color);
					X_Position_Ball[i] = (X_Position_Ball[i - 1] + X_Position_Ball[i]) / 2;
					Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Full_Color , Full_Color , Full_Color);
					SDL_Delay(3);
				}
			}
		}

		if(((Screen_Height / 2) + 50 - R < Y_Position_Blocks[0] + Block_Side) && (((Screen_Height / 2) + 50 - R > Y_Position_Blocks[0])))
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_1[i] > 0)
				{
					if(Touch_Block(i , X_Position_Ball[0]) == true)
					{
						X = Blocks_1[i];
						Blocks_1[i] = Blocks_1[i] - First_Balls_Active;
						First_Balls_Active = First_Balls_Active - X;
						Score = Score + X;
						if(First_Balls_Active <= 0)
						{
							return 0;
						}
						for(int i = First_Balls_Active + X - 1 ; i >= First_Balls_Active ; i--)
						{
							Balls_Active[i] = 0;
							Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Null_Color , Null_Color , Null_Color);
						}
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
						Draw_Rectangle(Screen_Width - 25 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Score_Number_Text(Score , Screen_Width - 25 , 520 , 20 , 40);
					}
				}
			}
		}

		if(((Screen_Height / 2) + 50 - R < Y_Position_Blocks[1] + Block_Side) && (((Screen_Height / 2) + 50 - R > Y_Position_Blocks[1])))
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_2[i] > 0)
				{
					if(Touch_Block(i , X_Position_Ball[0]) == true)
					{
						X = Blocks_2[i];
						Blocks_2[i] = Blocks_2[i] - First_Balls_Active;
						First_Balls_Active = First_Balls_Active - X;
						Score = Score + X;
						if(First_Balls_Active <= 0)
						{
							return 0;
						}
						for(int i = First_Balls_Active + X - 1 ; i >= First_Balls_Active ; i--)
						{
							Balls_Active[i] = 0;
							Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Null_Color , Null_Color , Null_Color);
						}
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
						Draw_Rectangle(Screen_Width - 25 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Score_Number_Text(Score , Screen_Width - 25 , 520 , 20 , 40);
					}
				}
			}
		}

		if(((Screen_Height / 2) + 50 - R < Y_Position_Blocks[2] + Block_Side) && (((Screen_Height / 2) + 50 - R > Y_Position_Blocks[2])))
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_3[i] > 0)
				{
					if(Touch_Block(i , X_Position_Ball[0]) == true)
					{
						X = Blocks_3[i];
						Blocks_3[i] = Blocks_3[i] - First_Balls_Active;
						First_Balls_Active = First_Balls_Active - X;
						Score = Score + X;
						if(First_Balls_Active <= 0)
						{
							return 0;
						}
						for(int i = First_Balls_Active + X - 1 ; i >= First_Balls_Active ; i--)
						{
							Balls_Active[i] = 0;
							Draw_Circle(X_Position_Ball[i] , Y_Position_Ball[i] , R , Null_Color , Null_Color , Null_Color);
						}
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
						Draw_Rectangle(Screen_Width - 25 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Score_Number_Text(Score , Screen_Width - 25 , 520 , 20 , 40);
					}
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////

		if(((Screen_Height / 2) + 50 - R < Y_Position_Extra_Balls[0] + r) && (((Screen_Height / 2) + 50 - R > Y_Position_Extra_Balls[0] - r)))
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_1[i] > 0)
				{
					if(Touch_Extra_Ball(i , Y_Position_Extra_Balls[0] , X_Position_Ball[0]) == true)
					{
						Extra_Balls_1[i] = 0;
						Balls_Active[First_Balls_Active] = 1;
						X_Position_Ball[First_Balls_Active] = X_Position_Ball[First_Balls_Active - 1];
						Draw_Circle(X_Position_Ball[First_Balls_Active] , Y_Position_Ball[First_Balls_Active] , R , Full_Color , Full_Color , Full_Color);
						First_Balls_Active++;
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[0] , r , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
					}
				}
			}
		}

		if(((Screen_Height / 2) + 50 - R < Y_Position_Extra_Balls[1] + r) && (((Screen_Height / 2) + 50 - R > Y_Position_Extra_Balls[1] - r)))
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_2[i] > 0)
				{
					if(Touch_Extra_Ball(i , Y_Position_Extra_Balls[1] , X_Position_Ball[0]) == true)
					{
						Extra_Balls_2[i] = 0;
						Balls_Active[First_Balls_Active] = 1;
						X_Position_Ball[First_Balls_Active] = X_Position_Ball[First_Balls_Active - 1];
						Draw_Circle(X_Position_Ball[First_Balls_Active] , Y_Position_Ball[First_Balls_Active] , R , Full_Color , Full_Color , Full_Color);
						First_Balls_Active++;
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[1] , r , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
					}
				}
			}
		}

		/*if(((Screen_Height / 2) + 50 - R < Y_Position_Extra_Balls[2] + r) && (((Screen_Height / 2) + 50 - R > Y_Position_Extra_Balls[2] - r)))
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_3[i] > 0)
				{
					if(Touch_Extra_Ball(i , Y_Position_Extra_Balls[2] , X_Position_Ball[0]) == true)
					{
						Extra_Balls_3[i] = 0;
						Balls_Active[First_Balls_Active] = 1;
						X_Position_Ball[First_Balls_Active] = X_Position_Ball[First_Balls_Active - 1];
						Draw_Circle(X_Position_Ball[First_Balls_Active] , Y_Position_Ball[First_Balls_Active] , R , Full_Color , Full_Color , Full_Color);
						First_Balls_Active++;
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[2] , r , Null_Color , Null_Color , Null_Color);
						Draw_Circle(X_Position_Ball[0] , Y_Position_Ball[0] , R , Full_Color , Full_Color , Full_Color);
						Draw_Rectangle(80 , 520 , 20 , 40 , Null_Color , Null_Color , Null_Color);
						Ball_Number_Text(First_Balls_Active , 80 , 520 , 20 , 40);
					}
				}
			}
		}*/


		for(int i = 0 ; i < Blocks_Num ; i++)
		{
			if(Blocks_Active[0] == 1)
			{
				if(Blocks_1[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Vy , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Blocks_Active[1] == 1)
			{
				if(Blocks_2[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Vy , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Blocks_Active[2] == 1)
			{
				if(Blocks_3[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Vy , Null_Color , Null_Color , Null_Color);
				}
			}
		}

		for(int i = 0 ; i < Extra_Balls_Num ; i++)
		{
			if(Extra_Balls_Active[0] == 1)
			{
				if(Extra_Balls_1[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[0] , r , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Extra_Balls_Active[1] == 1)
			{
				if(Extra_Balls_2[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[1] , r , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Extra_Balls_Active[2] == 1)
			{
				if(Extra_Balls_3[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[2] , r , Null_Color , Null_Color , Null_Color);
				}
			}
		}


		if(Blocks_Active[0] == 1)
		{
			Y_Position_Blocks[0] = Y_Position_Blocks[0] + Vy;
		}
		if(Blocks_Active[1] == 1)
		{
			Y_Position_Blocks[1] = Y_Position_Blocks[1] + Vy;
		}
		if(Blocks_Active[2] == 1)
		{
			Y_Position_Blocks[2] = Y_Position_Blocks[2] + Vy;
		}


		if(Extra_Balls_Active[0] == 1)
		{
			Y_Position_Extra_Balls[0] = Y_Position_Extra_Balls[0] + Vy;
		}
		if(Extra_Balls_Active[1] == 1)
		{
			Y_Position_Extra_Balls[1] = Y_Position_Extra_Balls[1] + Vy;
		}
		if(Extra_Balls_Active[2] == 1)
		{
			Y_Position_Extra_Balls[2] = Y_Position_Extra_Balls[2] + Vy;
		}



		if(Y_Position_Blocks[0] > 100)
		{
			Extra_Balls_Active[0] = 1;
		}
		if(Y_Position_Blocks[0] > 150)
		{
			Blocks_Active[1] = 1;
		}
		if(Y_Position_Blocks[1] > 100)
		{
			Extra_Balls_Active[1] = 1;
		}
		if(Y_Position_Blocks[1] > 150)
		{
			Blocks_Active[2] = 1;
		}

		if(Y_Position_Blocks[0] > 450)
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_Active[0] == 1)
				{
					if(Blocks_1[i] > 0)
					{
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Blocks[0] = 0;
			New_Blocks(Blocks_1 , First_Balls_Active + Hardness);
		}

		if(Y_Position_Blocks[1] > 450)
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_Active[1] == 1)
				{
					if(Blocks_2[i] > 0)
					{
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Blocks[1] = 0;
			New_Blocks(Blocks_2 , First_Balls_Active + Hardness);
		}

		if(Y_Position_Blocks[2] > 450)
		{
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_Active[2] == 1)
				{
					if(Blocks_3[i] > 0)
					{
						Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Blocks[2] = 0;
			New_Blocks(Blocks_3 , First_Balls_Active + Hardness);
		}

		//////////////////////////
		if(Y_Position_Extra_Balls[0] > 450)
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_Active[0] == 1)
				{
					if(Extra_Balls_1[i] > 0)
					{
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[0] , r , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Extra_Balls[0] = 0;
			New_Extra_Balls(Extra_Balls_1 , 2);
		}

		if(Y_Position_Extra_Balls[1] > 450)
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_Active[1] == 1)
				{
					if(Extra_Balls_2[i] > 0)
					{
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[1] , r , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Extra_Balls[1] = 0;
			New_Extra_Balls(Extra_Balls_2 , 2);
		}

		if(Y_Position_Extra_Balls[2] > 450)
		{
			for(int i = 0 ; i < Extra_Balls_Num ; i++)
			{
				if(Extra_Balls_Active[2] == 1)
				{
					if(Extra_Balls_3[i] > 0)
					{
						Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[2] , r , Null_Color , Null_Color , Null_Color);
					}
				}
			}
			Y_Position_Extra_Balls[2] = 0;
			New_Extra_Balls(Extra_Balls_3 , 2);
		}


		for(int i = 0 ; i < Blocks_Num ; i++)
		{
			if(Blocks_Active[0] == 1)
			{
				if(Blocks_1[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
					Draw_Text(Blocks_1[i] , (((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Blocks_Active[1] == 1)
			{
				if(Blocks_2[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
					Draw_Text(Blocks_2[i] , (((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
				}
			}
			if(Blocks_Active[2] == 1)
			{
				if(Blocks_3[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
					Draw_Text(Blocks_3[i] , (((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Block_Side , Null_Color , Null_Color , Null_Color);
				}
			}
		}

		for(int i = 0 ; i < Extra_Balls_Num ; i++)
		{
			if(Extra_Balls_Active[0] == 1)
			{
				if(Extra_Balls_1[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[0] , r , Full_Color , Full_Color , Full_Color);
				}
			}
			if(Extra_Balls_Active[1] == 1)
			{
				if(Extra_Balls_2[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[1] , r , Full_Color , Full_Color , Full_Color);
				}
			}
			if(Extra_Balls_Active[2] == 1)
			{
				if(Extra_Balls_3[i] > 0)
				{
					Draw_Circle(((50 * i) + 25) , Y_Position_Extra_Balls[2] , r , Full_Color , Full_Color , Full_Color);
				}
			}
		}
		SDL_Delay(50);
	}
}
