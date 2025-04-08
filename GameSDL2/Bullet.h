#pragma once
#include <SDL.h>
#include "Player.h"

struct Bullet {
    SDL_Rect rect;
    int speed;
    int dir;
    bool active;
    SDL_Texture* texture;

    Bullet(int x, int y, int direction);
    void Update();
    void Render(SDL_Renderer* renderer);
    bool CheckCollision(Player& target);
};