#include "Game.h"
#include <constants.h>
#include <libs/GLFW/include/GLFW/glfw3.h>

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}


Game::Game()
{
	srand(time(NULL));
	this->x = 0,
	this->y = 0,
	this->z = 0,
	this->angle = 0,
	this->angle_direction = 0,
	this->direction_forward = 0,
	this->direction_side = 0,
	this->direction_vertical = 0;

	this->map = new Map;

	// initialize perspective matrix
	this->updatePerspectiveMatrix();

	// grab bottle ids
	int bottleModelIDs[] = {
		19822,
		19821,
		19820,
		1509,
		1512,
		1517,
		1669,
		1544,
		19824,
		19823
	};
	for (int objectIndex = 0; objectIndex < this->map->objects.size(); objectIndex++) {
		for (int i = 0; i < sizeof(bottleModelIDs) / sizeof(int); i++) {
			if (this->map->objects[objectIndex]->modelID == bottleModelIDs[i]) {
				this->bottleHandlers.push_back(this->map->objects[objectIndex]);
				break;
			}
		}		
	}
}

void Game::updatePerspectiveMatrix() {
	this->P = glm::perspective(glm::radians(70.0f), this->aspectRatio, 0.1f, 60.0f); //Wylicz macierz rzutowania
}

/// <summary>
/// Update player's position based on timedfference
/// </summary>
/// <param name="timeDifferrence">Time difference in seconds</param>
void Game::updatePosition(float timeDifferrence)
{
	this->angle = fmod(this->angle + this->angle_direction * timeDifferrence, 2 * PI);
	float alcoholicAnglePart = std::clamp(this->alcoholLevel / 3.0f, 0.0f, 1.0f) * sin(this->alcoholicAngle);
	float fixed_angle = fmod(this->angle - alcoholicAnglePart * PI, 2 * PI);
	this->x += (glm::sin(fixed_angle) * this->direction_forward + glm::cos(fixed_angle) * direction_side) * timeDifferrence;
	this->y += this->direction_vertical * timeDifferrence;
	this->z += (glm::sin(fixed_angle + 0.5 * PI) * this->direction_forward + glm::cos(fixed_angle + 0.5 * PI) * this->direction_side) * timeDifferrence;
}

void Game::spinBottles(float timeDifference)
{
	for (int i = 0; i < this->bottleHandlers.size(); i++) {
		this->bottleHandlers[i]->setRotationRadians(0, 0, timeDifference * BOTTLE_ROTATION_SPEED);
	}
}

void Game::alcoholicAngleUpdate(float timeDifference)
{
	this->alcoholicAngle = fmod(this->alcoholicAngle + timeDifference * 1.1 * PI , 2 * PI);
	this->alcoholicCameraAngle = fmod(this->alcoholicCameraAngle + timeDifference * PI, 2 * PI);
}

void Game::useItem()
{
	// alcohol
	for (int i = 0; i < this->bottleHandlers.size(); i++) {
		float distance = sqrt(pow((this->bottleHandlers[i]->x - this->x), 2) +
			pow((this->bottleHandlers[i]->y - this->y), 2) +
			pow((this->bottleHandlers[i]->z - this->z), 2));
		if (distance <= this->itemUseDistance) {
			this->alcoholLevel += 0.2;
		}
	}
		
}
void Game::sobering(float timeDifferrence)
{
	if (this->alcoholLevel > 0) {
		this->alcoholLevel = std::max(0.0f, this->alcoholLevel - this->soberingSpeed * timeDifferrence);
	}
}
/// <summary>
/// Static wrapper for Game::keyCallback
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mod"></param>
void Game::keyCallback_handler(GLFWwindow* window, int key, int scancode, int action, int mod) {
	Game::getInstance().keyCallback(window, key, scancode, action, mod);
}

void Game::timePassed(float timeDifferrence)
{
	this->updatePosition(timeDifferrence);
	this->spinBottles(timeDifferrence);
	this->updateVMatrix();
	this->alcoholicAngleUpdate(timeDifferrence);
	this->sobering(timeDifferrence);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf(
		"gamePos: (%.2f, %.2f, %.2f), facing: %.2fdeg\n",
		this->x,
		this->y,
		this->z,
		this->angle * 180 / PI
	);
	printf(
		"alcohol(level: %.2f, movementAngle: %.2fdeg, cameraAngle: %.2fdeg)\n",
		this->alcoholLevel,
		this->alcoholicAngle * 180 / PI,
		this->alcoholicCameraAngle * 180 / PI
	);
}

void Game::updateVMatrix()
{
	float camera_amplitude = std::clamp(this->alcoholLevel, 0.0f, 5.0f) / 10;
	float horizontal_amplitude = cos(this->alcoholicCameraAngle) * camera_amplitude;
	float vertical_amplitude = sin(this->alcoholicCameraAngle) * camera_amplitude;

	float new_y = this->y + horizontal_amplitude;
	float new_x = this->x + (glm::sin(this->angle) * vertical_amplitude + glm::cos(this->angle) * vertical_amplitude);
	float new_z = this->z + (glm::sin(this->angle + 0.5 * PI) * vertical_amplitude + glm::cos(this->angle + 0.5 * PI) * vertical_amplitude);

	this->V = glm::lookAt(
		glm::vec3(new_x, new_y , new_z), // position
		glm::vec3(this->x + glm::sin(this->angle), this->y, this->z + glm::cos(this->angle)), // lookat
		glm::vec3(0.0f, 1.0f, 0.0f) // up vector
	);
}

/// <summary>
/// Generic key callback to do actions in game state
/// </summary>
/// <param name="window">OpenGL window</param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mod"></param>
void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	if (action == GLFW_PRESS) {

		switch (key) {
			// USE ELEMENT
			case GLFW_KEY_E:
				this->useItem();
				break;
			
			// IMMEDIATE SOBERING UP
			case GLFW_KEY_R:
				this->alcoholLevel = 0;
				break;


			// ANGLE
			case GLFW_KEY_KP_4:
				this->angle_direction = PLAYER_ROTATION_SPEED;
				break;
			case GLFW_KEY_KP_6:
				this->angle_direction = -PLAYER_ROTATION_SPEED;
				break;

			// HORIZONTAL
			case GLFW_KEY_W:
				this->direction_forward = PLAYER_SPEED;
				break;
			case GLFW_KEY_S:
				this->direction_forward = -PLAYER_SPEED;
				break;

			case GLFW_KEY_A:
				this->direction_side = PLAYER_SPEED;
				break;
			case GLFW_KEY_D:
				this->direction_side = -PLAYER_SPEED;
				break;

			// VERTICAL
			case GLFW_KEY_LEFT_SHIFT:
				this->direction_vertical = PLAYER_SPEED;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				this->direction_vertical = -PLAYER_SPEED;
				break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_KP_4:
			case GLFW_KEY_KP_6:
				this->angle_direction = 0;
				break;

			case GLFW_KEY_W:
			case GLFW_KEY_S:
				this->direction_forward = 0;
				break;

			case GLFW_KEY_A:
			case GLFW_KEY_D:
				this->direction_side = 0;
				break;

			case GLFW_KEY_LEFT_CONTROL:
			case GLFW_KEY_LEFT_SHIFT:
				this->direction_vertical = 0;
				break;
		}
	}
}

void Game::windowResizeCallback_handler(GLFWwindow* window, int width, int height) {
	Game::getInstance().windowResizeCallback(window, width, height);
}

void Game::windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) {
		return;
	}
	this->aspectRatio = (float)width / (float)height;
	this->updatePerspectiveMatrix();
	glViewport(0, 0, width, height);
}