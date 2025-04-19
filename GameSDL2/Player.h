#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "Bullet.h"

const int PLAYER_SPEED = 5;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = 15.0f;
const float MAX_FALL_SPEED = 10.0f;
const Uint32 ATTACK_COOLDOWN = 500;


enum PlayerState {
	IDLERIGHT,
	IDLELEFT,
	RUNRIGHT,
	RUNLEFT,
	JUMPRIGHT,
	JUMPLEFT,
	SWORDATTACKRIGHT,
	SWORDATTACKLEFT,
	SHOTRIGHT,
	SHOTLEFT,
	DEADRIGHT,
	DEADLEFT,
};

struct Player
{
	SDL_Rect srcRect,destRect;
	float dx, dy;
	std::vector<SDL_Texture*> textures;
	int health;
	bool isJumping;
	Uint32 lastAttackTime = 0;
	PlayerState state = IDLERIGHT;
	int startX, startY;
	std::vector<Bullet> bullets;
	std::vector<SDL_Texture*> bulletTexture;
	bool facingRight = true;
	bool isSwordAttacking;
	bool isShooting;
	SDL_Rect swordRect;

	Player(int x, int y);
	bool canAttack();
	void applyGravity();
	void RenderBullets(SDL_Renderer* renderer);
	void Shoot();
	void UpdateBullets(Player& target);
	void SwordAttack();
	void UpdateSword(Player& target);
	void Update();  
	void Render(SDL_Renderer* renderer);
	void reset();
};

