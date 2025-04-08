#include "Bullet.h"
#include <SDL_image.h>
const int BULLET_SPEED = 10;

Bullet::Bullet(int x, int y, int direction)
{
	rect = { x,y,30,15 };
	speed = direction * BULLET_SPEED;
	active = true;
	texture = nullptr;
}


void Bullet::Update() {
	rect.x += static_cast<int>(speed);
	if (rect.x < 0 || rect.x > 1280) {
		active = false;
	}

}

bool Bullet::CheckCollision(Player& target) {
	return SDL_HasIntersection(&rect, &target.destRect);
}

void Bullet::Render(SDL_Renderer* renderer) {
	if (active && texture) {
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}



