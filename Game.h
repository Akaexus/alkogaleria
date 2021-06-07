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
#include <vector>

class Game
{
	// ============= VARIABLES ===============
	// ------ POSITION --------
	public:
		float x, y, z; // positon
		float angle = 0;
		float alcoholicAngle = 0; // movement
		float alcoholicCameraAngle = 0;
		float playerHeight = 1.8f;
		float eyesHeight = 1.5f;
		float gravity = 5.6f;
		float jumpVelocity = 1.0f;
		float verticalAcceleration = 0.0f;
		float wallBuffer = 0.2f;
		Map* map;
		

	protected:
		float aspectRatio = 16 / 9;
		const float PLAYER_SPEED = 3;
		const float PLAYER_ROTATION_SPEED = PI / 2;
		const float BOTTLE_ROTATION_SPEED = PI / 5;
		float angle_direction = 0,
			direction_forward = 0,
			direction_side = 0,
			direction_vertical = 0;
	// MISC
	public:
		float alcoholLevel = 0;
		float itemUseDistance = 1.5f;
		float soberingSpeed = 0.005f;
	// OPENGL
	public:
		glm::mat4 P; // perspective matrix
		glm::mat4 V; // view matrix
	// objects
	public:
		std::vector<RWModel*> bottleHandlers;


	// ============= METHODS ================
	private:
		Game();
		void alcoholicAngleUpdate(float timeDifference);
	public:
		static Game& getInstance();
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
		static void keyCallback_handler(GLFWwindow* window, int key, int scancode, int action, int mod);
		void timePassed(float timeDifferrence);
		void updatePosition(float timeDifferrence);
		void sobering(float timeDifferrence);
		void spinBottles(float timeDifference);
		void updateVMatrix();
		void updatePerspectiveMatrix();
		static void windowResizeCallback_handler(GLFWwindow* window, int width, int height);
		void windowResizeCallback(GLFWwindow* window, int width, int height);
		void useItem();
		//void collision();
};

