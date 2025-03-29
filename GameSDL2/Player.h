#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const int PLAYER_SPEED = 5;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = 15.0f;
const float MAX_FALL_SPEED = 10.0f;
const Uint32 ATTACK_COOLDOWN = 500;


enum PlayerState {
	IDLE,    
	RUNNING, 
	JUMPING, 
	FALLING
};

struct Player
{
	SDL_Rect srcRect,destRect;
	float dx, dy;
	SDL_Texture* texture;
	int health;
	bool isJumping;
	bool isAttacking;
	Uint32 lastAttackTime = 0;
	PlayerState state = IDLE;
	int startX, startY;

	Player(int x, int y);
	bool canAttack();
	void applyGravity();
	void Update();  
	void Render(SDL_Renderer* renderer);
	void reset();
};

