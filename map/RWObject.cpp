#include "RWObject.h"
#include <libs/glew/include/GL/glew.h>
#include "lodepng.h"
#include <vector>
#include <libs/glew/include/GL/glew.h>
#include <libs/GLFW/include/GLFW/glfw3.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <map/Map.h>

const std::string RWObject::modelsLocation = "models";

RWObject::RWObject()
{
	this->vertexCount = 0;
}



RWObject::RWObject(int index, nlohmann::json geometry, nlohmann::json textures,	nlohmann::json frameList, nlohmann::json atomics)
{
	this->load(index, geometry, textures, frameList, atomics);
}

RWObject::~RWObject()
{
	if (this->vertexCount > 0) {
		delete[] this->vertices;
		delete[] this->vertexIndices;
		if (this->hasNormals) {
			delete[] this->normals;
		}

		if (this->hasTextures) {
			delete[] this->texCoords;
		}

		if (this->hasColors) {
			delete[] this->colors;
		}
		glDeleteTextures(1, this->texture);
	}
}

void RWObject::setPosition(float x, float y, float z)
{
	this->M = glm::translate(this->M, glm::vec3(x, y, z));
}

void RWObject::setRotation(float rx, float ry, float rz)
{
	glm::vec3 axis[] = { {1,0,0},{0,1,0},{0,0,1} };
	float angles[3] = { rx, ry, rz };
	for (int i = 0; i < 3; i++) {
		this->M = glm::rotate(this->M, glm::radians(angles[i]), axis[i]);
	}
}

void RWObject::initializeGeometry(nlohmann::json geometry)
{
	
	// VERTICES
	this->vertexCount = geometry["vertexInformation"].size();
	this->vertices = new float[this->vertexCount * 4];
	
	// VERTEX INDICES
	this->vertexIndicesCount = geometry["faceInformation"].size();
	this->vertexIndices = new int[this->vertexIndicesCount * 3];
	// NORMALS
	this->hasNormals = (bool)geometry["normalInformation"].size();

	if (this->hasNormals) {
		this->normals = new float[this->vertexCount * 4];
	}


	// TEXCOORDS
	this->hasTextures = (bool)geometry["textureMappingInformation"].size();
	if (this->hasTextures) {
		this->texCoords = new float[this->vertexCount * 2];
	}

	// COLORS
	this->hasColors = geometry["colorInformation"].size();
	if (this->hasColors) {
		this->colors = new float[this->vertexCount * 4];
	}
	

	// LOAD DATA
	for (int vertexIndex = 0; vertexIndex < this->vertexCount; vertexIndex++) {
		// VERTICES
		for (int coordIndex = 0; coordIndex < 3; coordIndex++) {
			this->vertices[4 * vertexIndex + coordIndex] = geometry["vertexInformation"][vertexIndex][coordIndex];
		}
		this->vertices[4 * vertexIndex + 3] = 1.0f;

		// NORMALS
		if (this->hasNormals) {
			for (int coordIndex = 0; coordIndex < 3; coordIndex++) {
				this->normals[4 * vertexIndex + coordIndex] = geometry["normalInformation"][vertexIndex][coordIndex];
			}
			this->normals[4 * vertexIndex + 3] = 0.0f;
		}

		// LOAD TEXTURE COORDS
		if (this->hasTextures) {
			for (int coordIndex = 0; coordIndex < 2; coordIndex++) {
				this->texCoords[2 * vertexIndex + coordIndex] = geometry["textureMappingInformation"][vertexIndex][coordIndex];
			}
		}

		// LOAD COLORS
		if (this->hasColors) {
			for (int coordIndex = 0; coordIndex < 4; coordIndex++) {
				this->colors[vertexIndex * 4 + coordIndex] = geometry["colorInformation"][vertexIndex][coordIndex] / 255.0;
			}
		}
	}
	/*
	(Face Information)
	{
		ARRAY OF SIZE : [Header - Triangle Count]
	}

	2 bytes - int - Vertex 2
		2 bytes - int - Vertex 1
		2 bytes - int - Flags
		2 bytes - int - Vertex 3
	*/
	for (int i = 0; i < this->vertexIndicesCount; i++) {
		this->vertexIndices[3 * i + 0] = geometry["faceInformation"][i][0];
		this->vertexIndices[3 * i + 1] = geometry["faceInformation"][i][1];
		this->vertexIndices[3 * i + 2] = geometry["faceInformation"][i][3];
	}

	// BOUDING SPHERE
	for (int i = 0; i < 4; i++) {
		this->boundingSphere[i] = geometry["boundingSphere"][i];
	}
}

void RWObject::load(int index, nlohmann::json geometry, nlohmann::json textures, nlohmann::json frameList, nlohmann::json atomics)
{
	
	this->index = index;
	this->initializeGeometry(geometry);
	if (this->hasTextures) {
		this->initializeTexture(geometry["materialList"], textures);
	}
	
	glm::mat3 temp;
	for (int col = 0; col < 3; col++) {
		for (int row = 0; row < 3; row++) {
			temp[col][row] = (float)frameList["rotationMatrix"][row * 3 + col];
		}
	}
	
	this->M = glm::mat4(temp);
}

void RWObject::initializeTexture(nlohmann::json materialList, nlohmann::json textures)
{
	this->textureColor = new float[4];
	for (int i = 0; i < 4; i++) {
		this->textureColor[i] = materialList["materialData"][0]["color"][i] / 255.0;
	}
	this->setTexture(materialList["materialData"][0]["texture"]["textureName"]);
}


void RWObject::setTexture(std::string textureName)
{
	this->textureName = textureName;
	std::map<std::string, GLuint>::iterator it = Map::textures.find(textureName);
	if (it == Map::textures.end()) { // texture was not loaded in shared storage
		Map::textures[textureName] = this->readTexture(textureName);
	}
	this->texture = &Map::textures[textureName];
}

GLuint RWObject::readTexture(std::string name) {
	std::string filename = RWObject::modelsLocation + "/" + name + ".png";
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename.c_str());
	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
