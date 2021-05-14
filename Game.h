#pragma once

#include <map/RWModel.h>
#include <map/RWObject.h>
#include <map/Map.h>
#include <libs/glew/include/GL/glew.h>
#include <libs/GLFW/include/GLFW/glfw3.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <constants.h>

class Game
{
	// ============= VARIABLES ===============
	// ------ POSITION --------
	public:
		float x, y, z; // positon
		float angle = 0;
		Map* map;

	protected:
		const float PLAYER_SPEED = 3;
		const float PLAYER_ROTATION_SPEED = PI / 2;
		float angle_direction = 0,
			direction_forward = 0,
			direction_side = 0,
			direction_vertical;
	// OPENGL
	public:
		glm::mat4 P; // perspective matrix
		glm::mat4 V; // view matrix


	// ============= METHODS ================
	private:
		Game();
	public:
		static Game& getInstance();
		void updatePosition(float timeDifferrence);
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
		static void keyCallback_handler(GLFWwindow* window, int key, int scancode, int action, int mod);
		void timePassed(float timeDifferrence);
		void updateVMatrix();

};

