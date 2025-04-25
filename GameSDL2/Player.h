#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "Map.h"
#include "Bullet.h"


const int PLAYER_SPEED = 5; // Tốc độ di chuyển của nhân vật
const float GRAVITY = 0.5f; // Trọng lực
const float JUMP_FORCE = 15.0f; // Lực nhảy
const float MAX_FALL_SPEED = 10.0f; // Tốc độ rơi tối đa của nhân vật
const Uint32 ATTACK_COOLDOWN = 500; // Thời gian hồi chiêu tránh spam 


// Trạng thái nhân vật
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
	HURTRIGHT,
	HURTLEFT
};


struct Player
{	
	static Mix_Chunk* swordSound; // Âm thanh chém
	static Mix_Chunk* gunSound; // Âm thanh bắn

	SDL_Rect srcRect, destRect;	// Khung ảnh gốc và vị trí vẽ nhân vật
	float dx, dy; // Vận tốc theo trục x và y
	std::vector<SDL_Texture*> textures; // Texture của nhân vật
	int health; // Máu
	bool isJumping; // Check xem nhân vật có đang nhảy không
	Uint32 lastAttackTime = 0; // Thời gian lần đánh cuối cùng
	PlayerState state = IDLERIGHT; // Trạng thái hiện tại
	int startX, startY; // Vị trí ban đầu
	std::vector<Bullet> bullets; // Danh sách đạn của nhân vật
	std::vector<SDL_Texture*> bulletTexture; // Danh sách ảnh đạn
	bool facingRight = true; // Hướng quay của nhân vật
	bool isSwordAttacking; // Check nhân vật có đang chém không
	bool isShooting; // Check xem nhân vật có đang bắn không
	bool hasDealtSwordDamage; // Check xem nhân vật đã gây sát thương bằng kiếm trong lần chém này chưa
	Uint32 lastFrameTime = 0; // Frame cuối 
	SDL_Rect swordRect; // Kích thước kiếm

	int shootAnimFrame = 0; // Khung hiện tại của animation bắn
	Uint32 shootAnimStartTime = 0; // Thời gian bắt đầu animation bắn

	int swordAnimFrame = 0; // Khung hiện tại của animation chém
	Uint32 swordAnimStartTime = 0; // Thời gian bắt đầu animation chém
	
	bool isDead; // Kiểm tra xem nhân vật đã chết chưa             
	Uint32 deathTime = 0; // Thời điểm chết 

	int currentFrame ; // Khung hình hiện tại dùng để vẽ
	bool isHurting = false; // Check xem nhân vật có bị thương không
	int hurtFrame = 0; // Khung hình bị thương
	Uint32 hurtStartTime = 0; // thời điểm bắt đầu bị thương
	const int hurtDuration = 300; // thời gian hiệu ứng bị thương 



	// Khởi tạo
	Player(int x, int y);
	// Kiểm tra có thể tấn công không
	bool canAttack();
	// Trọng lực
	void applyGravity();
	// Nhận sát thương
	void TakeDamage(int dmg);
	// Vẽ đạn
	void RenderBullets(SDL_Renderer* renderer);
	// Bắn súng
	void Shoot();
	// Cập nhật đạn
	void UpdateBullets(Player& target, Map& gameMap);
	// Chém kiếm
	void SwordAttack();
	// Cập nhật kiếm
	void UpdateSword(Player& target);
	// Cập nhật trạng thái người chơi
	void Update();  
	// Render nhân vật
	void Render(SDL_Renderer* renderer);
	// Reset lại nhân vật(dùng khi restart game)
	void reset();
};

