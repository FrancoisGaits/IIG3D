#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include "opengl_stuff.h"
#include "src/Objects/UVSphere.h"
#include "src/Objects/GeoSphere.h"
#include "src/Objects/Light.h"
#include "shaders.h"
#include "camera/camera.h"
#include "Objects/Model.h"


/** Simple class for managing an OpenGL demo
 */
class Scene {

public:
    explicit Scene(int width, int height, FragmentShader fs, VertexShader vs = VERTEX);

    virtual ~Scene();

    virtual void resize(int width, int height);

    virtual void draw();

    virtual void mouseclick(int button, float xpos, float ypos);
    virtual void mousemove(float xpos, float ypos);
    virtual void keyboardmove(int key, double time);
    virtual bool keyboard(unsigned char k);

    void addGeoSphere(float radius, unsigned precision, glm::vec3 position, glm::vec3 color = glm::vec3(1));
    void addUVSphere(float radius, unsigned precision, glm::vec3 position, glm::vec3 color = glm::vec3(1));
    void addModel(const char *path, glm::vec3 position, glm::vec3 color = glm::vec3(1), float div = 2000);

    void addPointLight(glm::vec3 position, glm::vec3 color);
    void addSpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 focus, float limit);


    std::string sceneInfoString();

private:

    int _width;
    int _height;


    glm::mat4 _view;
    glm::mat4 _projection;

    using CameraSelector=std::function<Camera *()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    int _button{0};
    float _mousex{0};
    float _mousey{0};

    Shader shader;

    std::vector<std::unique_ptr<Sphere>> spheres;
    std::vector<std::unique_ptr<Model>> models;
    std::vector<Light> lights;
};

#endif
