#include "gameplay.h"
#include "collision.h"
#include "circle.h"

using std::cout;
using std::endl;

Gameplay::Gameplay(Game* game, SDL_Renderer* renderer) : Scene(game, renderer) {
	tileMap = new Tilemap(renderer);
	enemyManager = new EnemyManager(renderer);
	userInterface = new UserInterface(renderer);
	player = new Player(renderer);
	score = 0;

	//	TODO: Circel colliison
	//Circle a = { 1,1,3 };
	//Circle b = { 5,5,3 };
	//if (Collision::CircleCollision(a, b))
	//	cout << "CUNT IT WORKED!" << "\n";
}

Gameplay::~Gameplay() {
	isRunning = false;
	for (int i = 0; i < 512; i++) {
		keyDown[i] = false;
	}
	delete tileMap;
	delete enemyManager;
	delete userInterface;
	delete player;
}

void Gameplay::Input() {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
		{
			isRunning = false;
			game->SetIsRunning(false);
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.scancode < 512) {
				keyDown[e.key.keysym.scancode] = true;
			}
		}
		else if (e.type == SDL_KEYUP) {
			if (e.key.keysym.scancode < 512) {
				keyDown[e.key.keysym.scancode] = false;
			}
		}
	}
	if (getKeyDown(SDL_SCANCODE_Q)) {
		isRunning = false;
		game->SetMenuOptions(Game::Menu::mainmenu);
	}
	player->Input(keyDown);
}

void Gameplay::Update() {
	enemyManager->Update();
	player->Update();

	auto& bullets(player->GetBullets());
	auto& enemies(enemyManager->GetEnemies());

	//	TODO: Have score based on kills not hits?
	for (auto& bullet : bullets)
		if(bullet->Active())
			for (auto& enemy : enemies)
				if (enemy->Active())
					if (Collision::BoxCollision(bullet->GetCollider(), enemy->GetCollider())) {
						bullet->Deactivate();
						//	Make this a bool method to test if player is dead.
						enemy->TakeDamage(50);
						userInterface->Score(++score);
					}
}

void Gameplay::Draw() {
	SDL_RenderClear(renderer);

	tileMap->Draw();
	enemyManager->Draw();
	userInterface->Draw();
	player->Draw();

	SDL_RenderPresent(renderer);
}