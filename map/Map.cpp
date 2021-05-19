#include "Map.h"
#include <fstream>
#include <sstream>

std::map<std::string, GLuint> Map::textures;

Map::Map() :
	lightSources{
		10.0, 1.2, 0.0, 1.0,
		-10.0, 1.2, 0.0, 1.0,
	}
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


	int ceiling = this->CreateObject(8355, 0, 2, 0, 180, 0.00, 0.00); // sufit
	this->objects[ceiling]->setTexture("beige_64");
	int floor = this->CreateObject(8355, 0, -2, 0, 0.00, 0.00, 0.00); // podloga
	this->objects[floor]->setTexture("woodfloor1");
	int wall1 = this->CreateObject(8355, 0, -2, 7, 90, 0, 180); // sciana
	int wall2 = this->CreateObject(8355, 0, -2, -7, 90, 0, 0); // sciana
	int wall3 = this->CreateObject(8356, -20, -2, 0, -90, 0, -90); // sciana
	int wall4 = this->CreateObject(8356, 20, -2, 0, -90, 0, 90); // sciana

	this->objects[wall1]->setTexture("conc_slabgrey_256128");
	this->objects[wall2]->setTexture("conc_slabgrey_256128");
	this->objects[wall3]->setTexture("conc_slabgrey_256128");
	this->objects[wall4]->setTexture("conc_slabgrey_256128");

	// g³ówna dywanologia
	for (int i = -16; i <= 16; i += 4) {
		this->CreateObject(2631, i, -1.95, 0, 90.00, 0.00, 0.00);
	}

	float bottleYDifferences[] = {
		0,
		0,
		0,
		0.2,
		0.2,
		0.2,
		0.18,
		0,
		0,
		0
	};

	int bottleModelIDs[] = {
		19822,
		19821,
		19820,
		1509,
		1512,
		1517,
		1669,
		1544,
		19824,
		19823
	};
	int bottleIndex = 0;
	int bottlesArraySize = sizeof(bottleModelIDs) / sizeof(int);
	// dywany boczne

	float distanceBetween = 5.5;
	float sideDistance = 3;
	int carpetsNumber = 4;
	int sides[] = { -1, 1 };

	for (float x = -carpetsNumber * distanceBetween; x <= carpetsNumber * distanceBetween; x += distanceBetween) {
		for (float z = -sideDistance; z <= sideDistance; z += 2 * sideDistance) {
			this->CreateObject(2631, x, -1.95, z, 90, 0, 90);
			this->CreateObject(2631, x, -1.95, z + (z > 0 ? 4 : -4), 90, 0, 90);
			this->CreateObject(1271, x, -1.5, z + (z > 0 ? 3 : -3), 0, 0, 0);
			this->CreateObject(bottleModelIDs[bottleIndex], x, -1.2 + bottleYDifferences[bottleIndex], z + (z > 0 ? 3 : -3), 90, 0, 0);
			bottleIndex = (bottleIndex + 1) % bottlesArraySize;
		}
	}
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
