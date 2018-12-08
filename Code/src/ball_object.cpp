
#include "ball_object.h"

using namespace irrklang;

BallObject::BallObject()
	: GameObject(), Radius(12.5f), Stuck(true) { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_height, GLuint window_width, GameObject *Player, GameObject *Player2, ISoundEngine *SoundEngine)
{
	//this->Stuck = true;
	// If not stuck to player board
	if (!this->Stuck)
	{
		// Move the ball
		this->Position += this->Velocity * dt;
		// Then check if outside window bounds and if so, reverse velocity and restore at correct position
		if (this->Position.x <= Player->Position.x +Player->Size.x/2)
		{
			
			this->Velocity.x *= 1.05f;
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = Player->Position.x + Player->Size.x/2;
			if(this->Position.y <= Player->Position.y + Player->Size.x/2 && this->Position.y >= Player->Position.y+this->Radius/2){
				this->Velocity.y = -(this->initialVelocity.y);
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
			}
			else if (this->Position.y < Player->Position.y + this->Radius/2 && this->Position.y > Player->Position.y - this->Radius/2) {
				this->Velocity.y = 0.0f;
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
				//this->Velocity.x = 350.0f;
			}
			else if (this->Position.y <= Player->Position.y - this->Radius/2 && this->Position.y >= Player->Position.y - Player->Size.x/2) {
				this->Velocity.y = (this->initialVelocity.y);
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
			}
			else {
				Player2->score += 1;
				this->Position.x = window_width / 2;
				this->Position.y = window_height / 2;
				this->Velocity.y = 0;
				this->Velocity.x = this->initialVelocity.x;
			}
		}
		else if (this->Position.x >= Player2->Position.x+(Player2->Size.x/2 -30.0f))
		{
			
			this->Velocity.x = -this->Velocity.x;
			this->Velocity.x *= 1.05f;
			this->Position.x = Player2->Position.x + (Player2->Size.x / 2 - 30.0f);
			if (this->Position.y <= Player2->Position.y + Player2->Size.x / 2 && this->Position.y >= Player2->Position.y + this->Radius / 2) {
				this->Velocity.y = -(this->initialVelocity.y);
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
			}
			else if (this->Position.y < Player2->Position.y + this->Radius / 2 && this->Position.y > Player2->Position.y - this->Radius / 2) {
				this->Velocity.y = 0.0f;
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
			}
			else if (this->Position.y <= Player2->Position.y - this->Radius / 2 && this->Position.y >= Player2->Position.y - Player2->Size.x / 2) {
				this->Velocity.y = (this->initialVelocity.y);
				SoundEngine->play2D("audio/solid.wav", GL_FALSE);
			}
			else {
				Player->score += 1;
				this->Position.x = window_width / 2;
				this->Position.y = window_height / 2;
				this->Velocity.y = 0;
				this->Velocity.x = -(this->initialVelocity.x);
				
			}
		}
		
		if (this->Position.y <= 0.0f)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
		}
		if (this->Position.y + this->Size.y >= window_height ) {
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = window_height - this->Size.y;
		}
		
		
		
		



	}
	return this->Position;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}