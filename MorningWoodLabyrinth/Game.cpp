#include "Game.h"


/**
* Constructor
* Note: It uses an initialization list to set the parameters
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS) :
	_windowTitle(windowTitle), 
	_screenWidth(screenWidth), 
	_screenHeight(screenHeight),
	_gameState(GameState::INIT), 
	_camera(screenWidth, screenHeight),
	_fpsLimiter(enableLimiterFPS, maxFPS, printFPS) {


}

/**
* Destructor
*/
Game::~Game()
{
}

/*
* Game execution
*/
void Game::run() {
		//System initializations
	initSystems();
		//Start the game if all the elements are ready
	gameLoop();
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);		
		//Compile and Link shader
	initShaders();
		//Set up the openGL buffers
	_openGLBuffers.initializeBuffers(_colorProgram);
		//Load the current scenario
	//_gameElements.initData();
	_gameElements.loadGameElements("./resources/scene2D.txt");

	for (int k = 0; k < MAX_COLUMNS; k++) {
		for (int j = 0; j < MAX_ROWS; j++) {
			sysParticles[k][j].setPosition(k*0.5f, 0.9 - j*0.5f, 0.0f);
			sysParticles[k][j].setVelocity(0.0f, 0.0f, 0.0f);
			sysParticles[k][j].setLifetime(50.0f);
			sysParticles[k][j].setBouncing(0.3f);
			if (j == 0) {//to fix the top of the cloth
				sysParticles[k][j].setFixed(true);
			}
			else {
				sysParticles[k][j].setFixed(false);
			}
			posSysPart[k][j] = sysParticles[k][j].getCurrentPosition(); //Copy position values	
			_gameElements.getGameElement((k * 10) + j)._translate = sysParticles[k][j].getCurrentPosition();
		}
	}
	_gameElements.getGameElement(101)._Colradius = 4;
	//----------------------
	// LOAD the Botton-Plane
	//----------------------
	_planeBottom.setPointNormal(glm::vec3(-5.0f, 0.0f, -17.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	/*_planeTop.setPointNormal(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_planeRight.setPointNormal(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	_planeLeft.setPointNormal(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));*/
}

/*
* Initialize the shaders:
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::initShaders() {
		//Compile the shaders
	_colorProgram.addShader(GL_VERTEX_SHADER, "./resources/shaders/vertex-shader.txt");
	_colorProgram.addShader(GL_FRAGMENT_SHADER, "./resources/shaders/fragment-shader.txt");
	_colorProgram.compileShaders();
		//Attributes must be added before linking the code
	_colorProgram.addAttribute("vertexPositionGame");
	_colorProgram.addAttribute("vertexColor");
		//Link the compiled shaders
	_colorProgram.linkShaders();
		//Bind the uniform variables. You must enable shaders before gettting the uniforme variable location
	_colorProgram.use();
	modelMatrixUniform = _colorProgram.getUniformLocation("modelMatrix");
	_viewMatrixUniform = _colorProgram.getUniformLocation("viewMatrix");
	_projectionMatrixUniform = _colorProgram.getUniformLocation("projectionMatrix");
	_colorProgram.unuse();
}

/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	_gameState = GameState::MENU;
	while (_gameState != GameState::EXIT) {		
			//Start synchronization between refresh rate and frame rate
		_fpsLimiter.startSynchronization();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute the player actions (keyboard and mouse)
		executePlayerCommands();
			//Update the game status
		doPhysics();
			//Draw the objects on the screen
		renderGame();	
			//Force synchronization
		_fpsLimiter.forceSynchronization();
	}
}

/*
* Processes input with SDL
*/
void Game::processInput() {
	_inputManager.update();
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}

}


