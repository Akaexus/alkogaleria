#include <string>
#include <map/RWModel.h>
#include <libs/glew/include/GL/glew.h>
#include <map>
#pragma once
class Map
{
	public:
		std::map<int, std::string> assignments;
		static std::map<std::string, GLuint> textures;
		std::vector<RWModel*> objects;
		std::map<std::string, RWModel*> object_handlers;
		Map();
		std::string getModelName(int modelID);
		float lightColors[4 * 2];
		int CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz); // @see https://sampwiki.blast.hk/wiki/CreateObject
		int CreateObject(int modelid, float x, float y, float z, float rx, float ry, float rz, std::string handler);
		float lightSources[4 * 2];
		


};

