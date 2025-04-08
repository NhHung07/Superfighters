#include "Player.h"
#include "Bullet.h"

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
	bulletTexture = nullptr;
	facingRight = true;
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

void Player::Shoot() {
	if (canAttack()) {
		int bulletX = facingRight ? destRect.x + destRect.w : destRect.x;
		int bulletY = destRect.y + destRect.h / 2;
		int direction = facingRight ? 1 : -1;

		Bullet newBullet(bulletX, bulletY, direction);
		newBullet.texture = bulletTexture;
		bullets.push_back(newBullet);  
		lastAttackTime = SDL_GetTicks();
	}
}

void Player::UpdateBullets(Player& target) {
	for (auto& bullet : bullets) {
		bullet.Update();
		if (bullet.active && bullet.CheckCollision(target)) {
			target.health -= 10;
			bullet.active=false;
		}
	}
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
}

void Player::RenderBullets(SDL_Renderer* renderer) {
	for (auto& bullet : bullets) {
		bullet.Render(renderer);
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


	if (dx > 0) {
		facingRight = true;  
	}
	else if (dx < 0) {
		facingRight = false; 
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
	bullets.clear();
	lastAttackTime = 0;
}