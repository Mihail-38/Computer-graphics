
#define GLEW_DLL
#define GLFW_DLL
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"

float points[] = {
    -0.3f, -0.1f, 0.0f,
     -0.2f, -0.3f, 0.0f,
     0.1f, -0.3f, 0.0f,
     0.3f, -0.1f, 0.0f,
    0.1f, 0.1f, 0.0f,
    -0.1f, 0.5f, 0.0f
};


GLuint index[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 1
};

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   	GLFWwindow* window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);
  	if (!window) {
   		glfwTerminate();
  		return -1;
   	}
   	glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("vertex_shader.glsl", "fragm_shader.glsl");

    

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();


        float redVal = (sin(timeValue) / 2.0f) + 0.5f;
        float greenVal = (cos(timeValue) / 2.0f) + 0.5f;
        float blueVal = (sin(timeValue + 3.14f) / 2.0f) + 0.5f;

        shader.Use();
        shader.SetUniform("frag_Color", redVal, greenVal, blueVal, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}