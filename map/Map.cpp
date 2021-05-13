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
	this->CreateObject(1509, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00);
}

std::string Map::getModelName(int modelID)
{
	std::string modelName = this->assignments[modelID];
	std::transform(modelName.begin(), modelName.end(), modelName.begin(), [](unsigned char c) { return std::tolower(c); });
	return modelName;
}

int Map::CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz)
{
	printf("%s\n", this->getModelName(modelid).c_str());
	this->objects.push_back(new RWModel(this->getModelName(modelid)));
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
