#ifndef GEOSPHERE_H
#define GEOSPHERE_H

#include "opengldemo.h"

#include "camera/camera.h"
#include "shaders.h"
#include "Mesh.h"

#include <memory>
#include <functional>


/** Simple drawing demonstration
 */
class GeoSphere : public OpenGLDemo {
public:
    explicit GeoSphere(int width, int height);

    void resize(int width, int height) override;

    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;

    void mousemove(float xpos, float ypos) override;

    void keyboardmove(int key, double time) override;

    bool keyboard(unsigned char k) override;

    ~GeoSphere() override = default;

private:

    void generateGeoSphereAttributes(unsigned nbDiv, float radius);

    // A simple geometry
    Mesh mesh;


    //Shaders
    Shader shader;

    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    // Camera
    using CameraSelector=std::function<Camera *()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};

/*------------------------------------------------------------------------------------------------------------------------*/


#endif
