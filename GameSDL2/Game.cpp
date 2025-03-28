#include "Game.h"
#include<iostream>

bool gameOver = false;
int winner = 0;

bool init(SDL_Window*& window, SDL_Renderer*& renderer ,TTF_Font*& font, TTF_Font*& largeFont)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "loi SDL khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	if (TTF_Init() == -1) {
		std::cout << "Loi TTF: " << TTF_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("Game cua Hung", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "loi window khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cout << "loi renderer khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "loi IMG khong the khoi tao " << IMG_GetError() << std::endl;
		return false;
	}
	largeFont = TTF_OpenFont("font.ttf", 72);
	font = TTF_OpenFont("font.ttf" ,24);
	if (!font||!largeFont) {
		std::cout << "Loi Font: " << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}


SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, path);
	if (!texture)
	{
		std::cout << "loi khong the load texture " << path << " - " << IMG_GetError() << std::endl;
	}
	return texture;
}

void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health)
{
	SDL_Rect bgRect = { x, y, 500, 10 };
	SDL_Rect hpRect = { x, y, health*2.5, 10 };

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &bgRect);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &hpRect);
}

void resetGame(Player& player1, Player& player2)
{
	player1.reset();
	player2.reset();
	gameOver = false;
	winner = 0;
}

void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning)
{
	if (event.type == SDL_QUIT)
	{
		isRunning = false;
	}
	if (gameOver) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (x >= 540 && x <= 740 && y >= 460 && y <= 510) {
				resetGame(player1,player2);
			}
		}
	}
	else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool keyDown = (event.type == SDL_KEYDOWN);
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			if (!player1.isJumping && keyDown)
			{
				player1.dy = -JUMP_FORCE;
				player1.isJumping = true;
			}
			break;
		case SDLK_s:player1.dy = keyDown ? PLAYER_SPEED : 0; break;
		case SDLK_a:player1.dx = keyDown ? -PLAYER_SPEED : 0; break;
		case SDLK_d:player1.dx = keyDown ? PLAYER_SPEED : 0; break;
		case SDLK_j:player1.isAttacking = true; break;

		case SDLK_UP:
			if (!player2.isJumping && keyDown)
			{
				player2.dy = -JUMP_FORCE;
				player2.isJumping = true;
			}
			break;
		case SDLK_DOWN:player2.dy = keyDown ? PLAYER_SPEED : 0; break;
		case SDLK_LEFT:player2.dx = keyDown ? -PLAYER_SPEED : 0; break;
		case SDLK_RIGHT:player2.dx = keyDown ? PLAYER_SPEED : 0; break;
		case SDLK_KP_1:player2.isAttacking = true; break;
		}
	}
}


bool checkCollision(Player& p1, Player& p2)
{
	return SDL_HasIntersection(&p1.destRect, &p2.destRect);
}

void checkGameOver(Player& player1, Player& player2) {
	if (player1.health <= 0) {
		gameOver = true;
		winner = 2;
	}
	else if (player2.health <= 0) {
		gameOver = true;
		winner = 1;
	}
}


