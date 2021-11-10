#pragma once
#include "GeometryPlane.h"

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect) {
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2) {
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::create(const glm::vec3 & point1, const glm::vec3 & point2)
{
	glm::vec3 cam(0, 0, 1);
	glm::vec3 v = point2 - point1;
	normal = glm::cross(v, cam);
	//std::cout << " X:" << normal.x << " Y ." << normal.y << " Z :" << normal.z << std::endl;
	dconst = glm::dot(point1, -normal);
	//std::cout << " D:" <<dconst << std::endl;
}

void Plane::setPosition(const glm::vec3& newPos) {
	dconst = -glm::dot(newPos, normal);
};

void Plane::setPointNormal(const glm::vec3& punt, const glm::vec3& normalVect) {
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(punt, normal);
};

bool Plane::isInside(const glm::vec3& point) {
	float dist;
	dist = glm::dot(point, normal) + dconst;
	if (dist > 1.e-7)
		return false;
	else
		return true;
};

float Plane::distPoint2Plane(const glm::vec3& point) {
	float dist;
	return dist = glm::dot(point, normal) + dconst;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point) {
	glm::vec3 closestP;
	float r = (-dconst - glm::dot(point, normal));
	return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	if (distPoint2Plane(point1)*distPoint2Plane(point2) > 0)	return false;
	float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
	pTall = (1 - r)*point1 + r*point2;
	return true;
}
void Plane::correction(Particle & p)
{
	float disact = distPoint2Plane(p.getCurrentPosition());
	glm::vec3 correcPos = -(1 + p.getBouncing()) * disact *normal;
	glm::vec3 correcVel = -(1 + p.getBouncing()) * (p.getVelocity()*normal)*normal;
	p.setPosition(p.getCurrentPosition() + correcPos);
	p.setVelocity(p.getVelocity() + correcVel);
}
;

Polygon::Polygon()
{
}

Polygon::~Polygon()
{
}

void Polygon::setPosition(const glm::vec3 & newPos)
{
}

Polygon::Polygon(int numVertex, std::vector<glm::vec3> listOfVertex, float e)
{
	_listOfVertex = listOfVertex;
	_numVertex = numVertex;
	_epsilon = e;
}

void Polygon::setNumVertex(int numVertex)
{
	_numVertex = numVertex;
}

void Polygon::setVertex(std::vector<glm::vec3> listOfVertex)
{
	_listOfVertex = listOfVertex;
}

void Polygon::setEpsilon(float e)
{
	_epsilon = e;
}


bool Polygon::isInside(const glm::vec3 & point)
{
	int contador = 0;
	float alpha;
	glm::vec3 Pi;
	glm::vec3 Pii;
	glm::vec3 Temp;
	//for ordenat
	for (int i = 0; i < _listOfVertex.size(); i++) {
		Pi = _listOfVertex[i];
		if (i + 1 < _listOfVertex.size()) {
			Pii = _listOfVertex[i + 1];
		}
		else {
			Pii = _listOfVertex[0];
		}
		if (Pi.y != Pii.y) {
			alpha = (point.y - Pi.y) / (Pii.y - Pi.y);
			if (alpha > 0 && alpha < 1) {
				Temp = ((1 - alpha)*Pi) + alpha*Pii;
				if (Temp.x > point.x) {
					contador++;
				}
			}
		}
	}
	//returns parells
	if (contador % 2 == 1) {
		//std::cout << "CONTACTO" << std::endl;
		return true;
	}
	else {
		//std::cout << "NO" << std::endl;
		return false;
	}

}

Plane Polygon::colPlane(glm::vec3 point, glm::vec3 prevpoint)
{
	int contador = 0;
	float alpha;
	float minAlpha = 10;
	glm::vec3 Pi;
	glm::vec3 Pii;
	Plane CP;
	for (int i = 0; i < _listOfVertex.size(); i++) {
		Pi = _listOfVertex[i];
		//for ordenat
		if (i + 1 < _listOfVertex.size()) {
			Pii = _listOfVertex[i + 1];
		}
		else {
			Pii = _listOfVertex[0];
		}
		if (colSec(point, prevpoint, Pi, Pii) == true) {
			CP.create(Pi, Pii);
		}
	}
	//std::cout << " X:" << CP.normal.x << " Y ." << CP.normal.y << " Z :" << CP.normal.z << std::endl;
	return CP;
}

void Polygon::correction(Particle & p)
{
	Plane surface = colPlane(p.getCurrentPosition(), p.getPreviousPosition());

	glm::vec3 pp = (p.getPreviousPosition() - ((1 + p.getBouncing())*((p.getPreviousPosition()*surface.normal) + surface.dconst)*surface.normal));
	//glm::vec3 pp = (p.getPreviousPosition() - (1 + p.getBouncing())*surface.distPoint2Plane(p.getPreviousPosition())*surface.normal);
	glm::vec3 pv = -(p.getVelocity() - ((1 + p.getBouncing())*(surface.normal*p.getVelocity())*surface.normal));
	p.setPosition(pp);
	p.setVelocity(pv);
}
bool Polygon::colSec(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1.x - p0.x;     s1_y = p1.y - p0.y;
	s2_x = p3.x - p2.x;     s2_y = p3.y - p2.y;

	float s, t;
	s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		return true;
	}
	return false; // No collision
}

