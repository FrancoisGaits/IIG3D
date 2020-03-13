#ifndef UVSPHERE_H
#define UVSPHERE_H

#include "mesh.h"
#include "Sphere.h"

#include <memory>
#include <functional>


class UVSphere : public Sphere {
public:
    explicit UVSphere(float radius, unsigned precision);

    std::string infoString() final;

private:
    Mesh generateSphereAttributes(float radius, unsigned precision);
};

/*------------------------------------------------------------------------------------------------------------------------*/


#endif
