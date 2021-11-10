#include "camera.h"



Camera::Camera(int _screenHeight,int _screenWidth)
{
	_FOV = 45.0f;
	_far = 1000.0f;
	_near = 0.1f;
	_projectionWidth = 20.0f;
	_projectionHeight = 20.0f;
	//prespectivw
	_PcameraPos = glm::vec3(-50.0f, 0.0f, 1.0f);
	_PcameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
	_PcameraUp = glm::vec3(0.0f, 0.0f, 0.01f);
	//orto
	_OcameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	_OcameraFront = glm::vec3(0.0f, 0.0f, -0.01f);
	_OcameraUp = glm::vec3(1.0f,  0.0f, 0.0f);

	_aspectRatio = ((float)_screenWidth /(float)_screenHeight);
	_OP = false;
	moveCamera();
	ChangeProjection();
	
}


Camera::~Camera()
{
}

void Camera::ChangeProjection()
{
	if (_OP== true) {//ortogonal
	
		_projectionMatrix = glm::ortho(-_projectionWidth / 2, _projectionWidth / 2, -_projectionHeight / 2, _projectionHeight / 2, _near, _far);
	}
	else if(_OP==false) {//prespective
		_projectionMatrix = glm::perspective(_FOV, _aspectRatio, _near, _far);
	}
}

void Camera::moveCamera()
{
	if (_OP == true) {
		_viewMatrix = glm::lookAt(_OcameraPos, _OcameraFront, _OcameraUp);
	}
	else if (_OP == false) {
		_viewMatrix = glm::lookAt(_PcameraPos, _PcameraFront, _PcameraUp);
	}
}

void Camera::setPos(glm::vec3 newPos,char camType)
{
	if (camType == 'o') {
		_OcameraPos.x = newPos.x;
		_OcameraPos.y = newPos.y;
		_OcameraPos.z = newPos.z;
	}
	else if (camType == 'p') {
		_PcameraPos.x = newPos.x;
		_PcameraPos.y = newPos.y;
		_PcameraPos.z = newPos.z;
	}
}

void Camera::setCameraFront(glm::vec3 newfront, char camType)
{
	if (camType == 'o') {
		_OcameraFront.x = newfront.x;
		_OcameraFront.y = newfront.y;
		_OcameraFront.z = newfront.z;
	}
	else if (camType == 'p') {
		_PcameraFront.x = newfront.x;
		_PcameraFront.y = newfront.y;
		_PcameraFront.z = newfront.z;
	}
}

bool Camera::getprojection()
{
	return _OP;
}

void Camera::setprojection(bool OP)
{
	_OP = OP;
}
