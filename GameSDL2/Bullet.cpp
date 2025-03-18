#include "Bullet.h"

const int BULLET_SPEED = 10;

Bullet::Bullet(int x, int y, float direction, SDL_Texture* init_texture)
{
	rect = { x,y,10,5 };
	dx = direction * BULLET_SPEED;
	texture = init_texture;
	active = true;
}


void Bullet::update() {
	rect.x += static_cast<int>(dx);
	if (rect.x < 0 || rect.x > 900) {
		active = false;
	}
}


void Bullet::render(SDL_Renderer* renderer) {
	if (active) {
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
}