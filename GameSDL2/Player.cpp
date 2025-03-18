#include "Player.h"


Player::Player(int x, int y, int w, int h,int init_health)
{
	rect = { x,y,w,h };
	dx = dy = 0;
	texture = nullptr;
	health = init_health;
	isJumping = isAttacking = false;
	lastAttackTime = 0;
}


bool Player::canAttack()
{
	Uint32 currentTime = SDL_GetTicks();
	return (currentTime - lastAttackTime) >= ATTACK_COOLDOWN;
}


void Player::applyGravity()
{
	dy += GRAVITY;
	if (dy > MAX_FALL_SPEED) {
		dy = MAX_FALL_SPEED;
	}
}