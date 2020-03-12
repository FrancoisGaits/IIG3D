#ifndef GEOSPHERE_H
#define GEOSPHERE_H

#include "mesh.h"
#include "Sphere.h"

#include <memory>
#include <functional>
#include <map>
#include <utility>


class GeoSphere : public Sphere {
public:
    explicit GeoSphere(float radius, unsigned precision);

private:
    unsigned divideEdge(unsigned a, unsigned b, float radius, glm::vec3& v1, glm::vec3& v2, Mesh& tmpMesh);

    Mesh generateSphereAttributes(float radius, unsigned precision);

    std::map<std::pair<unsigned,unsigned>,unsigned> cache;

    Mesh subdivide(Mesh &mesh, float radius);

    const float c1 = 1.f/sqrtf(1 + 1.61803398875f*1.61803398875f);
    const float c2 = 1.61803398875f*c1;
};

/*------------------------------------------------------------------------------------------------------------------------*/


#endif