void update(Player& player1, Player& player2, Map& gameMap)
{
	player1.applyGravity();
	player2.applyGravity();

	
	player1.destRect.x += (int)player1.dx;
	player1.destRect.y += (int)player1.dy;
	if (gameMap.isSolidTile(player1.destRect.x, player1.destRect.y) ||
		gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y) ||
		gameMap.isSolidTile(player1.destRect.x, player1.destRect.y + player1.destRect.h - 1) ||
		gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y + player1.destRect.h - 1))
	{
		player1.destRect.x -= (int)player1.dx;
	}


	player2.destRect.x += (int)player2.dx;
	player2.destRect.y += (int)player2.dy;
	if (gameMap.isSolidTile(player2.destRect.x, player2.destRect.y) ||
		gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y) ||
		gameMap.isSolidTile(player2.destRect.x, player2.destRect.y + player2.destRect.h - 1) ||
		gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y + player2.destRect.h - 1))
	{
		player2.destRect.x -= (int)player2.dx;
	}


	if (player1.dy > 0 &&
		(gameMap.isSolidTile(player1.destRect.x, player1.destRect.y + player1.destRect.h) ||
			gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y + player1.destRect.h)))
	{
		player1.destRect.y = ((player1.destRect.y+player1.destRect.h) / TILE_SIZE) * TILE_SIZE - player1.destRect.h;
		player1.dy = 0;
		player1.isJumping = false;  
	}
	if (player1.dy < 0 &&
		(gameMap.isSolidTile(player1.destRect.x, player1.destRect.y) ||
			gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y)))
	{
		player1.destRect.y = ((player1.destRect.y / TILE_SIZE) + 1) * TILE_SIZE;
		player1.dy = 0; 
	}


	if (player2.dy > 0 &&
		(gameMap.isSolidTile(player2.destRect.x, player2.destRect.y + player2.destRect.h) ||
			gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y + player2.destRect.h)))
	{
		player2.destRect.y = ((player2.destRect.y+player2.destRect.h) / TILE_SIZE) * TILE_SIZE - player2.destRect.h;
		player2.dy = 0;
		player2.isJumping = false;
	}
	if (player2.dy < 0 &&
		(gameMap.isSolidTile(player2.destRect.x, player2.destRect.y) ||
			gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y)))
	{
		player2.destRect.y = ((player2.destRect.y / TILE_SIZE) + 1) * TILE_SIZE;
		player2.dy = 0;
	}

	
	player1.destRect.x = std::max(0, std::min(player1.destRect.x, SCREEN_WIDTH - player1.destRect.w));
	player1.destRect.y = std::max(0, std::min(player1.destRect.y, SCREEN_HEIGHT - player1.destRect.h));

	player2.destRect.x = std::max(0, std::min(player2.destRect.x, SCREEN_WIDTH - player2.destRect.w));
	player2.destRect.y = std::max(0, std::min(player2.destRect.y, SCREEN_HEIGHT - player2.destRect.h));

	
	if (checkCollision(player1, player2)) {
		if (player1.isAttacking && player1.canAttack()) {
			player2.health -= 10;
			player1.isAttacking = false;
			player1.lastAttackTime = SDL_GetTicks();
			if (player2.health < 0) player2.health = 0;
		}
		if (player2.isAttacking && player2.canAttack()) {
			player1.health -= 10;
			player2.isAttacking = false;
			player2.lastAttackTime = SDL_GetTicks();
			if (player1.health < 0) player1.health = 0;
		}
	}


	player1.Update();
	player2.Update();

	checkGameOver(player1, player2);
}


void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font*& largeFont)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	if (gameOver ) 
	{ 
		SDL_Color color = { 255, 105, 180, 255 }; 
		SDL_Surface* surface = nullptr;
		SDL_Texture* texture = nullptr;

	
		std::string text = "Player " + std::to_string(winner) + " Wins!";
		surface = TTF_RenderText_Solid( largeFont, text.c_str(), color);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect rect = {240 ,100, 800, 200 };
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		}

		SDL_Rect button = { 540, 460, 200, 50 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderFillRect(renderer, &button);

		surface = TTF_RenderText_Solid(font, "Replay", color);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect textRect = { 550, 460, 180, 50 };
			SDL_RenderCopy(renderer, texture, NULL, &textRect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		}
	}
	else 
	{
		gameMap.render(renderer);
		player1.Render(renderer);
		player2.Render(renderer);
		renderHealthBar(renderer, 50, 20, player1.health);
		renderHealthBar(renderer, 730, 20, player2.health);
	}
	SDL_RenderPresent(renderer);
}


void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2, TTF_Font* font, TTF_Font* largeFont)
{
	SDL_DestroyTexture(player1.texture);
	SDL_DestroyTexture(player2.texture);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


