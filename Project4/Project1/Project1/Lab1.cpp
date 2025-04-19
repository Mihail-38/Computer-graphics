#define GLEW_DLL
#define GLFW_DLL
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
glm::vec3 CameraPos = glm::vec3(0, 0, 5.0);
glm::vec3 CameraDirectional = glm::vec3(0, 0, -1.0);
glm::vec3 CameraUp = glm::vec3(0, 1, 0);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 512.0f / 2.0f;
float lastY = 512.0f / 2.0f;
float fov = 45.0f;
bool firstMouse = true;

// Обработка изменения размера окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Обработка ввода с мыши
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    CameraDirectional = glm::normalize(front);
} // Added closing curly brace

// Обработка колесика мыши
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
} // Added closing curly brace

void settingMat4(int ID, const char* name, glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
} // Added closing curly brace

void processInput(GLFWwindow* window) {
    float cameraSpeed = 0.05f; // Увеличена скорость камеры
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CameraPos += cameraSpeed * CameraDirectional;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CameraPos -= cameraSpeed * CameraDirectional;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        CameraPos -= glm::normalize(glm::cross(CameraDirectional, CameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CameraPos += glm::normalize(glm::cross(CameraDirectional, CameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
} // Added closing curly brace

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW: init failed: " << glewGetErrorString(glewInit()) << std::endl;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("vertex_shader.glsl", "fragm_shader.glsl");

    while (!glfwWindowShouldClose(window)) {
        // Обработка ввода каждый кадр
        processInput(window);

        glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float redVal = (sin(timeValue) / 2.0f) + 0.5f;
        float greenVal = (cos(timeValue) / 2.0f) + 0.5f;
        float blueVal = (sin(timeValue + 3.14f) / 2.0f) + 0.5f;

        shader.Use();

        // Обновление матрицы проекции и вида каждый кадр
        glm::mat4 projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.0f);
        settingMat4(shader.Program, "projection", projection);

        glm::mat4 view = glm::lookAt(CameraPos, CameraPos + CameraDirectional, CameraUp);
        settingMat4(shader.Program, "view", view);

        shader.SetUniform("frag_Color", redVal, greenVal, blueVal, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // Перемещено внутрь цикла
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
