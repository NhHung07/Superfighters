#include "Player.h"
#include "Bullet.h"

int frameCount = 6;
int frameSpeed = 100;


Player::Player(int x, int y)
{
	destRect = { x,y,96,96 };
	startX = x;
	startY = y;
	srcRect = { 0, 32, 96, 96 };
	dx = dy = 0;
	health = 200;
	isJumping = isSwordAttacking = isShooting = isDead = false;
	lastAttackTime = 0;
	facingRight = true;
	int currentFrame = 0;
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
		isShooting = true;
		shootAnimFrame = 0;                   
		shootAnimStartTime = SDL_GetTicks();
		int bulletX = facingRight ? destRect.x + destRect.w : destRect.x;
		int bulletY = destRect.y + destRect.h / 2;
		int direction = facingRight ? 1 : -1;

		Bullet newBullet(bulletX, bulletY, direction);
		if (facingRight) {
			newBullet.texture = bulletTexture[0];
		}
		else {
			newBullet.texture = bulletTexture[1];
		}
		bullets.push_back(newBullet);  
		lastAttackTime = shootAnimStartTime;
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
		swordAnimFrame = 0;
		swordAnimStartTime = SDL_GetTicks();
		lastAttackTime = swordAnimStartTime;
		hasDealtSwordDamage = false;

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
	if (isSwordAttacking && !hasDealtSwordDamage) {
		if (SDL_HasIntersection(&swordRect, &target.destRect)) {
			target.health -= 15;
			if (target.health < 0) target.health = 0;
			hasDealtSwordDamage = true;
		}
	}
}


void Player::RenderBullets(SDL_Renderer* renderer) {
	for (auto& bullet : bullets) {
		bullet.Render(renderer);
	}
}

void Player::Update() {
	if (dx > 0) {
		facingRight = true;
	}
	else if (dx < 0) {
		facingRight = false;
	}


	if (!isDead && health <= 0) {
		isDead = true;
		deathTime = SDL_GetTicks();
	}


	if (isDead) {
		dx = dy = 0;
		isJumping = isSwordAttacking = isShooting = false;

		if (facingRight)
			state = DEADRIGHT;
		else
			state = DEADLEFT;

		// Chỉ cập nhật animation chết 1 lần, nếu chưa chạy xong 4 frame
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - deathTime >= frameSpeed) {
			deathTime = currentTime;
			if (currentFrame < frameCount - 1) {
				currentFrame++;
			}
		}

		return;
	}
	else if (isJumping && facingRight) {
		state = JUMPRIGHT;
	}
	else if (isJumping && !facingRight) {
		state = JUMPLEFT;
	}
	else if (isSwordAttacking && facingRight) {
		state = SWORDATTACKRIGHT;
	}
	else if (isSwordAttacking && !facingRight) {
		state = SWORDATTACKLEFT;
	}
	else if (isShooting && facingRight) {
		state = SHOTRIGHT;
	}
	else if (isShooting&& !facingRight) {
		state = SHOTLEFT;
	}
	else if (dx > 0) {
		state = RUNRIGHT;
	}
	else if (dx < 0) {
		state = RUNLEFT;
	}
	else if (facingRight){
		state = IDLERIGHT;
	}
	else {
		state = IDLELEFT;
	}

	Uint32 currentTime = SDL_GetTicks();

	if (isShooting) {
		if (currentTime - shootAnimStartTime >= frameSpeed) {
			shootAnimStartTime = currentTime;
			shootAnimFrame++;                      // Qua frame kế tiếp

			if (shootAnimFrame >= frameCount) {    // Hết 4 frame rồi
				isShooting = false;                // Thoát trạng thái bắn
				shootAnimFrame = 0;
			}
		}

		currentFrame = shootAnimFrame;             // Render frame hiện tại của bắn
	}
	else if (isSwordAttacking) {
		if (currentTime - swordAnimStartTime >= frameSpeed) {
			swordAnimStartTime = currentTime;
			swordAnimFrame++;   
			// Qua frame kế tiếp
			if (swordAnimFrame >= frameCount) {    // Hết 4 frame rồi
				isSwordAttacking = false;           // Thoát trạng thái chém
				swordAnimFrame = 0;
			}
		}
		currentFrame = swordAnimFrame;             // Render frame hiện tại của chém
	}
	else {
		if (currentTime > lastFrameTime + frameSpeed) {
			currentFrame = (currentFrame + 1) % frameCount;
			lastFrameTime = currentTime;
		}
	}
}




void Player::Render(SDL_Renderer* renderer) { 
	int tmp = 0;
	if (state == IDLERIGHT) tmp = 0;
	else if (state == IDLELEFT) tmp = 1;
	else if (state == RUNRIGHT) tmp = 2;
	else if (state == RUNLEFT) tmp = 3;
	else if (state == JUMPRIGHT) tmp = 4;
	else if (state == JUMPLEFT) tmp = 5;
	else if (state == SWORDATTACKRIGHT) tmp = 6;
	else if (state == SWORDATTACKLEFT) tmp = 7;
	else if (state == SHOTRIGHT) tmp = 8;
	else if (state == SHOTLEFT) tmp = 9;
	else if (state == DEADRIGHT) tmp = 10;
	else if (state == DEADLEFT) tmp = 11;
	
	SDL_Rect srcRect = { currentFrame*96 , 32 , 96 , 96 };

	SDL_RenderCopy(renderer, textures[tmp], &srcRect , &destRect);
}


void Player::reset()
{
	destRect = { startX,startY,96,96 };
	srcRect = { 0, 32, 96, 96 };
	dx = dy = 0;
	health = 200;
	isJumping = isSwordAttacking = isShooting = isDead = false;
	bullets.clear();
	lastAttackTime = 0;
	int currentFrame = 0;
}