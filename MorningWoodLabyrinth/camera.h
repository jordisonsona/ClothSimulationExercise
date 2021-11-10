#pragma once
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
class Camera
{
private:
	float _aspectRatio;
	float _FOV;
	float _far;
	float _near;
	float _projectionWidth;
	float _projectionHeight;
	bool _OP;
public:
	glm::vec3 _OcameraPos;
	glm::vec3 _PcameraPos;
	glm::vec3 _OcameraFront;
	glm::vec3 _PcameraFront;
	glm::vec3 _OcameraUp;
	glm::vec3 _PcameraUp;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	Camera(int _screenHeight, int _screenWidth);
	~Camera();
	void ChangeProjection();//true= orto false =prespective
	void moveCamera();
	void setPos(glm::vec3 newPos, char camType);
	void setCameraFront(glm::vec3 newfront, char camType);
	bool getprojection();
	void setprojection(bool OP);

};

