#ifndef SHADERS_H
#define SHADERS_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum FragmentShader {
    LAMBERT,
    FACETTE,
    ERREUR,
    BLINNPHONG
};

enum VertexShader {
    VERTEX
};


class Shader {
private:
    unsigned int ID;

    VertexShader _vertexShader;
    FragmentShader _fragmentShader;

    std::string _basePath = "../src/shaders/";


public:
    Shader(VertexShader vs, FragmentShader fs);

    std::string getFragmentShaderPath();

    std::string getVertexShaderPath();

    FragmentShader fragmentShader();
    VertexShader vertexShader();

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec3(const std::string &name, glm::vec3 &value) const;

    void setMat4fv(const std::string &name, glm::mat4 &value) const;

    ~Shader();

};


#endif
