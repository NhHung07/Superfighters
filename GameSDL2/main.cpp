#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
const int PLAYER_SPEED = 5;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = 12.0f;
const float MAX_FALL_SPEED = 10.0f;
const Uint32 ATTACK_COOLDOWN = 500;


struct Player
{
	SDL_Rect rect;
	float dx, dy;
	SDL_Texture* texture;
	int health ;
	bool isJumping;
	bool isAttacking;
	Uint32 lastAttackTime = 0;
};


bool inti(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "loi SDL khong the khoi tao " << SDL_GetError() << std::endl;
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

void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health) {
	SDL_Rect bgRect = { x, y, 100, 10 }; 
	SDL_Rect hpRect = { x, y, health, 10 };

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &bgRect);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &hpRect);
}


void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning)
{
	if (event.type == SDL_QUIT)
	{
		isRunning = false;
	}
	if (player1.health <= 0 || player2.health <= 0) 
	{
		std::cout << (player1.health <= 0 ? "Player 2 Wins!" : "Player 1 Wins!") << std::endl;
		isRunning = false;
	}
	else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool keyDown = (event.type == SDL_KEYDOWN);
		int speed = keyDown ? PLAYER_SPEED : 0;
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			if (!player1.isJumping)
			{
				player1.dy = -JUMP_FORCE;
				player1.isJumping = true;
			}
			break;
		case SDLK_s: player1.dy = speed; break;
		case SDLK_a: player1.dx = -speed; break;
		case SDLK_d: player1.dx = speed; break;
		case SDLK_j: player1.isAttacking = true; break;

		case SDLK_UP:
			if (!player2.isJumping)
			{
				player2.dy = -JUMP_FORCE;
				player2.isJumping = true;
			}
			break;
		case SDLK_DOWN: player2.dy = speed; break;
		case SDLK_LEFT: player2.dx = -speed; break;
		case SDLK_RIGHT: player2.dx = speed; break;
		case SDLK_KP_1: player2.isAttacking = true; break;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_a: if (player1.dx < 0) player1.dx = 0; break;
		case SDLK_d: if (player1.dx > 0) player1.dx = 0; break;

		case SDLK_LEFT: if (player2.dx < 0) player2.dx = 0; break;
		case SDLK_RIGHT: if (player2.dx > 0) player2.dx = 0; break;
		}
	}
}

bool canAttack(Player& player) {
	Uint32 currentTime = SDL_GetTicks();
	return (currentTime - player.lastAttackTime) >= ATTACK_COOLDOWN;
}


bool checkCollision(Player& p1, Player& p2) {
	return SDL_HasIntersection(&p1.rect, &p2.rect);
}

void applyGravity(Player& player) {
	player.dy += GRAVITY;
	if (player.dy > MAX_FALL_SPEED) {
		player.dy = MAX_FALL_SPEED;
	}
}

void update(Player& player1, Player& player2)
{
	applyGravity(player1);
	applyGravity(player2);

	player1.rect.x += (int)player1.dx;
	player1.rect.y += (int)player1.dy;

	if (player1.rect.x < 0)
	{
		player1.rect.x = 0;
	}
	if (player1.rect.y < 0)
	{
		player1.rect.y = 0;
	}
	if (player1.rect.x + player1.rect.w > SCREEN_WIDTH)
	{
		player1.rect.x = SCREEN_WIDTH - player1.rect.w;
	}
	if (player1.rect.y + player1.rect.h >= SCREEN_HEIGHT) {
		player1.rect.y = SCREEN_HEIGHT - player1.rect.h;
		player1.dy = 0;
		player1.isJumping = false;
	}


	player2.rect.x += (int)player2.dx;
	player2.rect.y += (int)player2.dy;

	if (player2.rect.x < 0)
	{
		player2.rect.x = 0;
	}
	if (player2.rect.y < 0)
	{
		player2.rect.y = 0;
	}
	if (player2.rect.x + player2.rect.w > SCREEN_WIDTH)
	{
		player2.rect.x = SCREEN_WIDTH - player2.rect.w;
	}
	if (player2.rect.y + player2.rect.h >= SCREEN_HEIGHT) {
		player2.rect.y = SCREEN_HEIGHT - player2.rect.h;
		player2.dy = 0;
		player2.isJumping = false;
	}


	if (checkCollision(player1, player2)) {
		if (player1.isAttacking && canAttack(player1)) {
			player2.health -= 10;
			player1.isAttacking = false;
			player1.lastAttackTime = SDL_GetTicks();
			if (player2.health < 0) player2.health = 0;
		}
		if (player2.isAttacking && canAttack(player2)) {
			player1.health -= 10;
			player2.isAttacking = false;
			player2.lastAttackTime = SDL_GetTicks();
			if (player1.health < 0) player1.health = 0;
		}
	}
}

void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, player1.texture, NULL, &player1.rect);
	SDL_RenderCopy(renderer, player2.texture, NULL, &player2.rect);
	renderHealthBar(renderer, 20, 20, player1.health);
	renderHealthBar(renderer, SCREEN_WIDTH - 120, 20, player2.health);
	SDL_RenderPresent(renderer);
}

void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2)
{
	SDL_DestroyTexture(player1.texture);
	SDL_DestroyTexture(player2.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (!inti(window, renderer))
	{
		return -1;
	}

	Player player1 = { {0,650,50,50},0,0,nullptr,100,false,false };
	Player player2 = { {850,650,50,50},0,0,nullptr,100,false,false };

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
		update(player1, player2);
		render(renderer, background, player1, player2);
		SDL_Delay(16);
	}

	close(window, renderer, background, player1, player2);

	return 0;
}