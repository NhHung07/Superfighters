#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "Map.h"
#include <string>

const int SCREEN_WIDTH = 1280; //?? r?ng khung h�nh
const int SCREEN_HEIGHT = 720; //?? cao khung h�nh

// Kh?i t?o
bool init(SDL_Window*& window, SDL_Renderer*& renderer);
// Load texture t? file ?nh
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
// V? thanh m�u cho ng??i ch?i
void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health);
// Reset tr?ng th�i game v� ng??i ch?i
void resetGame(Player& player1, Player& player2);
// X? l� s? ki?n
void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning);
// Ki?m tra ?i?u ki?n k?t th�c game
void checkGameOver(Player& player1, Player& player2);
// Update tr?ng th�i game
void update(Player& player1, Player& player2,Map& gameMap);
// Render game
void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font*& largeFont);
// H?y v� gi?i ph�ng ??i t??ng 
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2, Map& gameMap, TTF_Font* font, TTF_Font* largeFont, Mix_Music* backgroundMusic);


