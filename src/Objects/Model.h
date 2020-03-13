#ifndef IIG3D_MODEL_H
#define IIG3D_MODEL_H

#include <iostream>

#include "glm.hpp"
#include "mesh.h"
#include "glm/gtc/matrix_transform.hpp"

class Model {
public:
    explicit Model(std::string& path, glm::vec3 color = glm::vec3(1));

    void draw() const;

    const glm::vec3& color() const;
    const glm::mat4& model() const;

    Mesh loadMesh(std::string & path);

    std::string infoString() const;

    void translate(glm::vec3 vec);

private:
    glm::vec3 _color;

    glm::mat4 _model;

    Mesh mesh;
};

#endif
