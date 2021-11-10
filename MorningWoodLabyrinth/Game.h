#pragma once


//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Window.h"
#include "GLSLProgram.h"
#include "FPSLimiter.h"
#include "OpenGLBuffers.h"
#include "Vertex.h"
#include "Geometry.h"
#include "InputManager.h"
#include "camera.h"
#include "Particle.h"
#include "GeometryPlane.h"
#define TRIANGLES 0
#define PARTICLE_SYSTEM 1
#define LEFT_RECTANGLE 2
#define RIGHT_RECTANGLE 3
#define CIRCLE 4
#define MAX_COLUMNS 10
#define MAX_ROWS 10


const int NumGameObj = 5; //Triangle, SysParticles




//Game has four possible states: INIT (Preparing environment), PLAY (Playing), EXIT (Exit from the game) or MENU (Game menu)
enum class GameState { INIT, PLAY, EXIT, MENU, GAMEOVER, WIN };

//This class manages the game execution
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS);	//Constructor
		~Game();					//Destructor
		void run();					//Game execution

	private:
			//Attributes	
		std::string _windowTitle;		//Window Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels				
		GameState _gameState;			//It describes the game state				
		Window _window;					//Manage the OpenGL context
		GLSLProgram _colorProgram;		//Manage the shader programs
		FPSLimiter _fpsLimiter;			//Manage the synchronization between frame rate and refresh rate
		OpenGLBuffers _openGLBuffers;	//Manage the openGL buffers
		Geometry _gameElements;			//Manage the game elements
		InputManager _inputManager;		//Manage the input devices
		GLuint modelMatrixUniform;
		GLuint _viewMatrixUniform;
		GLuint _projectionMatrixUniform;
		Camera _camera;
			//Internal methods
		void initSystems();
		void initShaders();		
		void gameLoop();
		void processInput();
		void doPhysics();
		void executePlayerCommands();
		void renderGame();	
		void resetGame();
		void corrctionSphere(glm::vec3 C,Particle &P);

		void SpringForce(Particle & P1, Particle & P2);

		int wireMode = 1;
		bool colision(GameObject S, Particle P);

		glm::vec3 posSysPart[MAX_COLUMNS][MAX_ROWS]; //Only position is needed for draw
		Particle sysParticles[MAX_COLUMNS][MAX_ROWS];

		int _Numparticles = 100;
		float _dt = 0.01f; //Simulation time step

		Plane _planeBottom; //
		Plane _planeTop;
		Plane _planeRight;
		Plane _planeLeft;
};

