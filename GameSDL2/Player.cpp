#include "Player.h"
#include "Bullet.h"

int frameCount = 6; // Số lượng frame của animation
int frameSpeed = 100; // Thời gian giữa các frame animation

Mix_Chunk* Player::swordSound = nullptr; // Âm thanh chém kiếm
Mix_Chunk* Player::gunSound = nullptr; // Âm thanh bắn súng

Player::Player(int x, int y) 
{
	destRect = { x,y,96,96 }; // Vị trí và kích thước nhân vật
	startX = x;
	startY = y;
	srcRect = { 0, 32, 96, 96 }; // Frame đầu tiên hiển thị
	dx = dy = 0;
	health = 200;
	currentFrame = 0;
	isJumping = isSwordAttacking = isShooting = isDead = hasDealtSwordDamage = false;
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


void Player::TakeDamage(int dmg) {
	if (isDead || isHurting) return;

	health -= dmg;
	if (health < 0) health = 0;

	isHurting = true;
	hurtFrame = 0;
	hurtStartTime = SDL_GetTicks();
}


void Player::Shoot() {
	if (canAttack()) {
		isShooting = true;
		shootAnimFrame = 0;                   
		shootAnimStartTime = SDL_GetTicks();
		Mix_PlayChannel(-1, gunSound, 0);

		// Tạo đạn theo hướng nhân vật
		int bulletX = facingRight ? destRect.x + destRect.w : destRect.x;
		int bulletY = destRect.y + destRect.h / 2 - 20;
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

void Player::UpdateBullets(Player& target, Map& gameMap) {
	for (size_t i = 0; i < bullets.size(); ) {
		bullets[i].Update();  // Cập nhật vị trí đạn

		// Nếu đạn ngoài màn hình 
		if (bullets[i].isOutOfBounds()) {
			bullets.erase(bullets.begin() + i);
			continue;
		}
		// Nếu đạn va chạm với tile map
		if (gameMap.isCollidingWithTile(bullets[i].rect)) {
			bullets.erase(bullets.begin() + i);
			continue;
		}
		// Kiểm tra va chạm với người chơi đối phương
		if (SDL_HasIntersection(&bullets[i].rect, &target.destRect)) {
			target.TakeDamage(10);

			bullets.erase(bullets.begin() + i); 
			continue;
		}
		// Kiểm tra va chạm với đạn đối phương 
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
		Mix_PlayChannel(-1, swordSound, 0);

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
			target.TakeDamage(15);
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
	// Xác định hướng nhân vật
	if (dx > 0) {
		facingRight = true;
	}
	else if (dx < 0) {
		facingRight = false;
	}

	
	// Nếu nhân vật bị thương
	if (isHurting) {
		Uint32 now = SDL_GetTicks();
		if (now - hurtStartTime >= hurtDuration) {
			isHurting = false;
		}
		else {
			if (facingRight) {
				state = HURTRIGHT;
			}
			else {
				state = HURTLEFT;
			}
			currentFrame = (now - hurtStartTime) / (hurtDuration / frameCount);
			if (currentFrame >= frameCount) currentFrame = frameCount - 1;
			// Không thực hiện các update khác khi đang bị thương
			return;
		}
	}

	// Xử lí nếu nhân vật chết 
	if (!isDead && health <= 0) {
		isDead = true;
		deathTime = SDL_GetTicks();
	}

	// Xử lí các trạng thái nhân vật
	if (isDead) {
		dx = dy = 0;
		isJumping = isSwordAttacking = isShooting = false;

		if (facingRight)
			state = DEADRIGHT;
		else
			state = DEADLEFT;

		// Chỉ cập nhật animation chết 1 lần
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - deathTime >= frameSpeed) {
			deathTime = currentTime;
			if (currentFrame < frameCount - 1) {
				currentFrame++;
			}
		}
		// Không thực hiện update khác nếu nhân vật đã chết
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


	// Cập nhật animation theo trạng thái
	if (isShooting) {
		if (currentTime - shootAnimStartTime >= frameSpeed) {
			shootAnimStartTime = currentTime;
			shootAnimFrame++;                      // Qua frame kế tiếp

			if (shootAnimFrame >= frameCount) {    // Hết 6 frame rồi
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
			if (swordAnimFrame >= frameCount) {    // Hết 6 frame rồi
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
	else if (state == HURTRIGHT) tmp = 12;
	else if (state == HURTLEFT) tmp = 13;
	
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
	currentFrame = 0;

}