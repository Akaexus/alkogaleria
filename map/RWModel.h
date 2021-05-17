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
		float box[10];
		RWObject* objects;
		//std::map<std::string, RWObject> objects;
		static RWModel load(std::string modelName, int modelid);
		void setPosition(float x, float y, float z);
		void setRotation(float rx, float ry, float rz);
		void setRotationRadians(float rx, float ry, float rz);
		void setTexture(std::string textureName);
		void setCollision(std::string modelName);
};

