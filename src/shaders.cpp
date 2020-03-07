#include "shaders.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();


    GLint success;
    GLchar infoLog[512]; // warning fixed size ... request for LOG_LENGTH!!!
    GLuint vertexshader, fragmentshader;

    // 1. Generate the shader
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    // 2. set the source
    glShaderSource(vertexshader, 1, &vShaderCode, NULL);
    // 3. Compile
    glCompileShader(vertexshader);
    // 4. test for compile error
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 1. Generate the program
    ID = glCreateProgram();
    // 2. Attach the shaders to the program
    glAttachShader(ID, vertexshader);
    glAttachShader(ID, fragmentshader);
    // 3. Link the program
    glLinkProgram(ID);
    // 4. Test for link errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0)
        glUniform1i(loc, (int) value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setInt(const std::string &name, int value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0)
        glUniform1i(loc, value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setFloat(const std::string &name, float value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0)
        glUniform1f(loc, value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setMat4fv(const std::string &name, glm::mat4 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setVec3(const std::string &name, glm::vec3 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0)
        glUniform3f(loc, value.x, value.y, value.z);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}


std::string Shader::getShaderPath(FragmentShader fs) {
    std::string basePath = "../src/shaders/";
    switch(fs) {
        case LAMBERT :
            return basePath + "shaderLambert.fs";
        case FACETTE :
            return basePath + "shaderFacette.fs";
        case ERREUR :
            return basePath + "shaderErreur.fs";
        case BLINNPHONG :
            return basePath + "shaderBlinnPhong.fs";
        default:
            return basePath;
    }
}