bool Sphere::isInside(const glm::vec3 & point)
{
	float dist = glm::length(point - _center);
	if (dist > _radius) {
		return false;
	}
	else {
		return true;
	}

}

void Sphere::checkNCorrect(Particle & point)
{
	glm::vec3 pTall;

	if (intersecSegment(point.getPreviousPosition(), point.getCurrentPosition(), pTall) == true) {
		intersecSegment(point.getPreviousPosition(), point.getCurrentPosition(), pTall);
		glm::vec3 normalTangent(point.getPreviousPosition().x - _center.x, point.getPreviousPosition().y - _center.y, point.getPreviousPosition().z - _center.z);
		Plane tangent(point.getPreviousPosition(), normalTangent);
		//float dist = glm::length(point.getPreviousPosition() - _center); //normalTangent.
		float dist = tangent.distPoint2Plane(point.getCurrentPosition());
		glm::vec3 correcPos = -(1 + point.getBouncing()) * dist *tangent.normal;
		glm::vec3 correcVel = -(1 + point.getBouncing()) * (point.getVelocity()*tangent.normal)*tangent.normal;
		point.setPosition(point.getCurrentPosition() + correcPos);
		point.setVelocity(point.getVelocity() + correcVel);
	}
	else {

	}
}

Sphere::Sphere()
{
	_radius = 0.2;
	_center.x = 0.0;
	_center.y = -0.7;
	_center.z = 0.0;

}

Sphere::~Sphere()
{
}

void Sphere::setPosition(const glm::vec3 & newPos)
{
}

bool Sphere::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	glm::vec3 v = point2 - point1;
	float a = glm::dot(v, v);
	float b = glm::dot((2.0f*v), (point1 - _center));
	float c = (glm::dot(_center, _center) + glm::dot(point1, point1)) + (glm::dot((-2.0f * point1), _center) - (_radius* _radius));
	float determinant = (b*b) - (4.0f * (a*c));
	if (determinant < glm::pow(10.0f, -7.0f)) {//no tindrem cap solucio per tant cap punt de tall
											   //	std::cout << "glm::sqrt((b*b) - (4 * a*c)) is not Real!!!" << std::endl;
		return false;
	}
	else if (determinant > 0.0f) {

		float alfa1 = ((-b + glm::sqrt(determinant)) / (2.0f * a));
		float alfa2 = ((-b - glm::sqrt(determinant)) / (2.0f * a));


		if (alfa1 <= 1 && alfa1 >= 0 && alfa2 <= 1 && alfa2 >= 0) {//el segment talla la esfere en dos punts
			if (alfa1 < alfa2) {//alfa 1 es la mes petita
				pTall = (point1 + (alfa1*v));
				return true;
			}
			else if (alfa2 <= alfa1) {// alfa 2 es la mes petita
				pTall = (point1 + (alfa2*v));
				return true;
			}
			return false;
		}
		else if (alfa1 <= 1 && alfa1 >= 0) { //alfa 1 es la bona
			pTall = (point1 + (alfa1*v));
			return true;
		}
		else if (alfa2 <= 1 && alfa2 >= 0) { // alfa 2 es la bona
			pTall = (point1 + (alfa2*v));
			return true;
		}
		else {  // cap de les alfes esta dins del segment
			return false;
		}

	}
	else if (determinant == 0.0f) {
		float alfa1 = ((-b + glm::sqrt(determinant) / 2.0f * a)); //Sempre sera el + en aquest cas

		if (alfa1 <= 1 && alfa1 >= 0) {//alfa 1 talla pero alfa 2 no
			pTall = (point1 + (alfa1*v));
			return true;
		}
		else {
			return false;
		}

	}
};