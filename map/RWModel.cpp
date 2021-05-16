#include "RWModel.h"
#include <map/RWObject.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using nlohmann::json;

RWModel::RWModel()
{
	this->objectsNumber = 0;
}

RWModel::~RWModel()
{
	if (this->objectsNumber) {
		delete[] this->objects;
	}
}

RWModel::RWModel(std::string modelName, int modelid)
{
	this->modelName = modelName;
	this->modelID = modelid;
	std::string filename = RWObject::modelsLocation + "/" + modelName + ".json";
	std::ifstream ifs(filename);
	json jf = json::parse(ifs);
	ifs.close();
	this->objectsNumber = jf["model"]["geometryList"]["numberOfGeometricObjects"];
	
	this->objects = new RWObject[this->objectsNumber];
	
	for (int objectIndex = 0; objectIndex < this->objectsNumber; objectIndex++) {
		this->objects[objectIndex].load(
			objectIndex,
			jf["model"]["geometryList"]["geometries"][objectIndex],
			jf["model"]["geometryList"]["textures"][objectIndex],
			jf["model"]["frameList"]["frames"][objectIndex],
			jf["model"]["atomics"][objectIndex]
		);
	}
	printf("Loaded object %s with texture %s\n", filename.c_str(), this->objects[0].textureName.c_str());
}


RWModel RWModel::load(std::string modelName, int modelid)
{
	RWModel model(modelName, modelid);
	return model;
}

void RWModel::setPosition(float x, float y, float z)
{
	for (int index = 0; index < this->objectsNumber; index++) {
		this->objects[index].setPosition(x, y, z);
	}
}

void RWModel::setRotation(float rx, float ry, float rz)
{
	for (int index = 0; index < this->objectsNumber; index++) {
		this->objects[index].setRotation(rx, ry, rz);
	}
}

void RWModel::setRotationRadians(float rx, float ry, float rz)
{
	rx = glm::degrees(rx);
	ry = glm::degrees(ry);
	rz = glm::degrees(rz);
	this->setRotation(rx, ry, rz);
}

void RWModel::setTexture(std::string textureName)
{
	for (int index = 0; index < this->objectsNumber; index++) {
		this->objects[index].setTexture(textureName);
	}
}
