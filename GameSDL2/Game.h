#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include "Map.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;

bool inti(SDL_Window*& window, SDL_Renderer*& renderer);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health);
void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning);
bool checkCollision(Player& p1, Player& p2);
void update(Player& player1, Player& player2,Map& gameMap);
void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap);
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2);


