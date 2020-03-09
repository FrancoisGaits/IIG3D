#include "Sphere.h"

Sphere::Sphere(float radius, unsigned precision) :_radius(radius), _precision(precision) {

}


int Sphere::precision() {
    return _precision;
}

float Sphere::radius() {
    return _radius;
}

glm::mat4& Sphere::model() {
    return _model;
}

void Sphere::draw() {
    mesh.draw();
}
