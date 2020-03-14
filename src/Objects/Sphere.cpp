#include "Sphere.h"

Sphere::Sphere(float radius, unsigned precision, glm::vec3 color) : _radius(radius), _precision(precision),
                                                                    _color(color) {

}

unsigned Sphere::precision() {
    return _precision;
}

float Sphere::radius() {
    return _radius;
}

glm::mat4 &Sphere::model() {
    return _model;
}

void Sphere::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}

void Sphere::draw() {
    mesh.draw();
}

float Sphere::volume() {
    return mesh.volume();
}

unsigned Sphere::triangles() {
    return mesh.nbTriangles();
}

glm::vec3 &Sphere::color() {
    return _color;
}
