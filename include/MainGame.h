#pragma once

#include <Lengine/Window.h>
#include <Lengine/Camera3d.h>
#include <Lengine/Camera2D.h>

#include <Lengine/InputManager.h>
#include <Lengine/GLSLProgram.h>
#include <Lengine/SpriteFont.h>
#include <Lengine/SpriteBatch.h>


#include "Chunk.h"
#include "World.h"

class MainGame
{
public:
	MainGame();
	~MainGame();


	void run();
	void initSystems();
	void gameLoop();
	void initShaders();
	void processInput(bool& running);
	void drawGame();
	
private:

	Lengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	int _fps;
	int _maxFPS;
	int _worldWidth;
	int _worldDepth;
	int _renderDistance;
	float _deltaTime;

	bool _isLeftMouseDown;
	bool _isRightMouseDown;
	bool _needsWorldUpdate;
	bool _saveButton;
	bool _applyGravity;
	int8_t _blockType;


	
	Lengine::GLSLProgram _textureProgram;
	Lengine::Camera3d 	_camera;


	Lengine::InputManager _inputManager;


	World _world;
	GameElements _gameElements;
	GLuint _programID;
	
	
	
};

