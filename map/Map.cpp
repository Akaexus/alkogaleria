#include "Map.h"
#include <fstream>
#include <sstream>

std::map<std::string, GLuint> Map::textures;

Map::Map()
{
	
	std::ifstream infile("map/objects.ide");
	int modelID;
	std::string modelName;
	std::string line;
	while (std::getline(infile, line)) {
		std::stringstream linestream(line);
		linestream >> modelID >> modelName;
		this->assignments[modelID] = modelName;
	}
	//this->CreateObject(12920, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00);
	//
	
	int ceiling = this->CreateObject(8355, 0, 2, 0, 180, 0.00, 0.00); // sufit
	this->objects[ceiling]->setTexture("beige_64");
	int floor = this->CreateObject(8355, 0, -2, 0, 0.00, 0.00, 0.00); // podloga
	this->objects[floor]->setTexture("woodfloor1");
	int wall1 = this->CreateObject(8355, 0, -2, 7, 90, 0, 180); // sciana
	int wall2 = this->CreateObject(8355, 0, -2, -7, 90, 0, 0); // sciana
	int wall3 = this->CreateObject(8355, -20, -2, 0, 90, 0, -90); // sciana
	int wall4 = this->CreateObject(8355, 20, -2, 0, 90, 0, 90); // sciana
	//int wall3 = this->CreateObject(8355,  10, 0.00,   0, 0.00, 0.00,  90.00); // sciana
	//int wall4 = this->CreateObject(8355, -10, 0.00,   0, 90.00,   0.00,  90.00); // sciana
	
	this->objects[wall1]->setTexture("bank_wall1");
	this->objects[wall2]->setTexture("bank_wall1");
	this->objects[wall3]->setTexture("bank_wall1");
	this->objects[wall4]->setTexture("bank_wall1");
	
	// g³ówna dywanologia
	for (int i = -16; i <= 16; i += 4) {
		this->CreateObject(2631, i, -1.95, 0, 90.00, 0.00, 0.00);
	}
	
	int bottleModelIDs[] = { 19822,
	19821,
	19820,
	1509,
	1512,
	1517,
	1669,
	1544,
	19824,
	19823 };
	int bottleIndex = 0;
	int bottlesArraySize = sizeof(bottleModelIDs) / sizeof(int);

	// dywany boczne
	int multipliers[] = { -1, 1 };
	for (int i = 0; i < 4; i++) { // ilosc dywanów
		for (int j = 0; j < 2; j++) { // lewo prawo
			for (int k = 0; k < 2; k++) { // obie strony g³ownej œcie¿ki
				this->CreateObject(2631, multipliers[j] * 5.5 * i, -1.95, multipliers[k] * 3, 90.00, 0.00, 90.00);
				this->CreateObject(2631, multipliers[j] * 5.5 * i, -1.95, multipliers[k] * (3 + 4), 90.00, 0.00, 90.00);
				this->CreateObject(1271, multipliers[j] * 5.5 * i, -1.5,  multipliers[k] * (3 + 3), 0.00, 0.00, 0.00);
			}
		}
	}


	/*

	

	this->CreateObject(1271, 7.7587, 0.37, 3.246, 0.00, 0.00, 0.00);
	this->CreateObject(1271, 7.7587, 0.37, -0.504, 0.00, 0.00, 0.00);
	this->CreateObject(1271, 7.7587, 0.37, -4.254, 0.00, 0.00, 0.00);
	this->CreateObject(1271, 7.7587, 0.37, -8.254, 0.00, 0.00, 0.00);
	this->CreateObject(1271, 7.7587, 0.37, -12.114, 0.00, 0.00, 0.00);
	this->CreateObject(1271, -6.2413, 0.37, 3.246, 0.00, 0.00, 0.00);
	this->CreateObject(1271, -6.2413, 0.37, -0.504, 0.00, 0.00, 0.00);
	this->CreateObject(1271, -6.2413, 0.37, -4.254, 0.00, 0.00, 0.00);
	this->CreateObject(1271, -6.2413, 0.37, -8.254, 0.00, 0.00, 0.00);
	this->CreateObject(1271, -6.2413, 0.37, -12.114, 0.00, 0.00, 0.00);
	this->CreateObject(19822, -6.2413, 0.70, -12.114, 0.00, 0.00, 0.00);
	this->CreateObject(19821, -6.2413, 0.70, -8.254, 0.00, 0.00, 0.00);
	this->CreateObject(19820, -6.2413, 0.70, -4.254, 0.00, 0.00, 0.00);
	this->CreateObject(1509, -6.2413, 0.90, -0.504, 0.00, 0.00, 0.00);
	this->CreateObject(1512, -6.2413, 0.90, 3.246, 0.00, 0.00, 0.00);
	this->CreateObject(1517, 7.7587, 0.90, 3.246, 0.00, 0.00, 0.00);
	this->CreateObject(1669, 7.7587, 0.90, -0.504, 0.00, 0.00, 0.00);
	this->CreateObject(1544, 7.7587, 0.70, -4.254, 0.00, 0.00, 0.00);
	this->CreateObject(19824, 7.7587, 0.75, -8.254, 0.00, 0.00, 0.00);
	this->CreateObject(19823, 7.7587, 0.70, -12.114, 0.00, 0.00, 90.00);*/
	
}

std::string Map::getModelName(int modelID)
{
	std::string modelName = this->assignments[modelID];
	std::transform(modelName.begin(), modelName.end(), modelName.begin(), [](unsigned char c) { return std::tolower(c); });
	return modelName;
}

int Map::CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz)
{
	this->objects.push_back(new RWModel(this->getModelName(modelid), modelid));
	int index = this->objects.size() - 1;
	this->objects[index]->setPosition(x, y, z);
	this->objects[index]->setRotation(rx, ry, rz);
	return index;
}

int Map::CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz, std::string handler)
{
	int index = this->CreateObject(modelid, x, y, z, rx, ry, rz);
	this->object_handlers[handler] = this->objects[index];
	return 0;
}
