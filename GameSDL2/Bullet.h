#pragma once
#include <SDL.h>

struct Bullet {
    SDL_Rect rect;
    float dx;
    SDL_Texture* texture;
    bool active;

    Bullet(int x, int y, float direction, SDL_Texture* texure);
    void update();
    void render(SDL_Renderer* renderer);
};