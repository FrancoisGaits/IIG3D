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
    explicit Sphere(float radius, unsigned precision);

    void draw();

    unsigned precision();
    float radius();
    unsigned triangles();
    glm::mat4& model();

    void translate(glm::vec3 vec);

    virtual std::string infoString() = 0;

    ~Sphere() = default;

private:

    float _radius;
    unsigned _precision;

    // matrices
    glm::mat4 _model;

protected:
    // A simple geometry
    Mesh mesh;

};


#endif
