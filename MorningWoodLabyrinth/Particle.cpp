#include "Particle.h"
#include <iostream>

Particle::Particle() :
	_previousPosition(0, 0, 0), _currentPosition(0, 0, 0), _velocity(0, 0, 0), _force(0, 0, 0), _bouncing(1), _lifetime(50), _fixed(false)
{
}

Particle::Particle(const float& x, const float& y, const float& z) :
	_previousPosition(0, 0, 0), _velocity(0, 0, 0), _force(0, 0, 0), _bouncing(1), _lifetime(50), _fixed(false)
{
	_currentPosition.x = x;
	_currentPosition.y = y;
	_currentPosition.z = z;

}


Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x, y, z);
	//_previousPosition = _currentPosition;
	_currentPosition = pos;
}
void Particle::setPosition(glm::vec3 pos)
{
	_previousPosition = _currentPosition;
	_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x, y, z);
	_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force(x, y, z);
	_force = force;
}

void Particle::setForce(glm::vec3 force)
{
	_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	//glm::vec3 force(x,y,z);
	//_force += force;
	_force += glm::vec3(x, y, z);
}

void Particle::addForce(glm::vec3 force)
{
	_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel(x, y, z);
	_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
	_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
	_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
	_fixed = fixed;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
	return _currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
	return _previousPosition;
}

glm::vec3 Particle::getForce()
{
	return _force;
}

glm::vec3 Particle::getVelocity()
{
	return _velocity;
}

float Particle::getBouncing()
{
	return _bouncing;
}

float Particle::getLifetime()
{
	return _lifetime;
}

bool Particle::isFixed()
{
	return _fixed;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	if (!_fixed)
	{
		switch (method)
		{
		case UpdateMethod::EulerOrig:
		{
			_previousPosition = _currentPosition;
			_currentPosition += _velocity*dt;
			_velocity += _force*dt;
		}
		break;
		case UpdateMethod::EulerSemi:
		{
			_previousPosition = _currentPosition;
			_velocity += _force*dt;
			_currentPosition += _velocity*dt;
		}
		break;
		case UpdateMethod::Verlet:
		{
			float k = 0.96f;
			glm::vec3 NewPos = _currentPosition + k*(_currentPosition - _previousPosition) + _force*(dt*dt);
			_velocity = (NewPos - _currentPosition) / dt;
			_previousPosition = _currentPosition;
			_currentPosition = NewPos;
		}
		break;
		}
	}
	return;
}
