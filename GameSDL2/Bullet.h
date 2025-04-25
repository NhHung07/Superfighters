#pragma once
#include <SDL.h>



struct Bullet {
	// V? tr� v� k�ch th??c c?a vi�n ??n
    SDL_Rect rect;
    // T?c ?? bay c?a ??n
    int speed;
	// Texture c?a vi�n ??n
    SDL_Texture* texture;



    // H�m kh?i t?o vi�n ??n v?i v? tr� v� h??ng b?n
    Bullet(int x, int y, int direction);
    // H�m ki?m tra xem ??n ?� bay ra kh?i m�n h�nh hay ch?a
    bool isOutOfBounds();
    // C?p nh?t v? tr� ??n 
    void Update();
    // V? ??n l�n m�n h�nh
    void Render(SDL_Renderer* renderer);
};