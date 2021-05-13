/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <libs/glew/include/GL/glew.h>
#include <libs/GLFW/include/GLFW/glfw3.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include <lodepng.h>
#include "shaderprogram.h"
#include <map/RWModel.h>
#include <map/RWObject.h>
#include <map/Map.h>
#include <nlohmann/json.hpp>
#include <fstream>

using nlohmann::json;

float x = 0,
	  y = 0,
	  z = -1.5f,
	  angle = 0,
	  angle_direction = 0,
	  angle_speed = PI/2,
	  direction_forward = 0,
	  direction_side = 0,
	  speed = 1,
	  direction_vertical = 0;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_KP_4) {
			angle_direction = angle_speed;
		}
		else if (key == GLFW_KEY_KP_6) {
			angle_direction = -angle_speed;
		}

		if (key == GLFW_KEY_W) {
			direction_forward = speed;
		}
		else if (key == GLFW_KEY_S) {
			direction_forward = -speed;
		}

		if (key == GLFW_KEY_A) {
			direction_side = speed;
		}
		else if (key == GLFW_KEY_D) {
			direction_side = -speed;
		}

		if (key == GLFW_KEY_LEFT_SHIFT) {
			direction_vertical = speed;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL) {
			direction_vertical = -speed;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_KP_4 || key == GLFW_KEY_KP_6) {
			angle_direction = 0;
		}

		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			direction_forward = 0;
		}

		if (key == GLFW_KEY_D || key == GLFW_KEY_A) {
			direction_side = 0;
		}

		if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_LEFT_SHIFT) {
			direction_vertical = 0;
		}
	}
}
RWModel* mp5;

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();

	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle, float x, float y, float z, Map* map) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 V = glm::lookAt(
		glm::vec3(x, y, z), // position
		glm::vec3(x + glm::sin(angle), y + 0.0f, z + glm::cos(angle)), // lookat
		glm::vec3(0.0f, 1.0f, 0.0f) // up vector
	); //Wylicz macierz widoku
	printf("angle: %f, x %f, y %f, z %f\n", (angle * 180) / PI, x, y, z);
	glm::mat4 P = glm::perspective(glm::radians(70.0f), 1.0f, 1.0f, 150.0f); //Wylicz macierz rzutowania


	spTextured->use(); //Aktywuj program cieniujący
	glUniform4f(spTextured->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku


	for (int modelIndex = 0; modelIndex < map->objects.size(); modelIndex++) {
		int objectsNumber = map->objects[modelIndex]->objectsNumber;
		for (int objectIndex = 0; objectIndex < objectsNumber; objectIndex++) {
			RWObject* object = &(map->objects[modelIndex]->objects[objectIndex]);
			glm::mat4 M = object->M;
			
			
			glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
			glEnableVertexAttribArray(spTextured->a("vertex"));

			glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, object->vertices);
			//glEnableVertexAttribArray(spLambertTextured->a("vertex"));
			//glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, mp5.vertices);

			//glEnableVertexAttribArray(spLambert->a("color"));
			//glVertexAttribPointer(spLambert->a("color"), 4, GL_FLOAT, false, 0, mp5.colors);

			if (object->hasNormals) {
				glEnableVertexAttribArray(spTextured->a("normal"));
				glVertexAttribPointer(spTextured->a("normal"), 4, GL_FLOAT, false, 0, object->normals);
			}

			glEnableVertexAttribArray(spTextured->a("texCoord"));
			glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, object->texCoords);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *(object->texture));
			glUniform1i(spTextured->u("tex"), 0);

			glDrawElements(
				GL_TRIANGLES,
				object->vertexIndicesCount * 3,
				GL_UNSIGNED_INT,
				object->vertexIndices
			);

			glDisableVertexAttribArray(spTextured->a("vertex"));
			//glDisableVertexAttribArray(spColored->a("color"));
			if (object->hasNormals) {
				glDisableVertexAttribArray(spTextured->a("normal"));
			}
			glDisableVertexAttribArray(spTextured->a("texCoord"));			
		}
	}
	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{

	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1280, 1280, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	Map map;

	glfwSetTime(0); //Wyzeruj licznik czasu
	
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float timeDifferrence = glfwGetTime();
		angle = fmod(angle + angle_direction * timeDifferrence, 2*PI);
		x += (glm::sin(angle) * direction_forward + glm::cos(angle) * direction_side) * timeDifferrence;
		y += direction_vertical * timeDifferrence;
		z += (glm::sin(angle + 0.5*PI) * direction_forward + glm::cos(angle + 0.5 * PI) * direction_side) * timeDifferrence;

		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, angle, x, y, z, &map); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
