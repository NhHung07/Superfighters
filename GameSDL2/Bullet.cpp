#include "Bullet.h"
#include <SDL_image.h>
const int BULLET_SPEED = 10;

Bullet::Bullet(int x, int y, int direction)
{
	rect = { x,y,30,15 };
	speed = direction * BULLET_SPEED;
	texture = nullptr;
}

bool Bullet::isOutOfBounds() {
	return rect.x < 0 || rect.x > 1280;
}

void Bullet::Update() {
	rect.x += static_cast<int>(speed);
}

void Bullet::Render(SDL_Renderer* renderer) {
	if ( texture) {
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}



