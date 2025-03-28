#include "Player.h"


int currentFrame = 0;
int frameCount = 8;
int frameSpeed = 100;
Uint32 lastFrameTime = 0;
const int frameWidth = 50;
const int frameHeight = 50;


Player::Player(int x, int y)
{
	destRect = { x,y,50,50 };
	startX = x;
	startY = y;
	srcRect = { 0, 0, frameWidth, frameHeight };
	dx = dy = 0;
	texture = nullptr;
	health = 200;
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

void Player::Update() {
	if (isJumping) {
		state = JUMPING;
	}
	else if (dy > 0) { 
		state = FALLING;
	}
	else if (dx!=0) {
		state = RUNNING;
	}
	else {
		state = IDLE;
	}

	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastFrameTime + frameSpeed) {
		currentFrame = (currentFrame + 1) % frameCount;
		lastFrameTime = currentTime;
	}
}




void Player::Render(SDL_Renderer* renderer) {
	int spriteRow = 0;

	if (state == RUNNING) spriteRow = 1; 
	if (state == JUMPING) spriteRow = 2;  
	if (state == FALLING) spriteRow = 3;  

	SDL_Rect srcRect = { currentFrame * frameWidth, spriteRow * frameHeight, frameWidth, frameHeight };

	SDL_RenderCopy(renderer, texture, NULL, &destRect);
}


void Player::reset() 
{
	destRect = {startX,startY,50,50 };
	srcRect = { 0, 0, frameWidth, frameHeight };
	dx = dy = 0;
	health = 200;
	isJumping = isAttacking = false;
	lastAttackTime = 0;
}