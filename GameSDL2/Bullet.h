#pragma once
#include <SDL.h>



struct Bullet {
    SDL_Rect rect;
    int speed;
    int dir;
    SDL_Texture* texture;

    Bullet(int x, int y, int direction);
    bool isOutOfBounds();
    void Update();
    void Render(SDL_Renderer* renderer);
};