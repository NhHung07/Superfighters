#pragma once
#include <vector>
#include <SDL.h>
#include "Bullet.h"


struct Gun {
    SDL_Texture* bulletTexture;
    Uint32 lastShotTime;
    Uint32 cooldown;
    std::vector<Bullet> bullets;

    Gun(SDL_Texture* bulletTex, Uint32 cd = 300);
    void shoot(int x, int y, float direction);
    void update();
    void render(SDL_Renderer* renderer);
};
