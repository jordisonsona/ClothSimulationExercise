#include "Geometry.h"
#include "ErrorManagement.h"
#include <iostream>
#include <fstream>

using namespace std;




/*
* Constructor 
*/
Geometry::Geometry() {

	/*for (int i = 0; i < NUMBASICOBJECTS; i++)
	_verticesData[i] = 0;
	*/
	for (int i = 0; i < 2; i++) {

		_numVertices.push_back(36);
		_verticesData.push_back(new Vertex[_numVertices[i]]);
		if (_verticesData[i] == NULL)
			throw std::exception("System was not able to allocate memory");
		switch (i) {
		case WHITE_CUBE:
		case PARTICLES:
			//CUADRADO FRONTAL 
			_verticesData[i][0].setPosition(-1, -1, 0.0f);
			_verticesData[i][1].setPosition(1, -1, 0.0f);
			_verticesData[i][2].setPosition(-1, 1, 0.0f);

			_verticesData[i][3].setPosition(-1, 1, 0.0f);
			_verticesData[i][4].setPosition(1, -1, 0.0f);
			_verticesData[i][5].setPosition(1, 1, 0.0f);

			//CUADRADO LATERAL IZQUIERDO
			_verticesData[i][6].setPosition(-1, -1, -1.0f);
			_verticesData[i][7].setPosition(-1, -1, 0.0f);
			_verticesData[i][8].setPosition(-1, 1, 0.0f);

			_verticesData[i][9].setPosition(-1, -1, -1.0f);
			_verticesData[i][10].setPosition(-1, 1, 0.0f);
			_verticesData[i][11].setPosition(-1, -1, -1.0f);

			//CUADRADO DE DETRÁS
			_verticesData[i][12].setPosition(-1, -1, -1.0f);
			_verticesData[i][13].setPosition(1, -1, -1.0f);
			_verticesData[i][14].setPosition(1, 1, -1.0f);

			_verticesData[i][15].setPosition(-1, -1, -1.0f);
			_verticesData[i][16].setPosition(1, 1, -1.0f);
			_verticesData[i][17].setPosition(-1, 1, -1.0f);

			//CUADRADO LATERAL DERECHO
			_verticesData[i][18].setPosition(1, -1, 0.0f);
			_verticesData[i][19].setPosition(1, -1, -1.0f);
			_verticesData[i][20].setPosition(1, 1, -1.0f);

			_verticesData[i][21].setPosition(1, -1, 0.0f);
			_verticesData[i][22].setPosition(1, 1, 0.0f);
			_verticesData[i][23].setPosition(1, 1, -1.0f);

			//CUADRADO DE ARRIBA
			_verticesData[i][24].setPosition(-1, 1, 0.0f);
			_verticesData[i][25].setPosition(1, 1, 0.0f);
			_verticesData[i][26].setPosition(1, 1, -1.0f);

			_verticesData[i][27].setPosition(-1, 1, 0.0f);
			_verticesData[i][28].setPosition(1, 1, -1.0f);
			_verticesData[i][29].setPosition(-1, 1, -1.0f);

			//CUADRADO DE DEBAJO
			_verticesData[i][30].setPosition(-1, -1, 0.0f);
			_verticesData[i][31].setPosition(1, -1, 0.0f);
			_verticesData[i][32].setPosition(1, -1, -1.0f);

			_verticesData[i][33].setPosition(-1, -1, 0.0f);
			_verticesData[i][34].setPosition(1, -1, -1.0f);
			_verticesData[i][35].setPosition(-1, -1, -1.0f);
			break;
		}


		switch (i) {
		case WHITE_CUBE:
			for (int j = 0; j < _numVertices[i]; j++) {
				_verticesData[i][j].setColor(255, 255, 255, 255);
			}
			break;
		case PARTICLES:
			for (int k = 0; k < _numVertices[i]; k++) {
				_verticesData[i][k].setColor(0, 255, 0, 255);
			}
			break;
		default:
			break;
		}

	}

	_loader.loadAse("./resources/models/sphere.ASE", _numVertices, _verticesData, true);
}



Geometry::~Geometry(){
	for (int i = 0; i < NUMBASICOBJECTS; i++) {
		if (_verticesData[i]!=NULL)
			delete[] _verticesData[i];
	}
}

void Geometry::destroyObject(int i)
{
		_listOfObjects.erase(_listOfObjects.begin() + i);
}

/*
* Load the game elements from a text file
*/
void Geometry::loadGameElements(char fileName[100]){	
	/* Text format
	<number of game elements>
	<type of game element> <vec3 position> <angle> <vec3 rotation> <vec3 scale>	
	*/
	int numGameElements;
	GameObject tempObject;
	glm::vec3 vector3fElements;
	ifstream file;
	file.open(fileName);

	if (file.is_open()){
		file >> numGameElements;
		for (int i = 0; i < numGameElements; i++) {
			file >> tempObject._objectType;
			file >> vector3fElements.x;
			file >> vector3fElements.y;
			file >> vector3fElements.z;
			tempObject._translate = vector3fElements;
			file >> tempObject._angle;
			file >> vector3fElements.x;
			file >> vector3fElements.y;
			file >> vector3fElements.z;
			tempObject._rotation = vector3fElements;
			file >> vector3fElements.x;
			file >> vector3fElements.y;
			file >> vector3fElements.z;
			tempObject._scale = vector3fElements;

			_listOfObjects.push_back(tempObject);
		}
		file.close();
	}
	else{
		string message = "The file "+ string(fileName)+" doesn't exists";
		ErrorManagement::errorRunTime(message);
	}

}

/*
* Get the vertices data for an specific object
* @param objectID is the identifier of the requested object
* @return Vertex * is an array with all the vertices data
*/
Vertex * Geometry::getData(int objectID){
	return _verticesData[objectID];
}

/*
* Get the number of vertices for an specific object
* @param objectID is the identifier of the requested object
* @return int is the number of vertices
*/

int Geometry::getNumVertices(int objectID){
	return _numVertices[objectID];
}

/*
* Get the number of elements to render
*/
int Geometry::getNumGameElements() {
	return _listOfObjects.size();
}

/*
* Get the number of vertices of an specific game element
* @param objectID is the identifier of the requested object
*/
GameObject & Geometry::getGameElement(int objectID) {
	return (_listOfObjects[objectID]);
}