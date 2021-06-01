#pragma once
#include <string>
#include <map>
#include <map/RWObject.h>
#include <nlohmann/json.hpp>
#include <vector>

class RWModel
{
	public:
		RWModel();
		~RWModel();
		RWModel(std::string modelName, int modelid);
		int objectsNumber;
		int modelID;
		std::string modelName;
		float width, length, height;
		float box[6];
		float col_x[2], col_y[2], col_z[2];
		float x, y, z = 0;
		RWObject* objects;
		//std::map<std::string, RWObject> objects;
		static RWModel load(std::string modelName, int modelid);
		void readCollisionFromFile(std::string modelName);
		void setPosition(float x, float y, float z);
		void setCollisionBoundry();
		void setRotation(float rx, float ry, float rz);
		void setRotationRadians(float rx, float ry, float rz);
		void setTexture(std::string textureName);
};

