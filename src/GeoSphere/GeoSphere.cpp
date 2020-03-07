#include "GeoSphere.h"
#include <iostream>


/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


#define deg2rad(x) float(M_PI)*(x)/180.f

GeoSphere::GeoSphere(int width, int height, FragmentShader fs, int precision, bool drawfill) : OpenGLDemo(width, height,
                                                                                                          drawfill),
                                                                                               _radius(0.4),
                                                                                               _precision(precision),
                                                                                               _fs(fs),
                                                                                               shader("../src/shaders/shader.vs",
                                                                                                      Shader::getShaderPath(
                                                                                                              fs).data()),
                                                                                               _activecamera(1),
                                                                                               _camera(nullptr) {

    if (_precision > 8)
        _precision = 8;
    generateGeoSphereAttributes(_precision - 1, 0.40);


    mesh.load();

    _cameraselector.emplace_back([]() -> Camera * { return new EulerCamera(glm::vec3(0.f, 0.f, 1.f)); });
    _cameraselector.emplace_back([]() -> Camera * {
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

    if (_fs == ERREUR) {
        shader.setInt("prec", _precision*_precision);
        shader.setFloat("radius", _radius);
    } else if (_fs == BLINNPHONG) {
        shader.setVec3("cameraPos", _camera->position());
    }

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

//https://github.com/caosdoar/spheres/blob/master/src/spheres.cpp
void GeoSphere::generateGeoSphereAttributes(unsigned nbDiv, float radius) {
    mesh.clear();

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

    for (unsigned div = 0; div < nbDiv; ++div) {
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

            //std::cout << d << " " << e << " " << f << std::endl;

            tmpMesh.addTri(a, d, f);
            tmpMesh.addTri(d, b, e);
            tmpMesh.addTri(e, c, f);
            tmpMesh.addTri(d, e, f);
        }
        tmpMesh.normals = tmpMesh.vertices;
        mesh = tmpMesh;
    }
    std::cout << "\ttriangles : " << mesh.nbTriangles() << std::endl;
}
