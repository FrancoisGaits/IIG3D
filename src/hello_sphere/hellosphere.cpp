#include "hellosphere.h"
#include <iostream>


/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


#define deg2rad(x) float(M_PI)*(x)/180.f
#define gr 1.61803398875f

SimpleSphere::SimpleSphere(int width, int height) : OpenGLDemo(width, height),
                                                    shader("../src/shaders/shader.vs", "../src/shaders/shader.fs"),
                                                    _activecamera(0), _camera(nullptr) {

    //generateUVSphereAttributes(100, 100, 0.35, _vertices, _normals, _indices);
    generateGeoSphereAttributes(1,0.40, _vertices, _normals, _indices);

// Initialize the geometry
    // 1. Generate geometry buffers
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_nbo);
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);
    // 2. Bind Vertex Array Object
    glBindVertexArray(_vao);
    // 3. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
    // 4. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    // 5. Copy our normals array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
    // 6. Copy our vertices array in a buffer for OpenGL to use
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    // 7. Copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLfloat), _indices.data(), GL_STATIC_DRAW);
    //6. Unbind the VAO
    glBindVertexArray(0);


    _cameraselector.push_back([]() -> Camera * { return new EulerCamera(glm::vec3(0.f, 0.f, 1.f)); });
    _cameraselector.push_back([]() -> Camera * {
        return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
    });

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

SimpleSphere::~SimpleSphere() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}

void SimpleSphere::resize(int width, int height) {
    OpenGLDemo::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void SimpleSphere::draw() {
    OpenGLDemo::draw();

    shader.use();

    _view = _camera->viewmatrix();

    shader.setMat4fv("model", _model);
    shader.setMat4fv("view", _view);
    shader.setMat4fv("projection", _projection);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SimpleSphere::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void SimpleSphere::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void SimpleSphere::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool SimpleSphere::keyboard(unsigned char k) {
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

void SimpleSphere::generateUVSphereAttributes(unsigned nbParallels, unsigned nbMeridians, float radius,
                                            std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals,
                                            std::vector<GLuint> &indices) {
    float theta = M_PI / 2;
    float phi = 0;
    float pasTheta = M_PI / (nbParallels - 1);
    float pasPhi = 2.f * M_PI / (nbMeridians);

    unsigned nbPoints((nbParallels - 2) * nbMeridians + 2);

    for (unsigned para = 0; para < nbParallels; ++para) {
        phi = 0;
        if (para == 0 || para == nbParallels - 1) {
            vertices.emplace_back(cosf(theta) * cosf(phi) * radius);
            vertices.emplace_back(sinf(theta) * radius);
            vertices.emplace_back(cosf(theta) * sinf(phi) * radius);
        } else {
            for (unsigned meri = 0; meri < nbMeridians; ++meri) {
                vertices.emplace_back(cosf(theta) * cosf(phi) * radius);
                vertices.emplace_back(sinf(theta) * radius);
                vertices.emplace_back(cosf(theta) * sinf(phi) * radius);

                phi += pasPhi;
            }
        }

        theta += pasTheta;
    }

    normals = vertices;

    for (unsigned para = 0; para < nbParallels - 1; ++para) {
        if (para == 0) {
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {
                indices.emplace_back(0);
                indices.emplace_back(meri);
                indices.emplace_back(meri + 1 > nbMeridians ? 1 : meri + 1);
            }
        } else if (para == nbParallels - 2) {
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {
                indices.emplace_back(nbPoints - 1);
                indices.emplace_back(nbPoints - meri - 1);
                indices.emplace_back(nbPoints - (meri + 1 > nbMeridians ? 1 : meri + 1) - 1);
            }
        } else {
            unsigned dec = (nbMeridians * (para - 1));
            for (unsigned meri = 1; meri <= nbMeridians; ++meri) {                                  // a------------b
                unsigned a = meri + dec;                                                            // | \          |
                unsigned b = (meri == nbMeridians ? 1 : meri + 1) + dec;                            // |    \       |
                unsigned c = meri + nbMeridians + dec;                                              // |       \    |
                unsigned d = (meri == nbMeridians ? nbMeridians + 1 : meri + nbMeridians + 1) + dec;// |          \ |
                //triangle inferieur                                                                // d------------c
                indices.emplace_back(a);
                indices.emplace_back(c);
                indices.emplace_back(d);
                //triangle superieur
                indices.emplace_back(a);
                indices.emplace_back(d);
                indices.emplace_back(b);
            }
        }
    }
}

void computeHalfVertex(float radius, const float v1[3], const float v2[3], float newV[3])
{
    newV[0] = v1[0] + v2[0];    // x
    newV[1] = v1[1] + v2[1];    // y
    newV[2] = v1[2] + v2[2];    // z
    float scale = radius / sqrtf(newV[0]*newV[0] + newV[1]*newV[1] + newV[2]*newV[2]);
    newV[0] *= scale;
    newV[1] *= scale;
    newV[2] *= scale;
}

void SimpleSphere::generateGeoSphereAttributes(unsigned nbDiv, float radius, std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, std::vector<GLuint> &indices) {
    float a = radius;
    vertices = {
            a/2.f, a*gr/2.f, 0,
            a*gr/2.f, 0, a/2.f,
            0, a/2.f, a*gr/2.f,

            -a/2.f, a*gr/2.f, 0,
            0, a/2.f, -a*gr/2.f,
            a*gr/2.f, 0, -a/2.f,


            a/2.f, -a*gr/2.f, 0,
            0, -a/2.f, a*gr/2.f,
            -a*gr/2.f, 0, a/2.f,


            -a*gr/2.f, 0, -a/2.f,
            0, -a/2.f, -a*gr/2.f,
            -a/2.f, -a*gr/2.f, 0
    };

    normals = vertices;

    indices = {
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
     for(double i : vertices) {
         if(o++%3==0) {
             std::cout << "]" << std::endl << "[";
         }
         std::cout << i << (o%3 ? ",":"");
     }
     std::cout << std::endl;
 */
}
