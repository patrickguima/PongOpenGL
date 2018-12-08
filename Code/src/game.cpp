
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include <vector>;
#include "irrKlang/irrKlang.h"

// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
GameObject      *Player2;
std::vector <std::string> points = {"0","1","2","3","4","5"};
std::vector <std::string> background= { "background","rio"};
using namespace irrklang;
int backToDraw = 0;

ISoundEngine *SoundEngine = createIrrKlangDevice();

BallObject *Ball;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Mode(SINGLE),Keys(),Level(MEDIUM), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Player2;
}

void Game::Init()
{
	//PLAY MUSIC
	SoundEngine->play2D("audio/bites.mp3", GL_TRUE);
	

	// Load shaders
	

	ResourceManager::LoadShader("cg_ufpel.vs", "cg_ufpel.fs", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/menu.png", GL_TRUE, "menu");
	ResourceManager::LoadTexture("textures/level.png", GL_TRUE, "level");
	ResourceManager::LoadTexture("textures/pause.png", GL_TRUE, "pause");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/rio.png", GL_FALSE, "rio");
	ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("textures/pong.png", GL_TRUE, "pong");
	ResourceManager::LoadTexture("textures/patrick.png", GL_TRUE, "patrick");

	ResourceManager::LoadTexture("textures/win.png", GL_TRUE, "win");
	ResourceManager::LoadTexture("textures/2win.png", GL_TRUE, "2win");
	ResourceManager::LoadTexture("textures/lost.png", GL_TRUE, "lost");

	ResourceManager::LoadTexture("textures/0.png", GL_TRUE, "0");
	ResourceManager::LoadTexture("textures/1.png", GL_TRUE, "1");
	ResourceManager::LoadTexture("textures/2.png", GL_TRUE, "2");
	ResourceManager::LoadTexture("textures/3.png", GL_TRUE, "3");
	ResourceManager::LoadTexture("textures/4.png", GL_TRUE, "4");
	ResourceManager::LoadTexture("textures/5.png", GL_TRUE, "5");
	// Set render-specific controls


	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	

	glm::vec2 playerPos = glm::vec2(-45.0f, this->Height/2);
	Player = new GameObject(playerPos,glm::vec2(150,20), ResourceManager::GetTexture("paddle"));
	Player->Rotation = 1.57f;


	glm::vec2 player2Pos = glm::vec2(this->Width-(75.0f+30.0f), this->Height / 2);
	Player2 = new GameObject(player2Pos, glm::vec2(150, 20), ResourceManager::GetTexture("paddle"));
	Player2->Rotation = 1.57f;

	//glm::vec2 ballPos = playerPos + glm::vec2(75.0f,75.0f);

	glm::vec2 ballPos = glm::vec2(this->Width/2, this->Height/2);
	Ball = new BallObject(ballPos,12.5f, glm::vec2(450.0f,0.0f),
		ResourceManager::GetTexture("face"));
	Ball->initialVelocity = glm::vec2(450.0f, 450.0f);

}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt,this->Height, this->Width,Player,Player2, SoundEngine);
	
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->Keys[GLFW_KEY_F]) {
		this->funk = !this->funk;
		this->Keys[GLFW_KEY_F] = false;
			if (this->funk) {
				backToDraw = 1;
				SoundEngine->stopAllSounds();
				SoundEngine->play2D("audio/funk.mp3", GL_TRUE);
			}
			else {
				backToDraw = 0;
				SoundEngine->stopAllSounds();
				SoundEngine->play2D("audio/bites.mp3", GL_TRUE);
			}
	}


	
	if (this->State == GAME_ACTIVE) {
		Ball->Stuck = false;
		if (this->Keys[GLFW_KEY_P]) {
			this->State = GAME_PAUSE;
			this->Keys[GLFW_KEY_P] = false;
		}
		if (this->Keys[GLFW_KEY_M]) {
			this->mute = !this->mute;
			this->Keys[GLFW_KEY_M] = false;
			if (this->mute) {
				SoundEngine->setAllSoundsPaused(true);

			}
			else {
				SoundEngine->setAllSoundsPaused(false);
			}

		}
	}
	else if (this->State == GAME_FINAL) {
		Ball->Stuck = true;
		if (this->Keys[GLFW_KEY_M]) {
			this->State = GAME_MENU;
		}
	}else if (this->State == GAME_PAUSE) {
		Ball->Stuck = true;
				if (this->Keys[GLFW_KEY_P]) {
				this->State = GAME_ACTIVE;
				this->Keys[GLFW_KEY_P] = false;
			}
			if (this->Keys[GLFW_KEY_M]) {
				this->State = GAME_MENU;
			}

	}
	else if (this->State == GAME_MENU) {
		Player->score = 0;
		Player->Position.y = this->Height / 2;
		Player2->score = 0;
		Player2->Position.y = this->Height / 2;
		Ball->Position = glm::vec2(this->Width / 2, this->Height / 2);
		Ball->Velocity.y = 0;
		Ball->Stuck = true;
		if (this->Keys[GLFW_KEY_1]) {
			this->Mode = SINGLE;
			this->State = GAME_LEVEL;
			this->Keys[GLFW_KEY_1] = false;
		}
		if (this->Keys[GLFW_KEY_2]) {
			this->State = GAME_ACTIVE;
			this->Mode = MULTI;
			this->Keys[GLFW_KEY_2] = false;
		}
	}
	else if (this->State == GAME_LEVEL) {
		if (this->Keys[GLFW_KEY_1]) {
			this->State = GAME_ACTIVE;
			this->Level = EASY;
		}
		if (this->Keys[GLFW_KEY_2]) {
			this->State = GAME_ACTIVE;
			this->Level = MEDIUM;
		}
		if (this->Keys[GLFW_KEY_3]) {
			this->State = GAME_ACTIVE;
			this->Level = HARD;
		}
	}
	



	//PLAYERS MOVIMENT
	if (this->State == GAME_ACTIVE)
	{

		GLfloat velocity = 550.0f* dt;
		// Move PLAYER ONE

	

		//MOVE PLAYER TWO
		if (this->Mode == MULTI) {
			if (this->Keys[GLFW_KEY_W])
			{
				if (Player->Position.y >= (Player->Size.x / 2 - Player->Size.y / 2))
					Player->Position.y -= velocity;
			}
			if (this->Keys[GLFW_KEY_S])
			{
				if (Player->Position.y <= this->Height - (Player->Size.x / 2 + Player->Size.y / 2))
					Player->Position.y += velocity;
			}


			if (this->Keys[GLFW_KEY_UP])
			{
				if (Player2->Position.y >= (Player2->Size.x / 2 - Player2->Size.y / 2))
					Player2->Position.y -= velocity;
			}
			if (this->Keys[GLFW_KEY_DOWN]) {

				if (Player2->Position.y <= this->Height - (Player2->Size.x / 2 + Player2->Size.y / 2))
					Player2->Position.y += velocity;
			}
		}

		else {
			if (this->Mode == SINGLE) {
				if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
				{
					if (Player->Position.y >= (Player->Size.x / 2 - Player->Size.y / 2))
						Player->Position.y -= velocity;
				}
				if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
				{
					if (Player->Position.y <= this->Height - (Player->Size.x / 2 + Player->Size.y / 2))
						Player->Position.y += velocity;
				}
				if (this->Level == EASY) {
					if (Ball->Position.x >= 0) {
						if (Player2->Position.y + 10 > Ball->Position.y && Player2->Position.y >= (Player2->Size.x / 2 - Player2->Size.y / 2)) {
							Player2->Position.y -= velocity/2;
						}
						else {
							if (Player2->Position.y + 10 < Ball->Position.y && Player2->Position.y <= this->Height - (Player2->Size.x / 2 + Player2->Size.y / 2)) {
								Player2->Position.y += velocity/2;
							}

						}
					}
				}
				else if (this->Level == MEDIUM) {
					if (Ball->Position.x >= this->Width / 2) {
						if (Player2->Position.y + 10 > Ball->Position.y && Player2->Position.y >= (Player2->Size.x / 2 - Player2->Size.y / 2)) {
							Player2->Position.y -= (580*dt);
						}
						else {
							if (Player2->Position.y + 10 < Ball->Position.y && Player2->Position.y <= this->Height - (Player2->Size.x / 2 + Player2->Size.y / 2)) {
								Player2->Position.y += (580 * dt);
							}

						}
					}
				}

				else if (this->Level == HARD) {
					if (Player2->Position.y + 10 > Ball->Position.y && Player2->Position.y >= (Player2->Size.x / 2 - Player2->Size.y / 2)) {
						Player2->Position.y -= (400*dt);
					}
					else {
						if (Player2->Position.y + 10 < Ball->Position.y && Player2->Position.y <= this->Height - (Player2->Size.x / 2 + Player2->Size.y / 2)) {
							Player2->Position.y += (400*dt);
						}
					}
				}
			}
		}
	}



}




