#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "Map.h"
#include <string>

const int SCREEN_WIDTH = 1280; //?? r?ng khung hình
const int SCREEN_HEIGHT = 720; //?? cao khung hình

// Kh?i t?o
bool init(SDL_Window*& window, SDL_Renderer*& renderer);
// Load texture t? file ?nh
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
// V? thanh máu cho ng??i ch?i
void renderHealthBar(SDL_Renderer* renderer, int x, int y, int health);
// Reset tr?ng thái game và ng??i ch?i
void resetGame(Player& player1, Player& player2);
// X? lý s? ki?n
void handleEvent(SDL_Event& event, Player& player1, Player& player2, bool& isRunning);
// Ki?m tra ?i?u ki?n k?t thúc game
void checkGameOver(Player& player1, Player& player2);
// Update tr?ng thái game
void update(Player& player1, Player& player2,Map& gameMap);
// Render game
void render(SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2,Map& gameMap, TTF_Font* font, TTF_Font*& largeFont);
// H?y và gi?i phóng ??i t??ng 
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* background, Player& player1, Player& player2, Map& gameMap, TTF_Font* font, TTF_Font* largeFont, Mix_Music* backgroundMusic);


