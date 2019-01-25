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
const int Screen_Height = 650;

const int Max_Ball_Num = 30;
const int Blocks_Num = 6;
const int Number_of_Blocks_Row = 3;
const int R = 5;


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

void Null_Y_Position_Blocks(int Y_Position_Blocks[Number_of_Blocks_Row])
{
	for(int i = 0 ; i < Number_of_Blocks_Row ; i++)
	{
		Y_Position_Blocks[i] = 0;
	}
}

void New_Blocks(int Blocks[Blocks_Num] , int Blocks_Number)
{
	int c , Blocks_Active , Block_Position , Blocks_Numb;
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

int main()
{
	srand(time(0));
	int Balls_Active[Max_Ball_Num];
	int Blocks_Active[Number_of_Blocks_Row];
	int X_Position_Ball[Max_Ball_Num];
	int Y_Position_Ball[Max_Ball_Num];
	int Blocks_1[Blocks_Num];
	int Blocks_2[Blocks_Num];
	int Blocks_3[Blocks_Num];
	int Y_Position_Blocks[Number_of_Blocks_Row];

	const int Vx = 6;
	const int Vy = 5;
	const int Full_Color = 255;
	const int Null_Color = 0;
	const int Block_Side = 49;
	int First_Balls_Active = 4;
	bool Quit = false;
	bool Show = true;

	Initialize_Balls_Active(Balls_Active);
	Initialize_Blocks_Active(Blocks_Active);
	Initialize_X_Position_Ball(X_Position_Ball , Screen_Width / 2);
	Initialize_Y_Position_Ball(Y_Position_Ball , (Screen_Height / 2) + 100);
	Null_Blocks(Blocks_1);
	New_Blocks(Blocks_1 , 5);
	Null_Blocks(Blocks_2);
	New_Blocks(Blocks_2 , 5);
	Null_Blocks(Blocks_3);
	New_Blocks(Blocks_3 , 5);
	Null_Y_Position_Blocks(Y_Position_Blocks);

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
			for(int i = 0 ; i < Blocks_Num ; i++)
			{
				if(Blocks_1[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
				}
			}
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


		if(Y_Position_Blocks[0] > 150)
		{
			Blocks_Active[1] = 1;
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
			New_Blocks(Blocks_1 , 5);
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
			New_Blocks(Blocks_2 , 5);
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
			New_Blocks(Blocks_3 , 5);
		}

		for(int i = 0 ; i < Blocks_Num ; i++)
		{
			if(Blocks_Active[0] == 1)
			{
				if(Blocks_1[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[0] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
				}
			}
			if(Blocks_Active[1] == 1)
			{
				if(Blocks_2[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[1] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
				}
			}
			if(Blocks_Active[2] == 1)
			{
				if(Blocks_3[i] > 0)
				{
					Draw_Rectangle((((Block_Side + 1) * i) + 1) , Y_Position_Blocks[2] , Block_Side , Block_Side , Full_Color , Full_Color , Full_Color);
				}
			}
		}
		SDL_Delay(50);
	}
}