void Game::Render()
{

		Renderer->DrawSprite(ResourceManager::GetTexture(background[backToDraw]), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
	
	if (this->State == GAME_ACTIVE) {
		if (Player->score > 5 || Player2->score > 5) {
			this->State = GAME_FINAL;
		}
		else {
			Renderer->DrawSprite(ResourceManager::GetTexture(points[Player->score]), glm::vec2((this->Width / 2) - 100, 20), glm::vec2(20.0f, 20.0f), 0.0f);
			Renderer->DrawSprite(ResourceManager::GetTexture(points[Player2->score]), glm::vec2((this->Width / 2) + 100, 20), glm::vec2(20.0f, 20.0f), 0.0f);
		}
		Player->Draw(*Renderer);
		Player2->Draw(*Renderer);
		Ball->Draw(*Renderer);
	}
	else if (this->State == GAME_PAUSE) {
		
		Renderer->DrawSprite(ResourceManager::GetTexture("pause"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) - 100), glm::vec2(200.0f, 200.0f), 0.0f);
	}
	else if (this->State == GAME_MENU) {
		Renderer->DrawSprite(ResourceManager::GetTexture("menu"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) ), glm::vec2(200.0f, 200.0f), 0.0f);
		Renderer->DrawSprite(ResourceManager::GetTexture("pong"), glm::vec2((this->Width / 2) - 150, 50), glm::vec2(300.0f, 200.0f), 0.0f);
		Renderer->DrawSprite(ResourceManager::GetTexture("patrick"), glm::vec2((this->Width) - 100, (this->Height) - 30), glm::vec2(100.0f, 30.0f), 0.0f);
	}
	else if (this->State == GAME_LEVEL) {
		Renderer->DrawSprite(ResourceManager::GetTexture("level"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) - 100), glm::vec2(200.0f, 200.0f), 0.0f);
	}
	else if (this->State == GAME_FINAL) {
			if (Player->score > 5) {
				Renderer->DrawSprite(ResourceManager::GetTexture("win"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) - 100), glm::vec2(200.0f, 200.0f), 0.0f);
			}
			else if (this->Mode == SINGLE) {
				Renderer->DrawSprite(ResourceManager::GetTexture("lost"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) - 100), glm::vec2(200.0f, 200.0f), 0.0f);
			}
			else {
				Renderer->DrawSprite(ResourceManager::GetTexture("2win"), glm::vec2((this->Width / 2) - 100, (this->Height / 2) - 100), glm::vec2(200.0f, 200.0f), 0.0f);

		}
	
	}
	
}