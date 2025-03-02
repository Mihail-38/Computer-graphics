#include <glew.h>

#include <glfw3.h>

#include <iostream>

int main() {

	if (!glfwInit()) {
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		return -1;

	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POLYGON);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(-0.3f, -0.1f);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(-0.2f, -0.3f);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(0.1f, -0.3f);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(0.3f, -0.1f);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(0.1f, 0.1f);
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex2f(-0.1f, 0.5f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}