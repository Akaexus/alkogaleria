#pragma once
#include <string>
#include <map>
#include <RWObject.h>
#include <nlohmann/json.hpp>
#include <vector>

class RWModel
{
	public:
		RWModel();
		~RWModel();
		RWModel(std::string modelName);
		int objectsNumber;
		RWObject* objects;
		//std::map<std::string, RWObject> objects;
		static RWModel load(std::string modelName);
};

