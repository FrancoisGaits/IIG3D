#include "UVSphere.h"
#include <iostream>
#include <sstream>

UVSphere::UVSphere(float radius, unsigned precision, glm::vec3 color) : Sphere(radius, precision, color) {
    mesh = generateSphereAttributes(radius, precision);

    mesh.load();
}

Mesh UVSphere::generateSphereAttributes(float radius, unsigned precision) {
    unsigned nbParallels = precision + 3;
    unsigned nbMeridians = precision + 3;

    float phi = M_PI / 2;
    float theta = 0;
    float pasPhi = M_PI / (nbParallels - 1);
    float pasTheta = -2 * M_PI / (nbMeridians);

    unsigned nbPoints((nbParallels - 2) * nbMeridians + 2);

    Mesh mesh;

    for (unsigned para = 0; para < nbParallels; ++para) {
        theta = 0;
        float cphr = cosf(phi) * radius;
        float sphr = sinf(phi) * radius;

        //On évite la duplication au sommet
        if (para == 0 || para == nbParallels - 1) {
            mesh.addVertex(cosf(theta) * cphr,
                           sphr,
                           cphr * sinf(theta));
        } else {

            for (unsigned meri = 0; meri < nbMeridians; ++meri) {
                float sth = sinf(theta);
                float cth = cosf(theta);
                mesh.addVertex(cth * cphr,
                               sphr,
                               cphr * sth);


                theta += pasTheta;
            }
        }

        phi += pasPhi;
    }

    //Approximation de sphère
    mesh.normals = mesh.vertices;

    for (unsigned para = 0; para < nbParallels - 1; ++para) {
        if (para == 0) {
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {
                mesh.addTri(0, meri, meri + 1 > nbMeridians ? 1 : meri + 1);
            }
        } else if (para == nbParallels - 2) {
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {
                mesh.addTri(nbPoints - 1, nbPoints - meri - 1, nbPoints - (meri + 1 > nbMeridians ? 1 : meri + 1) - 1);
            }
        } else {
            unsigned dec = (nbMeridians * (para - 1));
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {                                  // a------------b
                unsigned a = meri + dec;                                                            // | \          |
                unsigned b = (meri == nbMeridians ? 1 : meri + 1) + dec;                            // |    \       |
                unsigned c = meri + nbMeridians + dec;                                              // |       \    |
                unsigned d = (meri == nbMeridians ? nbMeridians + 1 : meri + nbMeridians + 1) + dec;// |          \ |
                                                                                                    // d------------c
                mesh.addQuad(a, b, c, d);
            }
        }
    }

    return mesh;
}

std::string UVSphere::infoString() {
    std::stringstream mess;
    mess << "UVSphere :" << std::endl;
    mess << "    -precision : " << precision() << std::endl;
    mess << "    -radius : " << radius() << std::endl;
    mess << "    -triangles : " << triangles() << std::endl;
    return mess.str();
}