/**
* Executes the actions sent by the user by means of the keyboard and mouse
*/
void Game::executePlayerCommands() {
	if (_gameState == GameState::MENU) {
		_gameState = GameState::PLAY;
		/*std::cout << '.'<< std::endl;
		std::cout << "pres 1 to play and 2 to quit" << std::endl;
		int choice;
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			_gameState = GameState::PLAY;
			break;
		case 2:
			_gameState = GameState::EXIT;
			break;
		default:
			std::cout << "This is not a Modafucking option!! " << std::endl;
			executePlayerCommands();
			break;
		}*/

	}
	if (_gameState == GameState::PLAY) {
		if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
			cout << mouseCoords.x << ", " << mouseCoords.y << endl;
		}
		//es un click no mantingut
		///WASD move palyer
		if (_inputManager.isKeyDown(SDLK_w)) {
			_camera._PcameraPos.x += 0.1f;
			_camera._PcameraFront.x += 0.1f;
			_camera.moveCamera();
			//}
		}
		if (_inputManager.isKeyDown(SDLK_d)) {
			_camera._PcameraPos.y -= 0.1f;
			_camera._PcameraFront.y -= 0.1f;
			_camera.moveCamera();
			//}
		}
		if (_inputManager.isKeyDown(SDLK_s)) {
			//	if (_camera.getprojection() == false) {//only when its prespective
			_camera._PcameraPos.x -= 0.1f;
			_camera._PcameraFront.x -= 0.1f;
			_camera.moveCamera();
			//	}
		}
		if (_inputManager.isKeyDown(SDLK_a)) {
			//if (colision())
			//	if (_camera.getprojection() == false) {//only when its prespective
			_camera._PcameraPos.y += 0.1f;
			_camera._PcameraFront.y += 0.1f;
			_camera.moveCamera();
			//}
		}
		///QE Rotate player
		if (_inputManager.isKeyDown(SDLK_q)) {
			//float newX = sin(glm::radians(-_gameElements.getGameElement(5)._angle - 180));
			//float newY = cos(glm::radians(-_gameElements.getGameElement(5)._angle - 180));
			//_camera._PcameraFront = glm::vec3(_gameElements.getGameElement(5)._translate.x + newX / 3, _gameElements.getGameElement(5)._translate.y + newY / 3, _gameElements.getGameElement(5)._translate.z + 0.9);
			_camera.moveCamera();


		}
		if (_inputManager.isKeyDown(SDLK_e)) {
			//float newX = sin(glm::radians(-_gameElements.getGameElement(5)._angle - 180));
			//float newY = cos(glm::radians(-_gameElements.getGameElement(5)._angle - 180));
			//_camera._PcameraFront = glm::vec3(_gameElements.getGameElement(5)._translate.x + newX / 3, _gameElements.getGameElement(5)._translate.y + newY / 3, _gameElements.getGameElement(5)._translate.z + 0.9);
			_camera.moveCamera();
		}
		///P change prespective
		if (_inputManager.isKeyPressed(SDLK_p)) {
			if (_camera.getprojection() == true) {
				_camera.setprojection(false);
			}
			else if (_camera.getprojection() == false) {
				_camera.setprojection(true);
			}
			_camera.moveCamera();
			_camera.ChangeProjection();
		}
		if (_inputManager.isKeyPressed(SDLK_o)) {
			if (wireMode == 2) {
				wireMode = 0;
			}
			else {
				wireMode++;
			}
			//std::cout << wireMode << std::endl;
		}

		for (int i = 0; i < MAX_COLUMNS; i++) {//clean all the forces
			for (int j = 0; j < MAX_ROWS; j++) {//clean all the forces
				sysParticles[i][j].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
			}
		}
		/*
		j
		|	#---#---N---#---#
		|	#	D4	SN	D1	#
		|	W	SW	P	SE	E
		|	#	D3	SS	D2	#
		v	#---#---S---#---#
		i	0-------------->10
		*/
	
	}
}

