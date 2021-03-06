#include "scene.h"
#include <iostream>


Scene::Scene(int width, int height, FragmentShader fs, VertexShader vs) :
        _width(width), _height(height),
        _activecamera(1), shader(vs, fs) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, width, height);

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
    spheres.clear();
    lights.clear();
    models.clear();
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


    _view = _camera->viewmatrix();

    for (const auto &sphere : spheres) {
        shader.use();

        if (shader.fragmentShader() == ERREUR) {
            shader.setInt("prec", sphere->precision());
            shader.setFloat("radius", sphere->radius());
        } else if (shader.fragmentShader() == BLINNPHONG) {
            shader.setVec3("cameraPos", _camera->position());
            shader.setInt("nbLight", lights.size());
            shader.setVec3("objectColor", sphere->color());
            int i = 0;
            for (const auto &light :  lights) {
                shader.setLight("lights[" + std::to_string(i) + "]", light);
                ++i;
            }
        }

        shader.setMat4fv("model", sphere->model());
        shader.setMat4fv("view", _view);
        shader.setMat4fv("projection", _projection);

        sphere->draw();
    }
    for (const auto &model : models) {
        shader.use();

        if (shader.fragmentShader() == ERREUR) {
            shader.setInt("prec", 0);
            shader.setFloat("radius", 1);
        } else if (shader.fragmentShader() == BLINNPHONG) {
            shader.setVec3("cameraPos", _camera->position());
            shader.setInt("nbLight", lights.size());
            shader.setVec3("objectColor", model->color());
            int i = 0;
            for (const auto &light :  lights) {
                shader.setLight("lights[" + std::to_string(i) + "]", light);
                ++i;
            }
        }

        shader.setMat4fv("model", model->model());
        shader.setMat4fv("view", _view);
        shader.setMat4fv("projection", _projection);

        model->draw();
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

void Scene::addPointLight(glm::vec3 position, glm::vec3 color) {
    lights.emplace_back(Light(POINT, position, color));
}

void Scene::addSpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 focus, float limit) {
    lights.emplace_back(Light(SPOT, position, color, focus, limit));
}

std::string Scene::sceneInfoString() {
    std::stringstream mess;
    for (const auto &sphere : spheres) {
        mess << sphere->infoString();
    }
    for (const auto &light : lights) {
        mess << light.infoString();
    }
    for (const auto &model : models) {
        mess << model->infoString();
    }


    return mess.str();
}

void Scene::addGeoSphere(float radius, unsigned precision, glm::vec3 position, glm::vec3 color) {
    spheres.emplace_back(new GeoSphere(radius, precision, color));
    spheres.back()->translate(position);
}

void Scene::addUVSphere(float radius, unsigned precision, glm::vec3 position, glm::vec3 color) {
    spheres.emplace_back(new UVSphere(radius, precision, color));
    spheres.back()->translate(position);
}

void Scene::addModel(const char *path, glm::vec3 position, glm::vec3 color, float div) {
    std::string p(path);
    models.emplace_back(new Model(p, color, div));
    models.back()->translate(position);

}

