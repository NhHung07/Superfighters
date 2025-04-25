#pragma once
#include <SDL.h>



struct Bullet {
	// V? trí và kích th??c c?a viên ??n
    SDL_Rect rect;
    // T?c ?? bay c?a ??n
    int speed;
	// Texture c?a viên ??n
    SDL_Texture* texture;



    // Hàm kh?i t?o viên ??n v?i v? trí và h??ng b?n
    Bullet(int x, int y, int direction);
    // Hàm ki?m tra xem ??n ?ã bay ra kh?i màn hình hay ch?a
    bool isOutOfBounds();
    // C?p nh?t v? trí ??n 
    void Update();
    // V? ??n lên màn hình
    void Render(SDL_Renderer* renderer);
};