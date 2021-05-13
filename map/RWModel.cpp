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
	printf("RWModel destruct");
	if (this->objectsNumber) {
		delete[] this->objects;
	}
}

RWModel::RWModel(std::string modelName)
{
	std::string filename = RWObject::modelsLocation + "/" + modelName + ".json";
	printf("%s\n", filename.c_str());
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
}


RWModel RWModel::load(std::string modelName)
{
	RWModel model(modelName);
	return model;
}

void RWModel::setPosition(float x, float y, float z)
{
	// TODO
}

void RWModel::setRotation(float rx, float ry, float rz)
{
	for (int index = 0; index < this->objectsNumber; index++) {
		this->objects[index].setRotation(rx, ry, rz);
	}
}
