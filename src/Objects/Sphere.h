#ifndef IIG3D_SPHERE_H
#define IIG3D_SPHERE_H

#include "mesh.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <memory>
#include <functional>


/** Simple drawing demonstration
 */
class Sphere {
public:
    explicit Sphere(float radius, unsigned precision, glm::vec3 color = glm::vec3(1));

    void draw();

    unsigned precision();
    float radius();
    glm::vec3& color();
    unsigned triangles();
    glm::mat4& model();

    void translate(glm::vec3 vec);

    virtual std::string infoString() = 0;

    ~Sphere() = default;

private:

    float _radius;
    unsigned _precision;
    glm::vec3 _color;

    glm::mat4 _model;

protected:

    Mesh mesh;

};


#endif
