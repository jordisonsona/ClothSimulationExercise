#pragma once
#include "Vertex.h"
#include <vector>
#include "GameObject.h"
#include "ObjectLoader.h"


#define WHITE_CUBE 0
#define PARTICLES 1
#define SPHERE 2
#define NUMBASICOBJECTS 3
#define MAXVERTICESSQUARE 36

//This class stores and manipulates all the objects loaded from the text file
class Geometry
{
	
	
public:

	std::vector<Vertex *> _verticesData;
	std::vector <int> _numVertices;
	std::vector <GameObject> _listOfObjects;

	Geometry();
	~Geometry();
	void destroyObject(int i);
	void loadGameElements(char fileName[100]);
	Vertex * getData(int objectID);
	int getNumVertices(int objectID);
	int getNumGameElements();
	GameObject & getGameElement(int objectID);
	ObjectLoader _loader;
	void setPosition();
	void isInside(); 
	
};