/*
* Update the game objects based on the physics
*/
void Game::doPhysics() {
	if (_gameState == GameState::PLAY) {
		for (int i = 0; i < MAX_COLUMNS; i++) {
			for (int j = 0; j < MAX_ROWS; j++) {
				if (sysParticles[i][j].getLifetime() > 0) {//is in game
					if (sysParticles[i][j].isFixed() == false) {
						sysParticles[i][j].addForce(0.0f, 0.0f, -9.8f); //gravity
																		//calculate the distance BEFORE the solver
						float disantBottom = _planeBottom.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						float disantRight = _planeRight.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						float disantTop = _planeTop.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						float disantLeft = _planeLeft.distPoint2Plane(sysParticles[i][j].getCurrentPosition());

						//spring forces
						glm::vec3 SN, SE, SS, SW, D1, D2, D3, D4, N, E, S, W;

						if ((i == 0) || (i >= MAX_COLUMNS - 1) || (j == 0) || (j >= MAX_ROWS - 1)) {//Borders
							if ((i == 0 && j == 0) || (i == 0 && j == MAX_ROWS - 1) || (i == MAX_COLUMNS - 1 && j == 0) || (i == MAX_COLUMNS - 1 && j == MAX_ROWS - 1)) {//Corners
																																										 //apply 3 forces
								if ((i == 0) && (j == 0)) {
									//SN
									SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
									//SE
									SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
									//D1
									SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
									
								}
								else if ((i == 0) && (j >= MAX_ROWS - 1)) {
									//SN
									SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
									//SW
									SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
									//D4
									SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
								}
								else if ((i >= MAX_COLUMNS - 1) && (j == 0)) {
									//SE
									SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
									//SS
									SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
									//D2
									SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								}
								else {//i== MAX_COLUMNS - 1 && j== MAX_ROWS-1
									//SS
									SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
									//SW
									SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
									//D3
									SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								}
							}
							else {//apply 5 forces  ( in wich direction?)
								  if(i >= 0){//borde esquerre
								  	//SN = SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								  	//SE = SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								  	//SS = SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								  	//SW = SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								  	////Shear
								  	//D1 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								  	//D2 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								  	//D3 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								  	//D4 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
									
								  }
								  else if (i == MAX_COLUMNS ) {//borde dret 
								  							   //Streach
								  	//SN = SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								  	//SE = SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								  	//SS = SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								  	//SW = SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								  	////Shear
								  	//D1 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								  	//D2 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								  	//D3 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								  	//D4 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
									////SE
									SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
									////SW
									SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								  }
								  else if(j == 0){//brode superior
								  	//SN = SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								  	//SE = SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								  	//SS = SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								  	//SW = SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								  	////Shear
								  	//D1 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								  	//D2 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								  	//D3 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								  	//D4 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
									
								}
								  else{//j==MAX_ROWS-1 borde inferior
								  	//SN = SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								  	//SE = SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								  	//SS = SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								  	//SW = SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								  	////Shear
								  	//D1 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								  	//D2 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								  	//D3 = SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								  	//D4 = SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
									
								  }

							}
						}
						else {
							if ((i == 1) || (i == MAX_COLUMNS - 2) || (j == 1) || (j == MAX_ROWS - 2)) {//not apply bend
								//Streach
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								//Shear
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
								
							}
							else {//apply all the forces
								  //Streach
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j - 1]);
								//Shear
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j + 1]);
								SpringForce(sysParticles[i][j], sysParticles[i - 1][j - 1]);
								SpringForce(sysParticles[i][j], sysParticles[i + 1][j - 1]);
								//Bend
								SpringForce(sysParticles[i][j], sysParticles[i + 2][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j + 2]);
								SpringForce(sysParticles[i][j], sysParticles[i - 2][j]);
								SpringForce(sysParticles[i][j], sysParticles[i][j - 2]);
								
							}
						}
						//////////////////////////
						sysParticles[i][j].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);//solver
						sysParticles[i][j].setLifetime(sysParticles[i][j].getLifetime() - _dt); //lifetime is decreased
						//Check for sphere collisions 
						if (colision(_gameElements.getGameElement(101), sysParticles[i][j])== true) {
							corrctionSphere(_gameElements.getGameElement(101)._translate, sysParticles[i][j]);
						}
						//Check for planes collisions
						float disact;

						disact = _planeBottom.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						if (disantBottom*disact < 0.0f) {
							_planeBottom.correction(sysParticles[i][j]);
						}
						//to implement
						disact = _planeRight.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						if (disantRight*disact < 0.0f) {
							_planeRight.correction(sysParticles[i][j]);
						}

						disact = _planeTop.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						if (disantTop*disact < 0.0f) {
							_planeTop.correction(sysParticles[i][j]);
						}

						disact = _planeLeft.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
						if (disantLeft*disact < 0.0f) {
							_planeLeft.correction(sysParticles[i][j]);
						}
					}
				}
			}
		}
		for (int i = 0; i < MAX_COLUMNS; i++) {//clean all the forces
			for (int j = 0; j < MAX_ROWS; j++) {//clean all the forces
				_gameElements.getGameElement((i * 10) + j)._translate = sysParticles[i][j].getCurrentPosition(); //Avoid to accumulate
			}
		}
		//KFC
		GameObject currentGameElement;
		for (int i = 0; i < _gameElements.getNumGameElements(); i++) {//it works just for a single tank in the scene
			currentGameElement = _gameElements.getGameElement(i);
		}
		for (int i = 0; i < _gameElements.getNumGameElements(); i++) {//sphere coliders
			currentGameElement = _gameElements.getGameElement(i);
			currentGameElement._Colcenter = currentGameElement._translate;
			if (currentGameElement._scale.x > currentGameElement._scale.y && currentGameElement._scale.x > currentGameElement._scale.z) {// x is the bigger radius
				currentGameElement._Colradius = currentGameElement._scale.x;
			}
			else if (currentGameElement._scale.y >= currentGameElement._scale.x && currentGameElement._scale.y >= currentGameElement._scale.z) {// y is the bigger radius
				currentGameElement._Colradius = currentGameElement._scale.y;
			}
			else {//z is the bigger radius
				currentGameElement._Colradius = currentGameElement._scale.z;
			}
		}
	}
	if (_gameState == GameState::MENU) {
		
	}
	
}

