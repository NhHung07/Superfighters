#include "Bullet.h"
#include <SDL_image.h>

// T?c ?? bay c?a viên ??n
const int BULLET_SPEED = 10;


Bullet::Bullet(int x, int y, int direction)
{
	rect = { x,y,60,60 };
	// Tính t?c ?? theo h??ng
	speed = direction * BULLET_SPEED;
	// Ban ??u ch?a có texture
	texture = nullptr;
}


bool Bullet::isOutOfBounds() {
	return rect.x < 0 || rect.x > 1280;
}


void Bullet::Update() {
	// C?p nh?t v? trí viên ??n
	rect.x += static_cast<int>(speed);
}


void Bullet::Render(SDL_Renderer* renderer) {
	if ( texture) {
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}



