#include "Game.h"
#include "Map.h"
#include <iostream>


int main(int argc, char* argv[])
{
	// Khai báo cửa sổ và renderer
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	// Font chữ dùng để hiển thị
	TTF_Font* font = NULL;
	TTF_Font* largeFont = NULL;

	// Nhạc nền
	Mix_Music* backgroundMusic = NULL;

	// Khởi tạo 
	if (!init(window, renderer))
	{
		return -1;
	}

	// Khởi tạo bản đồ game
	Map gameMap(renderer);

	// Khởi tạo người chơi 1 và 2
	Player player1(150, 640);
	Player player2(1034, 640);

	// Tải background
	SDL_Texture* background = loadTexture("Resource Files/Image/background.png", renderer);
	if (!background)
	{
		std::cout << "Loi tai background" << std::endl;
		close(window, renderer, nullptr, player1, player2,gameMap,font,largeFont,backgroundMusic);
		return -1;
	}

	// Tải texture cho các trạng thái của người chơi 1
	player1.textures.push_back(loadTexture("Resource Files/Image/player1IdleRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1IdleLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1RunRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1RunLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1JumpRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1JumpLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1SwordAttackRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1SwordAttackLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1ShotRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1ShotLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1DeadRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1DeadLeft.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1HurtRight.png", renderer));
	player1.textures.push_back(loadTexture("Resource Files/Image/player1HurtLeft.png", renderer));

	// Tải texture cho các trạng thái của người chơi 2
	player2.textures.push_back(loadTexture("Resource Files/Image/player2IdleRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2IdleLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2RunRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2RunLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2JumpRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2JumpLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2SwordAttackRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2SwordAttackLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2ShotRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2ShotLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2DeadRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2DeadLeft.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2HurtRight.png", renderer));
	player2.textures.push_back(loadTexture("Resource Files/Image/player2HurtLeft.png", renderer));

	// Tải texture đạn cho player 1
	player1.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet1Right.png", renderer));
	player1.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet1Left.png", renderer));

	// Tải texture đạn cho player 2
	player2.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet2Right.png", renderer));
	player2.bulletTexture.push_back(loadTexture("Resource Files/Image/bullet2Left.png", renderer));

	// Tải font chữ dùng để hiển thị
	largeFont = TTF_OpenFont("Resource Files/Font/font.ttf", 72);
	font = TTF_OpenFont("Resource Files/Font/font.ttf", 24);

	// Tải âm thanh
	backgroundMusic = Mix_LoadMUS("Resource Files/Sound/background.mp3");
	Player::swordSound = Mix_LoadWAV("Resource Files/Sound/sword.wav");
	Player::gunSound = Mix_LoadWAV("Resource Files/Sound/gun.wav");

	// Bắt đầu vòng lặp game
	SDL_Event event;
	bool isRunning = true;

	// Phát nhạc nền lặp lại
	Mix_PlayMusic(backgroundMusic, -1);


	while (isRunning)
	{
		// Xử lý sự kiện
		while (SDL_PollEvent(&event) != 0)
		{
			handleEvent(event, player1, player2, isRunning);
		}
		// Cập nhật logic game
		update(player1, player2,gameMap);
		// Vẽ game
		render(renderer, background, player1, player2,gameMap,font,largeFont);
		// Delay để game chạy ở ~60 FPS
		SDL_Delay(16);
	}

	// Dọn dẹp tài nguyên và thoát
	close(window, renderer, background, player1, player2, gameMap, font, largeFont, backgroundMusic);

	return 0;
}