#ifndef SIMPLESPHERE_H
#define SIMPLESPHERE_H

#include "opengldemo.h"

#include "hello_camera/camera.h"
#include "shaders.hpp"

#include <memory>
#include <functional>


/** Simple drawing demonstration
 */
class SimpleSphere : public OpenGLDemo {
public:
    explicit SimpleSphere(int width, int height);
    ~SimpleSphere() override;

    void resize(int width, int height) override;
    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;

private:
    void generateUVSphereAttributes(unsigned nbParrallels, unsigned nbMeridians, float radius, std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, std::vector<GLuint> &indices);

    void generateGeoSphereAttributes(unsigned nbDiv, float radius, std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, std::vector<GLuint> &indices);

    // A simple geometry
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    // OpenGL object for geometry
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;

    //Shaders
    Shader shader;
    
    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};

/*------------------------------------------------------------------------------------------------------------------------*/


#endif // SIMPLESPHERE_H
