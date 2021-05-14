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
	this->x = 0,
	this->y = 0,
	this->z = -1.5f,
	this->angle = 0,
	this->angle_direction = 0,
	this->direction_forward = 0,
	this->direction_side = 0,
	this->direction_vertical = 0;

	this->map = new Map;

	// initialize perspective matrix
	this->updatePerspectiveMatrix();
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
	this->x += (glm::sin(this->angle) * this->direction_forward + glm::cos(this->angle) * direction_side) * timeDifferrence;
	this->y += this->direction_vertical * timeDifferrence;
	this->z += (glm::sin(this->angle + 0.5 * PI) * this->direction_forward + glm::cos(this->angle + 0.5 * PI) * this->direction_side) * timeDifferrence;
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
	this->updateVMatrix();
}

void Game::updateVMatrix()
{
	this->V = glm::lookAt(
		glm::vec3(this->x, this->y, this->z), // position
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
		if (key == GLFW_KEY_KP_4) {
			this->angle_direction = PLAYER_ROTATION_SPEED;
		}
		else if (key == GLFW_KEY_KP_6) {
			this->angle_direction = -PLAYER_ROTATION_SPEED;
		}

		if (key == GLFW_KEY_W) {
			
			this->direction_forward = PLAYER_SPEED;
		}
		else if (key == GLFW_KEY_S) {
			this->direction_forward = -PLAYER_SPEED;
		}

		if (key == GLFW_KEY_A) {
			this->direction_side = PLAYER_SPEED;
		}
		else if (key == GLFW_KEY_D) {
			this->direction_side = -PLAYER_SPEED;
		}

		if (key == GLFW_KEY_LEFT_SHIFT) {
			this->direction_vertical = PLAYER_SPEED;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL) {
			this->direction_vertical = -PLAYER_SPEED;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_KP_4 || key == GLFW_KEY_KP_6) {
			this->angle_direction = 0;
		}

		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			this->direction_forward = 0;
		}

		if (key == GLFW_KEY_D || key == GLFW_KEY_A) {
			this->direction_side = 0;
		}

		if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_LEFT_SHIFT) {
			this->direction_vertical = 0;
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