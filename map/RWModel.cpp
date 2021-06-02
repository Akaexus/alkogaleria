#include "RWModel.h"
#include <map/RWObject.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <sstream>

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
	this->x = x;
	this->y = y;
	this->z = z;
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

void RWModel::readCollisionFromFile(std::string objectName)
{
	
	std::fstream plik;
	std::string name = this->modelName, path = "../models/";
	path += name;
	name = path;
	name += ".cst";
	printf("\n;%s;\n", name.c_str());
	plik.open(name.c_str() , std::ios::in);
	std::string line1, line2, token;
	int iter = 0;
	while (std::getline(plik, line1)) {
		printf("%d\n", iter);
		if (line1[0] == '#' || line1[0] == 'C' || line1.length() == 0)
			continue;
		getline(plik, line2);
		std::stringstream ss(line2);
		while (std::getline(ss, token, ',')) {
			if (iter <= 5) {
				std::istringstream(token) >> this->box[iter];
				iter++;
			}
		}
		this->length = box[3];
		this->width = box[4];
		this->height = box[5];
		printf("%f %f %f\n", this->length, this->width, this->height);
	}
}

void RWModel::setCollisionBoundry() {
	this->col_x[0] = this->x + this->length / 2;
	this->col_x[1] = this->x - this->length / 2;
	this->col_y[0] = this->y + this->width / 2;
	this->col_y[1] = this->y - this->width / 2;
	this->col_z[0] = this->z + this->height / 2;
	this->col_z[1] = this->z - this->height / 2;
	printf("%d X:%f,%f Y:%f,%f Z:%f,%f", this->modelID, this->col_x[0], this->col_x[1], this->col_y[0], this->col_y[1], this->col_z[0], this->col_z[1]);
}
