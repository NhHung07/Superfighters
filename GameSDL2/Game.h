#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "Map.h"
#include <string>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


bool init(SDL_Window*& window, SDL_Renderer*& renderer);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health);
void resetGame(Player& player1, Player& player2);
void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning);
bool checkCollision(Player& p1, Player& p2);
void checkGameOver(Player& player1, Player& player2);
void update(Player& player1, Player& player2,Map& gameMap);
void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font*& largeFont);
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2, TTF_Font* font, TTF_Font* largeFont, Mix_Music* backgroundMusic);


