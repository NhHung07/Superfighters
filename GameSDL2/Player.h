#pragma once

#include <SDL.h>

const int PLAYER_SPEED = 5;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = 15.0f;
const float MAX_FALL_SPEED = 10.0f;
const Uint32 ATTACK_COOLDOWN = 500;


struct Player
{
	SDL_Rect rect;
	float dx, dy;
	SDL_Texture* texture;
	int health;
	bool isJumping;
	bool isAttacking;
	Uint32 lastAttackTime = 0;


	Player(int x, int y, int w, int h,int health);
	bool canAttack();
	void applyGravity();
};

