#include "RWModel.h"
#include <RWObject.h>
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

RWModel::RWModel(std::string modelName)
{
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
}


RWModel RWModel::load(std::string modelName)
{
	RWModel model(modelName);
	return model;
}
