#include "GeoSphere.h"
#include <iostream>
#include <sstream>


GeoSphere::GeoSphere(float radius, unsigned precision, glm::vec3 color) : Sphere(radius, static_cast<unsigned>(
        pow(3.5, precision - 1) + 1), color) {

    mesh = generateSphereAttributes(radius, precision - 1);

    mesh.load();
}

Mesh GeoSphere::subdivide(Mesh &mesh, float radius) {
    Mesh tmpMesh;
    tmpMesh.vertices = mesh.vertices;
    cache.clear();
    for (unsigned i = 0; i < mesh.indices.size(); i += 3) {
        unsigned a = mesh.indices[i];        //   a
        unsigned b = mesh.indices[i + 1];      //  / \ ~
        unsigned c = mesh.indices[i + 2];      // b---c

        glm::vec3 v0 = glm::vec3(mesh.vertices[a * 3], mesh.vertices[a * 3 + 1], mesh.vertices[a * 3 + 2]);
        glm::vec3 v1 = glm::vec3(mesh.vertices[b * 3], mesh.vertices[b * 3 + 1], mesh.vertices[b * 3 + 2]);
        glm::vec3 v2 = glm::vec3(mesh.vertices[c * 3], mesh.vertices[c * 3 + 1], mesh.vertices[c * 3 + 2]);

        unsigned d = divideEdge(a, b, radius, v0, v1, tmpMesh);
        unsigned e = divideEdge(b, c, radius, v1, v2, tmpMesh);
        unsigned f = divideEdge(c, a, radius, v2, v0, tmpMesh);

        tmpMesh.addTri(a, d, f);
        tmpMesh.addTri(d, b, e);
        tmpMesh.addTri(e, c, f);
        tmpMesh.addTri(d, e, f);
    }
    tmpMesh.normals = tmpMesh.vertices;

    return tmpMesh;
}

unsigned GeoSphere::divideEdge(unsigned a, unsigned b, float radius, glm::vec3 &v1, glm::vec3 &v2, Mesh &tmpMesh) {
    if (a < b) {
        std::swap(a, b);
    }
    auto edge = std::pair<unsigned, unsigned>(a, b);
    auto it = cache.find(edge);
    if (it != cache.end()) {
        return it->second;
    }

    glm::vec3 v = glm::normalize((v1 + v2) / 2.f) * radius;

    unsigned ret = tmpMesh.nbVertices();
    tmpMesh.addVertex(v);
    cache.emplace(edge, ret);

    return ret;
}


Mesh GeoSphere::generateSphereAttributes(float radius, unsigned precision) {
    Mesh mesh;

    mesh.vertices = {
            c1 * radius, c2 * radius, 0,
            c2 * radius, 0, c1 * radius,
            0, c1 * radius, c2 * radius,

            -c1 * radius, c2 * radius, 0,
            0, c1 * radius, -c2 * radius,
            c2 * radius, 0, -c1 * radius,

            c1 * radius, -c2 * radius, 0,
            0, -c1 * radius, c2 * radius,
            -c2 * radius, 0, c1 * radius,

            -c2 * radius, 0, -c1 * radius,
            0, -c1 * radius, -c2 * radius,
            -c1 * radius, -c2 * radius, 0
    };

    mesh.normals = mesh.vertices;

    mesh.indices = {
            0, 2, 1,
            0, 1, 5,
            0, 5, 4,
            0, 4, 3,
            0, 3, 2,

            11, 10, 6,
            11, 6, 7,
            11, 7, 8,
            11, 8, 9,
            11, 9, 10,

            1, 7, 6,
            5, 6, 10,
            4, 10, 9,
            3, 9, 8,
            2, 8, 7,

            10, 4, 5,
            9, 3, 4,
            8, 2, 3,
            7, 1, 2,
            6, 5, 1
    };

    for (unsigned div = 0; div < precision; ++div) {
        mesh = subdivide(mesh, radius);
    }

    return mesh;
}

std::string GeoSphere::infoString() {
    std::stringstream mess;
    mess << "GeoSphere :" << std::endl;
    mess << "    -precision : " << precision() << std::endl;
    mess << "    -radius : " << radius() << std::endl;
    mess << "    -triangles : " << triangles() << std::endl;
    return mess.str();
}
