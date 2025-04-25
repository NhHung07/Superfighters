#include "Game.h"
#include<iostream>

bool gameOver = false; // Kiểm tra game đã kết thúc chưa 
int winner = 0; // Người chơi thắng 



bool init(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Loi SDL khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_Log("Loi MIXER: %s", Mix_GetError());
	}
	if (TTF_Init() == -1) {
		std::cout << "Loi TTF: " << TTF_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("Game cua Superfighters Hung", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "Loi window khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cout << "Loi renderer khong the khoi tao " << SDL_GetError() << std::endl;
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "Loi IMG khong the khoi tao " << IMG_GetError() << std::endl;
		return false;
	}
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
	SDL_Rect bgRect = { x, y, 500, 15 };
	SDL_Rect hpRect = { x, y, health*2.5, 15 };
	// Nền thanh máu (màu xám)
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &bgRect);
	// Thanh máu hiện tại (màu đỏ)
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
		isRunning = false; //Thoát game
	}
	// Xử lý click chuột vào nút replay khi game kết thúc
	if (gameOver) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (x >= 540 && x <= 740 && y >= 460 && y <= 510) {
				resetGame(player1,player2);
			}
		}
	}
	// Xử lý đầu vào bàn phím khi game đang chạy
	else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool keyDown = (event.type == SDL_KEYDOWN);
		// Điều khiển người chơi
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
		case SDLK_j:player1.SwordAttack(); break;
		case SDLK_k:player1.Shoot(); break;
		case SDLK_l:player2.health = 0 ; break;

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
		case SDLK_KP_1:player2.SwordAttack(); break;
		case SDLK_KP_2:player2.Shoot(); break;
		}
	}
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
	// Áp dụng trọng lực
	player1.applyGravity();
	player2.applyGravity();

	// Cập nhật đạn
	player1.UpdateBullets(player2,gameMap);
	player2.UpdateBullets(player1,gameMap);

	// Cập nhật kiếm
	player1.UpdateSword(player2);
	player2.UpdateSword(player1);

	// Di chuyển và xử lý va chạm người chơi 1 theo phương ngang
	player1.destRect.x += (int)player1.dx;
	player1.destRect.y += (int)player1.dy;
	if (gameMap.isSolidTile(player1.destRect.x, player1.destRect.y) ||
		gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y) ||
		gameMap.isSolidTile(player1.destRect.x, player1.destRect.y + player1.destRect.h - 1) ||
		gameMap.isSolidTile(player1.destRect.x + player1.destRect.w - 1, player1.destRect.y + player1.destRect.h - 1))
	{
		player1.destRect.x -= (int)player1.dx;
	}

	// Di chuyển và xử lý va chạm người chơi 2 theo phương ngang
	player2.destRect.x += (int)player2.dx;
	player2.destRect.y += (int)player2.dy;
	if (gameMap.isSolidTile(player2.destRect.x, player2.destRect.y) ||
		gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y) ||
		gameMap.isSolidTile(player2.destRect.x, player2.destRect.y + player2.destRect.h - 1) ||
		gameMap.isSolidTile(player2.destRect.x + player2.destRect.w - 1, player2.destRect.y + player2.destRect.h - 1))
	{
		player2.destRect.x -= (int)player2.dx;
	}

	// Xứ lí va chạm người chơi 1 theo phương dọc  
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

	// Xứ lí va chạm người chơi 2 theo phương dọc  
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

	// Giới hạn người chơi trong màn hình
	player1.destRect.x = std::max(0, std::min(player1.destRect.x, SCREEN_WIDTH - player1.destRect.w));
	player1.destRect.y = std::max(0, std::min(player1.destRect.y, SCREEN_HEIGHT - player1.destRect.h));

	player2.destRect.x = std::max(0, std::min(player2.destRect.x, SCREEN_WIDTH - player2.destRect.w));
	player2.destRect.y = std::max(0, std::min(player2.destRect.y, SCREEN_HEIGHT - player2.destRect.h));

	
	
	// Cập nhật trạng thái người chơi
	player1.Update();
	player2.Update();

	// Kiểm tra kết thúc game
	checkGameOver(player1, player2);
}


void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font*& largeFont)
{
	// Xóa màn hình
	SDL_RenderClear(renderer);
	// Vẽ các thành phần game
	SDL_RenderCopy(renderer, background, NULL, NULL);
	gameMap.render(renderer);
	player1.Render(renderer);
	player2.Render(renderer);
	renderHealthBar(renderer, 50, 20, player1.health);
	renderHealthBar(renderer, 730, 20, player2.health);
	player1.RenderBullets(renderer);
	player2.RenderBullets(renderer);
	// Hiển thị màn hình kết thúc game 
	if (gameOver ) 
	{ 
		SDL_Color color = { 255, 105, 180, 255 }; 
		SDL_Surface* surface = nullptr;
		SDL_Texture* texture = nullptr;

		// Hiển thị thông báo người chiến thắng
		std::string text = "Player " + std::to_string(winner) + " Wins!";
		surface = TTF_RenderText_Solid( largeFont, text.c_str(), color);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect rect = {240 ,100, 800, 200 };
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		}

		// Vẽ nút replay
		SDL_Rect button = { 540, 460, 200, 50 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderFillRect(renderer, &button);

		// Vẽ chữ "Replay" lên nút
		surface = TTF_RenderText_Solid(font, "Replay", color);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect textRect = { 550, 460, 180, 50 };
			SDL_RenderCopy(renderer, texture, NULL, &textRect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		}
	}
	// Cập nhật màn hình
	SDL_RenderPresent(renderer);
}


void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font* largeFont, Mix_Music* backgroundMusic)
{
	// Giải phóng texture người chơi 1
	for (SDL_Texture* tex : player1.textures) {
		SDL_DestroyTexture(tex);
	}
	// Giải phóng texture người chơi 2
	for (SDL_Texture* tex : player2.textures) {
		SDL_DestroyTexture(tex);
	}
	// Giải phóng texture đạn người chơi 1
	for (SDL_Texture* tex : player1.bulletTexture) {
		SDL_DestroyTexture(tex);
	}
	// Giải phóng texture đạn người chơi 2
	for (SDL_Texture* tex : player2.bulletTexture) {
		SDL_DestroyTexture(tex);
	}
	gameMap.~Map();
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_Quit();
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeChunk(Player::swordSound);
	Mix_FreeChunk(Player::gunSound);
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
}

