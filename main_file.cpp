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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>


glm::vec3 bg;
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 col(0.755f, 0.613f, 0.152f);

float angle = 0.0f;
float speed = 2.0f * PI;

float lastFrame;


void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT)
			speed = -PI * 5;
		else if (key == GLFW_KEY_RIGHT)
			speed = PI * 5;
	}

	if (action == GLFW_RELEASE) {
		speed = 0.0f;
	}
	*/
}


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************

}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bg[0], bg[1], bg[2], 1.0f);
	

	glm::mat4 M(1.0f);
	glm::mat4 V = glm::lookAt(
		camPos,
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f)
	);
	glm::mat4 P = glm::perspective(50.0f * PI / 180.f, 1.0f, 1.0f, 50.0f);



	
	spLambert->use();
	glUniform4f(spLambert->u("color"), col[0], col[1], col[2], 1.0f);

	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(-1.15f, 0.0f, 0.0f));
	M = glm::rotate(M, angle * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));

	Models::torus.drawSolid(true);

	for (int i = 0; i < 10; ++i) {
		glm::mat4 Mc = M;
		Mc = glm::rotate(Mc, ((360 / 10) * i) * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		Mc = glm::translate(Mc, glm::vec3(0.0f, 1.0f, 0.0f));
		Mc = glm::scale(Mc, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mc));
		Models::cube.drawSolid(true);
	}


	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(1.15f, 0.0f, 0.0f));
	M = glm::rotate(M, - angle * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));

	Models::torus.drawSolid(true);

	for (int i = 0; i < 10; ++i) {
		glm::mat4 Mc = M;
		Mc = glm::rotate(Mc, ((360/ 10) * (i+0.5f)) * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		Mc = glm::translate(Mc, glm::vec3(0.0f, 1.0f, 0.0f));
		Mc = glm::scale(Mc, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mc));
		Models::cube.drawSolid(true);
	}
	/*
	M = glm::mat4(1.0f);
	M = glm::rotate(M, 90.0f * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, angle * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));

	Models::torus.drawWire(true);
	*/


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("debug");
		ImGui::Text((char*)glGetString(GL_VERSION));
		ImGui::ColorEdit3("background", (float*)& bg, ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit3("objColor", (float*)& col, ImGuiColorEditFlags_NoInputs);
		ImGui::SliderFloat3("camera", (float*)& camPos, -10.0f, 10.0f, "%.2f", 0);
		ImGui::SliderFloat("angle", &angle, 0.0f, 180.0f, "%.2f", 0);
		ImGui::SliderFloat("speed", &speed, 0.0f, 180.0f, "%.2f", 0);
		ImGui::End();
	}

}




int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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
	glEnable(GL_DEPTH_TEST);

	

	initOpenGLProgram(window); //Operacje inicjujące
	glfwSetKeyCallback(window, KeyboardCallback);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	

	//Główna pętla
	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		angle += speed * deltaTime;
		angle = std::fmod(angle, 180.0f);

		drawScene(window); //Wykonaj procedurę rysującą

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	exit(EXIT_SUCCESS);
}
