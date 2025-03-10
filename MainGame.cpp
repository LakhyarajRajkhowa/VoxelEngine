#include "MainGame.h"
#include <iostream>

#include <Lengine/Lengine.h>
#include <Lengine/Timing.h>


unsigned int VBO, VAO, EBO;
MainGame::MainGame() 
{
	_screenWidth = 1540; //1540
	_screenHeight = 900; //900
    _worldWidth = INT_MAX; 
    _worldDepth = INT_MAX;
    _renderDistance = 16;
    _fps = 0;
    _maxFPS = 2000;
    _isLeftMouseDown = false;
    _isRightMouseDown = false;
    _needsWorldUpdate = true;
    _applyGravity = false;
    _saveButton = false;
    _blockType = 1;
}

MainGame::~MainGame() {}

void MainGame::run() {
	Lengine::init();
    initSystems();
	gameLoop();

}

void MainGame::initSystems() {
    
	_window.create("Voxel Engine FPS: " , _screenWidth, _screenHeight, 0);
	_camera.init(_screenWidth, _screenHeight, &_inputManager, glm::ivec3(100, 50, 100));

	glClearColor(.0, .0, .0, 1.0f);

	initShaders();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  
    _world.init(_worldWidth, _worldDepth, _renderDistance);
    
}

void MainGame::initShaders() {
	_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	bool running = true;
    
    int time = 0;

    Lengine::FpsLimiter fpsLimiter;

    fpsLimiter.setMaxFPS(_maxFPS);
	while (running) {
        
        fpsLimiter.begin();
        _deltaTime = 60.0f/ ((_fps)?_fps:60);
        _inputManager.update();
        processInput(running);

        _camera.update(_deltaTime);
        

		drawGame();
        _world.updateWorld(_camera.getCameraPosition(), _camera.getCameraDirection(), _isLeftMouseDown, _isRightMouseDown, _applyGravity, _blockType);

        _fps = fpsLimiter.end();

        if (time == 100) {
          std::cout << "FPS: " << _fps << std::endl;
          //std::cout << ""<<_camera.getCameraPosition().x << " " << _camera.getCameraPosition().y << " " << _camera.getCameraPosition().z << std::endl;
            time = 0;
          //  std::cout << _camera.getCameraDirection().x << " " << _camera.getCameraDirection().y << " " << _camera.getCameraDirection().z << std::endl;
        }
        time++;
	}
}

void MainGame::processInput(bool &running) {
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            _inputManager.pressKey(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            _inputManager.releaseKey(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            _inputManager.pressKey(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _inputManager.releaseKey(event.button.button);
            break;
        case SDL_MOUSEMOTION:
            _inputManager.setMouseCoords(event.motion.x, event.motion.y);
            break;

        }

    }
        if (_inputManager.isKeyPressed(SDLK_ESCAPE)) running = false;
        if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) _isRightMouseDown = true;
        if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) _isLeftMouseDown = true;
        if (_inputManager.isKeyPressed(SDLK_g)) _applyGravity = true;

        if (_inputManager.isKeyPressed(SDLK_1)) _blockType = 4;
        if (_inputManager.isKeyPressed(SDLK_2)) _blockType = 7;
        if (_inputManager.isKeyPressed(SDLK_3)) _blockType = 13;
        if (_inputManager.isKeyPressed(SDLK_4)) _blockType = 16;
        if (_inputManager.isKeyPressed(SDLK_5)) _blockType = 19;
        if (_inputManager.isKeyPressed(SDLK_6)) _blockType = 22;
        if (_inputManager.isKeyPressed(SDLK_7)) _blockType = 25;
        if (_inputManager.isKeyPressed(SDLK_8)) _blockType = 28;
        if (_inputManager.isKeyPressed(SDLK_9)) _blockType = 31;

        if (_inputManager.isKeyPressed(SDLK_F5)) _saveButton = true;

       
        int mouseX, mouseY;
        SDL_GetRelativeMouseState(&mouseX, &mouseY);
        _camera.processMouse((float)mouseX, (float)mouseY);
        SDL_SetRelativeMouseMode(SDL_TRUE);

	}

void MainGame::drawGame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.68f, 0.85f, 0.90f, 1.0f);
    _gameElements.drawCrosshair();
   

	_textureProgram.use();
	_textureProgram.setMat4("view", _camera.getViewMatrix());
	_textureProgram.setMat4("projection", _camera.getProjectionMatrix());

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    _textureProgram.setMat4("model", model);


    GLuint fogColorLoc = _textureProgram.getUnifromLocation("fogColor");
    GLuint fogStartLoc = _textureProgram.getUnifromLocation("fogStart");
    GLuint fogEndLoc = _textureProgram.getUnifromLocation("fogEnd");

    glUniform3f(fogColorLoc, 0.7f, 0.8f, 0.9f);  // Light blue fog
    glUniform1f(fogStartLoc, (500.0f / 32.0f) * _renderDistance);
    glUniform1f(fogEndLoc, (500.0f / 32.0f) * _renderDistance + 20);
  
    _world.renderWorld(_textureProgram);
   
	_textureProgram.unuse();

	_window.swapBuffer();
}
















