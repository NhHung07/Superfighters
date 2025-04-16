#pragma once
#include <SDL.h>

const int MAP_WIDTH = 1280;
const int MAP_HEIGHT = 720;
const int TILE_SIZE = 40;  

struct Map {
    SDL_Texture* texture;
    int mapData[MAP_HEIGHT / TILE_SIZE][MAP_WIDTH / TILE_SIZE];  
    void loadMap();  

    Map(SDL_Renderer* renderer);
    ~Map();
    void render(SDL_Renderer* renderer);
    bool isSolidTile(int x, int y);  
};