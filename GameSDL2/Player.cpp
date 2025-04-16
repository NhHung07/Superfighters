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
	isJumping = isSwordAttacking = false;
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
	for (size_t i = 0; i < bullets.size(); ) {
		bullets[i].Update();  // Cập nhật vị trí đạn

		// Nếu đạn ngoài màn hình thì xóa
		if (bullets[i].isOutOfBounds()) {
			bullets.erase(bullets.begin() + i);
			continue;
		}

		// Kiểm tra va chạm với người chơi đối phương
		if (SDL_HasIntersection(&bullets[i].rect, &target.destRect)) {
			target.health -= 10;
			if (target.health < 0) target.health = 0;

			bullets.erase(bullets.begin() + i); // Xóa viên đạn đã trúng
			continue;
		}
		bool collided = false;
		for (size_t j = 0; j < target.bullets.size(); ++j) {
			if (SDL_HasIntersection(&bullets[i].rect, &target.bullets[j].rect)) {
				// Xóa viên đạn của đối phương
				target.bullets.erase(target.bullets.begin() + j);
				// Xóa viên đạn của mình
				bullets.erase(bullets.begin() + i);
				collided = true;
				break;
			}
		}
		if (collided) continue;

		++i;
	}
}

void Player::SwordAttack() {
	if (canAttack()) {
		isSwordAttacking = true;
		lastAttackTime = SDL_GetTicks();

		int swordWidth = 30;
		int swordHeight = 40;
		int offset = 10;

		// Chém bên phải nếu đang quay phải
		if (facingRight) {
			swordRect = {
				destRect.x + destRect.w + offset,
				destRect.y + (destRect.h - swordHeight) / 2,
				swordWidth,
				swordHeight
			};
		}
		else { // Quay trái
			swordRect = {
				destRect.x - swordWidth - offset,
				destRect.y + (destRect.h - swordHeight) / 2,
				swordWidth,
				swordHeight
			};
		}
	}
}

void Player::UpdateSword(Player& target) {
	if (isSwordAttacking) {
		if (SDL_HasIntersection(&swordRect, &target.destRect)) {
			target.health -= 15;
			if (target.health < 0) target.health = 0;
		}
		// Sau khi chém xong, ngừng chém sau 1 frame
		isSwordAttacking = false;
	}
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
	isJumping = isSwordAttacking = false;
	bullets.clear();
	lastAttackTime = 0;
}