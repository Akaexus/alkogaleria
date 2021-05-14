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
	this->CreateObject(8355, 0, 0.00, -7.0, 90.00, 0.00, 0.00); // podloga
	this->CreateObject(8355, 0, 4.00, -7.0, 90.00, 180.00, 0.00); // sufit
	this->CreateObject(8355, 0, 0.00, 10, 0.00, 0.00, 0.00); // sciana
	this->CreateObject(8355, 10, 0.00, -7, 0.00, 0.00, 90.00); // sciana
	this->CreateObject(8355, -10, 0.00, -7, 0.00, 0.00, 90.00); // sciana
	this->CreateObject(8355, 0, 0.00, -15, 0.00, 0.00, 180.00); // sciana

	//this->CreateObject(8355, 0.7387, 0.00, 48.976, 0.00, 0.00, 0.00);
	/*this->CreateObject(8355, 9.4387, 14.00, 40.816, 0.00, 0.00, -90.00);
	this->CreateObject(8355, -7.9313, 14.00, 48.926, 0.00, 0.00, 90.00);
	this->CreateObject(8355, 0.7587, 10.00, -18.784, 90.00, 90.00, 90.00);
	this->CreateObject(8355, -0.9413, 10.00, 5.156, 90.00, -90.00, 90.00);
	this->CreateObject(8355, -5.6413, 10.00, 32.576, 0.00, 0.00, 180.00);
	*/
	//this->CreateObject(2631, -32.8113, 14.49, 67.376, 0.00, 0.00, 0.00);
	//this->CreateObject(2631, 0.7587, 0.00, -19.924, 0.00, 0.00, 90.00);
	//this->CreateObject(2631, 0.7587, 0.00, -16.024, 0.00, 0.00, 90.00);
	
	this->CreateObject(2631, 0.7587, 0.00, 7.306, 0.00, 0.00, 90.00);
	this->CreateObject(2631, 0.7587, 0.00, 3.356, 0.00, 0.00, 90.00);
	this->CreateObject(2631, 0.7587, 0.00, -0.594, 0.00, 0.00, 90.00);
	this->CreateObject(2631, 0.7587, 0.00, -4.544, 0.00, 0.00, 90.00);
	this->CreateObject(2631, 0.7587, 0.00, -8.494, 0.00, 0.00, 90.00);
	this->CreateObject(2631, 0.7587, 0.00, -12.444, 0.00, 0.00, 90.00);


	this->CreateObject(2631, 3.6787, 0.00, -12.134, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 7.6287, 0.00, -12.144, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 3.6787, 0.00, -4.344, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 7.6287, 0.00, -4.344, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 3.6787, 0.00, 3.416, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 7.6287, 0.00, 3.416, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 3.6787, 0.00, -8.224, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 3.6787, 0.00, -0.484, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 7.6287, 0.00, -8.224, 0.00, 0.00, 0.00);
	this->CreateObject(2631, 7.6287, 0.00, -0.484, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -2.2013, 0.00, 3.416, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -6.1513, 0.00, 3.416, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -2.2013, 0.00, -0.484, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -6.1513, 0.00, -0.484, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -2.2013, 0.00, -4.344, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -6.1513, 0.00, -4.344, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -2.2013, 0.00, -8.224, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -6.1513, 0.00, -8.224, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -2.2013, 0.00, -12.134, 0.00, 0.00, 0.00);
	this->CreateObject(2631, -6.1513, 0.00, -12.134, 0.00, 0.00, 0.00);
	

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
	this->CreateObject(19823, 7.7587, 0.70, -12.114, 0.00, 0.00, 90.00);

}

std::string Map::getModelName(int modelID)
{
	std::string modelName = this->assignments[modelID];
	std::transform(modelName.begin(), modelName.end(), modelName.begin(), [](unsigned char c) { return std::tolower(c); });
	return modelName;
}

int Map::CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz)
{
	this->objects.push_back(new RWModel(this->getModelName(modelid)));
	int index = this->objects.size() - 1;
	this->objects[index]->setPosition(x, y, z);
	this->objects[index]->setRotation(rx-90, ry, rz);
	return index;
}

int Map::CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz, std::string handler)
{
	int index = this->CreateObject(modelid, x, y, z, rx, ry, rz);
	this->object_handlers[handler] = this->objects[index];
	return 0;
}
