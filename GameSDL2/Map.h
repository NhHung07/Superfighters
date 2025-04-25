#pragma once
#include <SDL.h>

// K�ch th??c map
const int MAP_WIDTH = 1280;
const int MAP_HEIGHT = 736;

// K�ch th??c m?i � tile 
const int TILE_SIZE = 32;  

struct Map {
    // Texture c?a to�n b? tileset
    SDL_Texture* texture; 
    // M?ng 2 chi?u ch?a d? li?u b?n ??
    int mapData[MAP_HEIGHT / TILE_SIZE][MAP_WIDTH / TILE_SIZE]; 


      
    // Constructor
    Map(SDL_Renderer* renderer);
    // Destructor 
    ~Map();
    // N?p d? li?u map
    void loadMap();
    // V? to�n b? b?n ?? l�n m�n h�nh
    void render(SDL_Renderer* renderer);
    // Ki?m tra xem � tile t?i v? tr� (x, y) c� ph?i l� � c?ng kh�ng
    bool isSolidTile(int x, int y);  
	// Ki?m tra va ch?m ??n v?i tile map
    bool isCollidingWithTile(const SDL_Rect& rect);
};