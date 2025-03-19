#include "Shader.h"
using namespace std;
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

    string vertexCode = ReadFile(vertexPath);
    string fragmentCode = ReadFile(fragmentPath);

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glShaderSource(fragment, 1, &fShaderCode, NULL);

    GLint success;
    GLchar infoLog[1024];
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        exit(1);
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        exit(1);
    }

    Program = glCreateProgram();
    glAttachShader(Program, vertex);
    glAttachShader(Program, fragment);
    glLinkProgram(Program);

    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Program, 1024, NULL, infoLog);
        cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        exit(1);
    }


    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const {
    glUseProgram(Program);
}
std::string Shader::ReadFile(const GLchar* filename) {
    try {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    catch (const std::ifstream::failure& e) {
        cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filename << std::endl;
        return "";
    }
}


void Shader::SetUniform(const std::string& name, float value) {
    Use();
    glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
}

void Shader::SetUniform(const string& name, int value) {
    Use();
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}

void Shader::SetUniform(const string& name, float x, float y, float z) {
    Use();
    glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z);
}

void Shader::SetUniform(const string& name, float r, float g, float b, float a) {
    Use();
    glUniform4f(glGetUniformLocation(Program, name.c_str()), r, g, b, a);
}

void Shader::SetUniform(const string& name, float* matrix) const {
    Use();
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, matrix);
}