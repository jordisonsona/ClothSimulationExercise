#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include "Particle.h"

struct Geometrym {
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
};
struct Plane : public Geometrym {
	glm::vec3 normal;
	float dconst;
	Plane() {};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect);
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

	void create(const glm::vec3& point1, const glm::vec3& point2);
	void setPosition(const glm::vec3& newPos);
	void setPointNormal(const glm::vec3& point, const glm::vec3& normal);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& punt1, const glm::vec3& punt2, glm::vec3& pTall);
	void correction(Particle &p);
};

struct Polygon : public Geometrym{
	std::vector <glm::vec3> _listOfVertex;
	float _epsilon;
	int _numVertex;
	//	float dconst;
	Polygon();
	~Polygon();
	void setPosition(const glm::vec3& newPos);
	Polygon(int numVertex, std::vector <glm::vec3> listOfVertex, float e);
	void setNumVertex(int numVertex);
	void setVertex(std::vector <glm::vec3> listOfVertex);
	void setEpsilon(float e);
	bool isInside(const glm::vec3& point);
	Plane colPlane(glm::vec3 point, glm::vec3 prevpoint);
	void correction(Particle &p);
	bool colSec(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};
struct Sphere : public Geometrym {
	float _radius;
	glm::vec3 _center;
	bool isInside(const glm::vec3& point);
	void checkNCorrect(Particle& point);
	Sphere();
	~Sphere();
	void setPosition(const glm::vec3& newPos);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};
