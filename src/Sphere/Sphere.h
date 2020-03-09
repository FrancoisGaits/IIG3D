#ifndef IIG3D_SPHERE_H
#define IIG3D_SPHERE_H

#include "mesh.h"

#include <memory>
#include <functional>


/** Simple drawing demonstration
 */
class Sphere {
public:
    explicit Sphere(float radius, unsigned precision);

    void draw();

    int precision();

    float radius();

    glm::mat4& model();

    ~Sphere() = default;

private:

    float _radius;
    int _precision;

    // matrices
    glm::mat4 _model;

protected:
    // A simple geometry
    Mesh mesh;

};


#endif