/**
* Draw the sprites on the screen
*/
void Game::renderGame() {
	if (_gameState == GameState::MENU) {

	}
	if (_gameState == GameState::PLAY) {
		//Temporal variable
		GameObject currentRenderedGameElement;

		//Clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Bind the GLSL program. Only one code GLSL can be used at the same time
		_colorProgram.use();

		glUniformMatrix4fv(_viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera._viewMatrix));
		glUniformMatrix4fv(_projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera._projectionMatrix));

		//For each one of the elements: Each object MUST BE RENDERED based on its position, rotation and scale data

		for (int i = 0; i < _gameElements.getNumGameElements(); i++) {
			currentRenderedGameElement = _gameElements.getGameElement(i);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::mat4 modelMatrix;
				modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);

				if (currentRenderedGameElement._angle != 0) {
					modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
				}

				modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);

				glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

				_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));
		}

		//Unbind the program
		_colorProgram.unuse();

		//Swap the display buffers (displays what was just drawn)
		_window.swapBuffer();
	}
}

void Game::SpringForce(Particle& P1, Particle& P2){	//Spring variables
	float Ke = 20.0f;
	float Kd = 10.0f;
	float L = 1.0f;
	glm::vec3 modular = (P2.getCurrentPosition() - P1.getCurrentPosition());
	float modul = glm::abs(glm::length(modular));
	glm::vec3 velocity = P2.getVelocity() - P1.getVelocity();
	glm::vec3  Force;
	Force = ((Ke*(modul - L)) + (Kd*(velocity*(modular / modul))))*(modular / modul);
	P1.addForce(Force);
	P2.addForce(-Force);
	Force = glm::vec3(0);
}

void Game::resetGame()
{
	void initSystems();
	void initShaders();
}

void Game::corrctionSphere(glm::vec3 C, Particle & P)
{
	glm::vec3 pTall;
	Sphere _s;
	_s._center = C;
	_s._radius = 4.0f;
	if (_s.intersecSegment(P.getPreviousPosition(), P.getCurrentPosition(), pTall) == true) {
		_s.intersecSegment(P.getPreviousPosition(), P.getCurrentPosition(), pTall);
		glm::vec3 normalTangent(P.getPreviousPosition().x - _s._center.x, P.getPreviousPosition().y - _s._center.y, P.getPreviousPosition().z - _s._center.z);
		Plane tangent(P.getPreviousPosition(), normalTangent);
		//float dist = glm::length(point.getPreviousPosition() - _center); //normalTangent.
		float dist = tangent.distPoint2Plane(P.getCurrentPosition());
		glm::vec3 correcPos = -(1 + P.getBouncing()) * dist *tangent.normal;
		glm::vec3 correcVel = -(1 + P.getBouncing()) * (P.getVelocity()*tangent.normal)*tangent.normal;
		P.setPosition(P.getPreviousPosition() + correcPos);
		P.setVelocity(P.getVelocity() + correcVel);
	}
	else {

	}
}

bool Game::colision(GameObject S, Particle P)
{
	glm::vec3 v = glm::vec3(S._translate - P.getCurrentPosition());
	float dist = ((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
	float mindist = S._Colradius +0.1;

	if (dist < (mindist*mindist)) {
		return true;
	}
	else {
		return false;
	}
}

