#include "RWModel.h"
#include <nlohmann/json.hpp>
#include <fstream>

using nlohmann::json;

RWModel RWModel::load(std::string modelName, unsigned int frameNumber)
{
	RWModel model;

	std::ifstream ifs(modelName);
	json jf = json::parse(ifs);
	ifs.close();
	model.vertexCount = jf["model"]["geometryList"]["geometries"][0]["vertexInformation"].size();
	model.vertexIndicesCount = jf["model"]["geometryList"]["geometries"][0]["faceInformation"].size();
	printf("vertex Count = %d", model.vertexCount);
	model.vertices = new float[model.vertexCount * 4];
	model.normals = new float[model.vertexCount * 4];
	model.texCoords = new float[model.vertexCount * 2];
	model.colors = new float[model.vertexCount * 4];
	model.vertexIndices = new int[model.vertexIndicesCount * 3];
	

	for (int i = 0; i < model.vertexCount; i++) {
		// LOAD VERTEX COORDINATES
		for (int j = 0; j < 3; j++) {
			model.vertices[4 * i + j] = jf["model"]["geometryList"]["geometries"][0]["vertexInformation"][i][j];
		}
		model.vertices[4 * i + 3] = 1.0f;

		// LOAD NORMAL VECTORS
		for (int j = 0; j < 3; j++) {
			model.normals[4 * i + j] = jf["model"]["geometryList"]["geometries"][0]["normalInformation"][i][j];
		}
		model.normals[4 * i + 3] = 0.0f;

		// LOAD TEXTURE COORDS
		for (int j = 0; j < 2; j++) {
			model.texCoords[2 * i + j] = jf["model"]["geometryList"]["geometries"][0]["textureMappingInformation"][i][j];
		}



		
		model.colors[4 * i + 0] = 1.0f;
		model.colors[4 * i + 1] = 0.0f;
		model.colors[4 * i + 2] = 0.0f;
		model.colors[4 * i + 3] = 1.0f;
	}
	for (int i = 0; i < model.vertexIndicesCount; i++) {
		model.vertexIndices[3 * i + 0] = jf["model"]["geometryList"]["geometries"][0]["faceInformation"][i][0];
		model.vertexIndices[3 * i + 1] = jf["model"]["geometryList"]["geometries"][0]["faceInformation"][i][1];
		model.vertexIndices[3 * i + 2] = jf["model"]["geometryList"]["geometries"][0]["faceInformation"][i][3];
	}
	model.textureName = jf["textures"]["textureDictionary"]["textureNatives"][0]["textureName"];
	//printf("\n%d %d %d\n", model.vertexIndices[0], model.vertexIndices[1], model.vertexIndices[2]);
    return model;
}
