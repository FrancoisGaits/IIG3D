#include "scene.h"
#include <iostream>


Scene::Scene(int width, int height, bool drawfill, unsigned precision, FragmentShader fs, VertexShader vs) :
                                                                    _width(width), _height(height), _drawfill(drawfill),
                                                                    _activecamera(1), shader(vs, fs) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, width, height);

    spheres.emplace_back(new GeoSphere(0.3,precision));

    _cameraselector.emplace_back([]() -> Camera * { return new EulerCamera(glm::vec3(0.f, 0.f, 1.f)); });
    _cameraselector.emplace_back([]() -> Camera * {
        return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
    });

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);

}

Scene::~Scene() {
}

void Scene::resize(int width, int height) {
   _width = width;
   _height = height;
   _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
   _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void Scene::draw() {
    glClearColor(0.05f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (_drawfill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    _view = _camera->viewmatrix();

    for(const auto & sphere : spheres) {
        shader.use();

        if (shader.fragmentShader() == ERREUR) {
            shader.setInt("prec", sphere->precision());
            shader.setFloat("radius", sphere->radius());
        } else if (shader.fragmentShader() == BLINNPHONG) {
            shader.setVec3("cameraPos", _camera->position());
        }

        shader.setMat4fv("model", sphere->model());
        shader.setMat4fv("view", _view);
        shader.setMat4fv("projection", _projection);

        sphere->draw();
    }

}

void Scene::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void Scene::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void Scene::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool Scene::keyboard(unsigned char k) {
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

void Scene::switchFragmentShader(FragmentShader fs) {

}

void Scene::toggledrawmode() {
    _drawfill = !_drawfill;
}

