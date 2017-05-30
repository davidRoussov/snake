#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <random>
#include <time.h>
#include <vector>
using namespace std;

const int SQUARE_LENGTH = 20;
const int SCREEN_WIDTH = SQUARE_LENGTH * 25;
const int SCREEN_HEIGHT = SQUARE_LENGTH * 25;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool quit = false;
int snake_x = 0;
int snake_y = 0;
int fruit_x;
int fruit_y;
int snake_body_length = 1;

enum Direction {LEFT, RIGHT, UP, DOWN};
Direction direction = RIGHT;

struct Pos {
	int x;
	int y;
};

vector<Pos> snake_body;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL error: %s\n", SDL_GetError());
			success - false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL error: %\s", SDL_GetError());
				success = false;
			}
		}
	}

	return success;	
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

void check_if_left_window()
{
	if (snake_x < 0 || snake_x >= SCREEN_WIDTH)
	{
		quit = true;
	}
	else if (snake_y < 0 || snake_y >= SCREEN_HEIGHT)
	{
		quit = true;
	}
}

void generate_fruit()
{
	srand(time(NULL));
	int slots_x = SCREEN_WIDTH / SQUARE_LENGTH;
	int slots_y = SCREEN_HEIGHT / SQUARE_LENGTH;

	fruit_x = (rand() % slots_x) * SQUARE_LENGTH;
	fruit_y = (rand() % slots_y) * SQUARE_LENGTH;
}

void draw() {

	// draw fruit
	SDL_Rect fruit_rect = { fruit_x, fruit_y, SQUARE_LENGTH, SQUARE_LENGTH };
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
	SDL_RenderFillRect(gRenderer, &fruit_rect);

	if (direction == UP) 
	{
		snake_y -= SQUARE_LENGTH;
	}
	else if (direction == DOWN)
	{
		snake_y += SQUARE_LENGTH;
	}
	else if (direction == LEFT)
	{
		snake_x -= SQUARE_LENGTH;
	}
	else if (direction == RIGHT)
	{	
		snake_x += SQUARE_LENGTH;
	}


	struct Pos new_pos;
	new_pos.x = snake_x;
	new_pos.y = snake_y;
	snake_body.push_back(new_pos);

	for (int i = 0, size = snake_body.size(); i < size; ++i)
	{
		SDL_Rect fillRect = {snake_body[i].x, snake_body[i].y, SQUARE_LENGTH, SQUARE_LENGTH };
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}

	if (snake_body.size() >= snake_body_length)
	{
		snake_body.erase(snake_body.begin());
	}

	if (snake_x == fruit_x && snake_y == fruit_y) {
		snake_body_length++;
		generate_fruit();
	}

	check_if_left_window();
}

void process_user_input(string key) {
	if (key.compare("Up") == 0) 
	{
		direction = UP;
	}
	else if (key.compare("Down") == 0)
	{
		direction = DOWN;
	}
	else if (key.compare("Left") == 0)
	{
		direction = LEFT;
	}
	else if (key.compare("Right") == 0)
	{
		direction = RIGHT;
	}
}

void test_how_vectors_work() 
{
	struct Pos pos1;
	pos1.x = 2;
	pos1.y = 3;

	snake_body.push_back(pos1);

	struct Pos pos2;
	pos2.x = 4;
	pos2.y = 5;

	snake_body.push_back(pos2);

	snake_body.erase(snake_body.begin());

	for (int i = 0, size = snake_body.size(); i < size; ++i)
	{
		cout << snake_body[i].x << " " << snake_body[i].y << endl;
	}
}

int main( int argc, char* args[] )
{
	if (!init()) {
		cout << "Failed to initialize SDL" << endl;
	} 
	else 
	{
		generate_fruit();

		while (!quit)
		{
			SDL_Event event;
			string key_name;
			string user_input;
			while (SDL_PollEvent(&event)) {
				key_name = SDL_GetKeyName(event.key.keysym.sym);
				if (key_name == "Escape") {
					quit = true;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					user_input = key_name;
				}
			}

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			
			process_user_input(user_input);

			draw();

			//Update screen
			SDL_RenderPresent(gRenderer);

			SDL_Delay(150);
		}
	}

	close();

	return 0;
}