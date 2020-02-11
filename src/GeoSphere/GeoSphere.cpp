#include "GeoSphere.h"
#include <iostream>


/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


#define deg2rad(x) float(M_PI)*(x)/180.f
#define gr 1.61803398875f

GeoSphere::GeoSphere(int width, int height, std::string fsPath) : OpenGLDemo(width, height),
								  shader("../src/shaders/shader.vs", fsPath.data()),
								  _activecamera(1), _camera(nullptr) {

    generateGeoSphereAttributes(1, 0.40);


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

void GeoSphere::resize(int width, int height) {
    OpenGLDemo::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void GeoSphere::draw() {
    OpenGLDemo::draw();

    shader.use();

    _view = _camera->viewmatrix();

    shader.setMat4fv("model", _model);
    shader.setMat4fv("view", _view);
    shader.setMat4fv("projection", _projection);

    mesh.draw();
}

void GeoSphere::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void GeoSphere::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void GeoSphere::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool GeoSphere::keyboard(unsigned char k) {
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

//https://github.com/caosdoar/spheres/blob/master/src/spheres.cpp
void GeoSphere::generateGeoSphereAttributes(unsigned nbDiv, float radius) {
    float a = radius;

    mesh.clear();

    mesh.vertices = {
            a / 2.f, a * gr / 2.f, 0,
            a * gr / 2.f, 0, a / 2.f,
            0, a / 2.f, a * gr / 2.f,

            -a / 2.f, a * gr / 2.f, 0,
            0, a / 2.f, -a * gr / 2.f,
            a * gr / 2.f, 0, -a / 2.f,

            a / 2.f, -a * gr / 2.f, 0,
            0, -a / 2.f, a * gr / 2.f,
            -a * gr / 2.f, 0, a / 2.f,

            -a * gr / 2.f, 0, -a / 2.f,
            0, -a / 2.f, -a * gr / 2.f,
            -a / 2.f, -a * gr / 2.f, 0
    };

    mesh.normals = mesh.vertices;

    mesh.indices = {
            0, 2, 1,
            0, 1, 5,
            0, 5, 4,
            0, 4, 3,
            0, 3, 2,

            11, 6, 10,
            11, 7, 6,
            11, 8, 7,
            11, 9, 8,
            11, 10, 9,

            1, 7, 6,
            5, 6, 10,
            4, 10, 9,
            3, 9, 8,
            2, 8, 7,

            10, 5, 4,
            9, 4, 3,
            8, 2, 3,
            7, 1, 2,
            6, 5, 1
    };


    /* int o = 0;
     for(double i : mesh.vertices) {
         if(o++%3==0) {
             std::cout << "]" << std::endl << "[";
         }
         std::cout << i << (o%3 ? ",":"");
     }
     std::cout << std::endl;
 */
}
