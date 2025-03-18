#include "Game.h"
#include "Map.h"
#include <iostream>


int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (!inti(window, renderer))
	{
		return -1;
	}

	Map gameMap(renderer);
	Player player1(50, 600, 50, 50, 200);
	Player player2(500, 600, 50, 50, 200);

	SDL_Texture* background = loadTexture("background.png", renderer);
	if (!background)
	{
		std::cout << "Loi tai background" << std::endl;
		close(window, renderer, nullptr, player1, player2);
		return -1;
	}


	player1.texture = loadTexture("player1.png", renderer);
	player2.texture = loadTexture("player2.png", renderer);
	if (!player1.texture || !player2.texture)
	{
		std::cout << "Loi tai nhan vat" << std::endl;
		close(window, renderer, background, player1, player2);
		return -1;
	}


	SDL_Event event;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			handleEvent(event, player1, player2, isRunning);
		}
		update(player1, player2,gameMap);
		render(renderer, background, player1, player2,gameMap);
		SDL_Delay(16);
	}

	close(window, renderer, background, player1, player2);

	return 0;
}