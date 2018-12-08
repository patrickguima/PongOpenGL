
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MULTI,
	GAME_PAUSE,
	GAME_MENU,
	GAME_LEVEL,
	GAME_FINAL
};
enum GameMode {
	SINGLE,
	MULTI,
	WIN,
	LOSE
};
enum GameLevel{
	EASY,
	MEDIUM,
	HARD,
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	
	GameState              State;
	GameMode				Mode;
	GameLevel				Level;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	bool mute = false;
	bool funk = false;

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};

#endif