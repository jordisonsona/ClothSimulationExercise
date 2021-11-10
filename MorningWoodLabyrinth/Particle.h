#pragma once
#include <glm\glm.hpp>

class Particle
{
public:
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z);
	~Particle();

	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
	void setBouncing(float bouncing);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);

	//getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();
	glm::vec3 getForce();
	glm::vec3 getVelocity();
	float getBouncing();
	float getLifetime();
	bool isFixed();

	//other
	void addForce(glm::vec3 force);
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method);


private:
	glm::vec3 _currentPosition;
	glm::vec3 _previousPosition;
	glm::vec3 _force;
	glm::vec3 _velocity;

	float _bouncing;
	float _lifetime;
	bool  _fixed;

};

