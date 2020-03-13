#include "Sphere.h"

Sphere::Sphere(float radius, unsigned precision) :_radius(radius), _precision(precision) {

}

unsigned Sphere::precision() {
    return _precision;
}

float Sphere::radius() {
    return _radius;
}

glm::mat4& Sphere::model() {
    return _model;
}

void Sphere::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}

void Sphere::draw() {
    mesh.draw();
}

unsigned Sphere::triangles() {
    return mesh.nbTriangles();
}
