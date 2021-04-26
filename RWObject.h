#pragma once
#include <nlohmann/json.hpp>
#include <GL/glew.h>
#include <string>
class RWObject
{
	public:
		RWObject();
		RWObject(
			int index,
			nlohmann::json geometry,
			nlohmann::json textures,
			nlohmann::json frameList,
			nlohmann::json atomics
		);
		void load(
			int index,
			nlohmann::json geometry,
			nlohmann::json textures,
			nlohmann::json frameList,
			nlohmann::json atomics
		);

		~RWObject();
		int index;
		static const std::string modelsLocation;
		// GEOMETRY
		void initializeGeometry(nlohmann::json geometry);
		void initializeTexture(nlohmann::json materialList, nlohmann::json textures);
		int vertexCount;
		float *vertices;
		int *vertexIndices;
		int hasNormals;
		unsigned int vertexIndicesCount;
		
		float *normals;
		float *vertexNormals;
		bool hasTextures;
		float *texCoords;
		bool hasColors;
		float *colors;
		float boundingSphere[4];
		// TEXTURES
		float* textureColor;
		std::string textureName;
		static GLuint readTexture(std::string name);
		GLuint texture;



};

