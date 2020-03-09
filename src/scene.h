#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include "opengl_stuff.h"
#include "src/Sphere/UVSphere.h"
#include "shaders.h"
#include "camera/camera.h"


/** Simple class for managing an OpenGL demo
 */
class Scene {

public:
    explicit Scene(int width, int height, bool drawfill=true);
    virtual ~Scene();

    virtual void resize(int width, int height);
    virtual void draw();

    virtual void mouseclick(int button, float xpos, float ypos);
    virtual void mousemove(float xpos, float ypos);
    virtual void keyboardmove(int key, double time);
    virtual bool keyboard(unsigned char k);


    void toggledrawmode();

private:

    // Width and heigth of the viewport
    int _width;
    int _height;

    // Rendering mode (true is filled, false is wireframed
    bool _drawfill;

    glm::mat4 _view;
    glm::mat4 _projection;

    // Camera
    using CameraSelector=std::function<Camera *()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    Shader shader;
    std::unique_ptr<Sphere> sphere;

};


#endif // SCENE_H
