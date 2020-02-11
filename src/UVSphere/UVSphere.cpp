#include "UVSphere.h"
#include <iostream>


/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


#define deg2rad(x) float(M_PI)*(x)/180.f

UVSphere::UVSphere(int width, int height, std::string fsPath) : OpenGLDemo(width, height),
								shader("../src/shaders/shader.vs", fsPath.data()),
								_activecamera(1), _camera(nullptr) {

    generateUVSphereAttributes(50, 50, 0.35);

    mesh.load();

    _cameraselector.push_back([]() -> Camera * { return new EulerCamera(glm::vec3(0.f, 0.f, 1.f)); });
    _cameraselector.push_back([]() -> Camera * {
        return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
    });

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void UVSphere::resize(int width, int height) {
    OpenGLDemo::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void UVSphere::draw() {
    OpenGLDemo::draw();

    shader.use();

    _view = _camera->viewmatrix();

    shader.setMat4fv("model", _model);
    shader.setMat4fv("view", _view);
    shader.setMat4fv("projection", _projection);

    mesh.draw();
}

void UVSphere::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void UVSphere::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void UVSphere::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool UVSphere::keyboard(unsigned char k) {
    switch (k) {
        case 'p':
            _activecamera = (_activecamera + 1) % 2;
            _camera.reset(_cameraselector[_activecamera]());
            _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
            return true;
        default:
            return false;
    }
}

void UVSphere::generateUVSphereAttributes(unsigned nbParallels, unsigned nbMeridians, float radius) {
    float theta = M_PI / 2;
    float phi = 0;
    float pasTheta = M_PI / (nbParallels - 1);
    float pasPhi = 2.f * M_PI / (nbMeridians);

    unsigned nbPoints((nbParallels - 2) * nbMeridians + 2);

    mesh.clear();

    for (unsigned para = 0; para < nbParallels; ++para) {
        phi = 0;
        if (para == 0 || para == nbParallels - 1) {
            mesh.addVertex(cosf(theta) * cosf(phi) * radius,
                           sinf(theta) * radius,
                           cosf(theta) * sinf(phi) * radius);
        } else {
            for (unsigned meri = 0; meri < nbMeridians; ++meri) {
                mesh.addVertex(cosf(theta) * cosf(phi) * radius,
                               sinf(theta) * radius,
                               cosf(theta) * sinf(phi) * radius);

                phi += pasPhi;
            }
        }

        theta += pasTheta;
    }

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
}
