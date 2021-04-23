#pragma once
#include <string>

class RWModel
{
	public:
		int vertexCount;
		float* vertices;
		int* vertexIndices;
		int vertexIndicesCount;
		float* normals;
		float* vertexNormals;
		float* texCoords;
		float* colors;
		std::string textureName;
		static RWModel load(std::string modelName, unsigned int frameNumber);
};

