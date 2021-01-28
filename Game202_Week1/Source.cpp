#include <iostream>
#include <ctime>
#include <SDL.h>
#include <math.h>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;

	//	Our objects
SDL_Rect playerPaddle;
SDL_Rect aiPaddle;
SDL_Rect ball;

SDL_Event event;

int mouse_x, mouse_y;	//	Variables for mouse position

int speed_x, speed_y;	//	Ball speed
int direction[2] = { -1, 1 };	

	//	Some useful consts to adjust the game
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

bool running = true;

bool InitEngine()
{
	bool ReturnVal = true;	//	Value to return in order to avoid multiple Returns in one function

	window = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0 /**SDL_WINDOW_SHOWN**/);

	if (!window)
	{
		cout << "SDL CreateWindow failed" << endl;
		ReturnVal = false;
	}
	else
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (!renderer)
		{
			cout << "SDL renderer cannot be created" << endl;
			ReturnVal = false;
		}
		
		return ReturnVal;
	}
}

//	Stats:
int plScore = 0, aiScore = 0;	//	Score
//int plHits = 0;		//	TBA

void ConsoleUPD()
{
	//	Console score output
	system("CLS");
	cout << endl;
	cout << endl;
	cout << "\t\t\tSCORE:" << endl;
	cout << endl;
	cout << "\t\tYou:\t\tAI:" << endl;
	cout << "\t\t" << plScore << "\t\t" << aiScore << endl;
	cout << endl;
	cout << endl;

	//	Other stats output

	cout << "\tRound \t" << plScore + aiScore + 1 << endl;
	cout << "\tBall speed: \t" << abs(speed_x) << "m/s" << endl;
	//cout << "\tBlocked balls: \t" << plHits << endl;			// TBA
}

bool CheckColision(SDL_Rect A, SDL_Rect B)
{
	bool ReturnValue = true;	//	Returning value, true by default

	//	Sides of each rectanle
	int leftA = A.x, rightA = A.x + A.w, topA = A.y, bottomA = A.y + A.h;
	int leftB = B.x, rightB = B.x + B.w, topB = B.y, bottomB = B.y + B.h;

	//	If any of the sifes from A are outside of B
	if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
	{
		ReturnValue = false;	//	We will return false later
	}
	return ReturnValue;			//	Return true by default or false if there was no collison
}

void LoadGame()
{
		//	Load playerPaddle
	playerPaddle.x = 20;
	playerPaddle.y = SCREEN_HEIGHT / 2 - 50;	//	Placing paddle in the middle
	playerPaddle.w = 20;
	playerPaddle.h = 100;

		//	Load aiPaddle
	aiPaddle.x = SCREEN_WIDTH - 40;	//	Placing paddle 20 px before the right edge
	aiPaddle.y = SCREEN_HEIGHT / 2 - 50;	//	Placing paddle in the middle
	aiPaddle.w = 20;
	aiPaddle.h = 100;

		//	Load ball
	ball.x = 270;
	ball.y = 290;
	ball.w = 20;
	ball.h = 20;
		//	Ball speed
	speed_x = -1;
	speed_y = -1;
}

int Difficulty = 1;	//	Is used to calculate ball color

void Render()
{
	SDL_RenderClear(renderer);

		//	Render BG
	SDL_Rect background = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &background);

		//	Render Middle Line
	SDL_Rect middleLine = { SCREEN_WIDTH / 2 - 3, 40, 6, SCREEN_HEIGHT - 80 };	//	Placing line in the middle
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderFillRect(renderer, &middleLine);

		//	Render playerPaddle
	SDL_SetRenderDrawColor(renderer, 0, 0, 250, 255);
	SDL_RenderFillRect(renderer, &playerPaddle);

		//	Render aiPaddle
	SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
	SDL_RenderFillRect(renderer, &aiPaddle);

		//	Render ball
	Difficulty = abs(speed_x) + abs(speed_y) - 1;

	SDL_SetRenderDrawColor(renderer, 255 - 255 / Difficulty, 255 / Difficulty, 0, 255);	//	Collor gets more red through the game
	SDL_RenderFillRect(renderer, &ball);

	SDL_RenderPresent(renderer);
}

void Input()
{	//	This function tracks mouse and detects key pressing

	while (SDL_PollEvent(&event))
	{
		//	Mouse
		if (event.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
		}

		//	When 'x' or 'F4' pressed
		if (event.type == SDL_QUIT)
		{
			running = false;	//	Stop the loop
		}

		if (event.type == SDL_KEYDOWN)
			switch (event.key.keysym.sym)
			{	//	When 'Esc' is pressed
			case SDLK_ESCAPE:
				running = false;	//	Stop the loop
				break;
			}
	}
}

int aiAim;

void Update()
{
	playerPaddle.y = mouse_y;

	if (ball.x < 0 || ball.x > SCREEN_WIDTH)
	{
		//	Score calculation
		if (ball.x < 0)
			aiScore++;
		else
			plScore++;

		ball.x = SCREEN_WIDTH / 2 - 10;			//	Placing ball in the center
		ball.y = SCREEN_HEIGHT / (rand() % 9 + 1);	//	On the random height
		//	Randomly throving the ball
		speed_x = (rand() % 2 + 1) * direction[rand() % 2];
		speed_y = (rand() % 2 + 1) * direction[rand() % 2];

		ConsoleUPD();

	}

	if (ball.y < 0 || ball.y > (SCREEN_HEIGHT - ball.h))
	{
		speed_y = -speed_y;
	}

		//	Actually moving the ball
	ball.x += speed_x;
	ball.y += speed_y;

		//	That one AI that will conquer the world soon 
	
				//aiPaddle.y = ball.y - aiPaddle.h / 2 + ball.h / 2;	!!OLD CODE!!

	aiAim = ball.y - aiPaddle.h / 2 + ball.h / 2 + (rand() % 200) - 100;	//	This is the 'y' position that AI is targeting with displacement to make it look more dynamic

		//	AI moves to it's aim slover than the ball. No immortal AI anymore!
	if (aiAim > aiPaddle.y)
		aiPaddle.y += 2;
	else if (aiAim < aiPaddle.y)
		aiPaddle.y -= 2;

		//	Checking Colisions
	if (CheckColision(ball, aiPaddle) || CheckColision(ball, playerPaddle))
	{
		speed_x = -speed_x;

		//	Ball becames faster with each hit!
		if (speed_x > 0)
			speed_x++;
		else
			speed_x--;

		ConsoleUPD();
	}

	SDL_Delay(10);
}



int main(int argc, char* argv[])
{
	srand(time(NULL));
	ConsoleUPD();

	if (!InitEngine())
	{
		return -1;
	}
	else
	{
		LoadGame();
		while (running)
		{
			//	Input function
			Input();

			//	Update function
			Update();

			//	Render function
			Render();
		}
		return 0;
	}
	
}