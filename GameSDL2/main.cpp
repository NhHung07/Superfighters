#include "Game.h"
#include "Map.h"
#include <iostream>


int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	TTF_Font* font = NULL;
	TTF_Font* largeFont = NULL;

	if (!init(window, renderer,font,largeFont))
	{
		return -1;
	}

	Map gameMap(renderer);
	Player player1(50, 600);
	Player player2(1180, 600);

	SDL_Texture* background = loadTexture("Resource Files/Image/background.png", renderer);
	if (!background)
	{
		std::cout << "Loi tai background" << std::endl;
		close(window, renderer, nullptr, player1, player2,font,largeFont);
		return -1;
	}


	player1.textures.push_back(loadTexture("Resource Files/Image/player1IdleRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1IdleLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1RunRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1RunLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1JumpRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1JumpLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1SwordAttackRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1SwordAttackLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1ShotRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1ShotLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1DeadRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1DeadLeft.png", renderer));


	player2.textures.push_back(loadTexture("Resource Files/Image/player2IdleRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2IdleLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2RunRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2RunLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2JumpRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2JumpLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2SwordAttackRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2SwordAttackLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2ShotRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2ShotLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2DeadRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2DeadLeft.png", renderer));


	player1.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet1Right.png", renderer));
	player1.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet1Left.png", renderer));
	player2.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet2Right.png", renderer));
	player2.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet2Left.png", renderer));


	SDL_Event event;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			handleEvent(event, player1, player2, isRunning);
		}
		update(player1, player2,gameMap);
		render(renderer, background, player1, player2,gameMap,font,largeFont);
		SDL_Delay(16);
	}

	close(window, renderer, background, player1, player2,font,largeFont);

	return 0;
}