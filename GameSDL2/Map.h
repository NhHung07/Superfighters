#pragma once
#include <SDL.h>

// Kích th??c map
const int MAP_WIDTH = 1280;
const int MAP_HEIGHT = 736;

// Kích th??c m?i ô tile 
const int TILE_SIZE = 32;  

struct Map {
    // Texture c?a toàn b? tileset
    SDL_Texture* texture; 
    // M?ng 2 chi?u ch?a d? li?u b?n ??
    int mapData[MAP_HEIGHT / TILE_SIZE][MAP_WIDTH / TILE_SIZE]; 


      
    // Constructor
    Map(SDL_Renderer* renderer);
    // Destructor 
    ~Map();
    // N?p d? li?u map
    void loadMap();
    // V? toàn b? b?n ?? lên màn hình
    void render(SDL_Renderer* renderer);
    // Ki?m tra xem ô tile t?i v? trí (x, y) có ph?i là ô c?ng không
    bool isSolidTile(int x, int y);  
	// Ki?m tra va ch?m ??n v?i tile map
    bool isCollidingWithTile(const SDL_Rect& rect);
};