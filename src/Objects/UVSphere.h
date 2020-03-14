#ifndef UVSPHERE_H
#define UVSPHERE_H

#include "mesh.h"
#include "Sphere.h"

#include <memory>
#include <functional>


class UVSphere : public Sphere {
public:
    explicit UVSphere(float radius, unsigned precision, glm::vec3 color = glm::vec3(1));

    std::string infoString() final;

private:
    static Mesh generateSphereAttributes(float radius, unsigned precision);
};

/*------------------------------------------------------------------------------------------------------------------------*/


#endif
