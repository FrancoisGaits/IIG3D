#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QKeyEvent>

#include <memory>

#include "scene.h"
#include "src/Objects/GeoSphere.h"
#include "src/Objects/UVSphere.h"
#include "shaders.h"

enum State {
    CLEAR,
    UV,
    GEO,
    MODEL,
    DEMO
};

enum Lighting {
    BASIC,
    THREE,
    THREEC
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);

    ~MyOpenGLWidget() final;

    // size hints for the widget
    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    void resetScene();

    void switchFragmentShader(FragmentShader fs);
    void switchState(State s);
    void switchLighting(Lighting l);

    std::string sceneInfoString();

public slots:

    void cleanup();

protected:
    // OpenGL management
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    // Event management
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private :
    FragmentShader currFs;
    unsigned currPrec;
    State currState;
    Lighting currLight;
    bool drawfill;

    Scene *_scene;

    using SceneConstructors=std::function<Scene *(int, int)>;
    SceneConstructors _sceneconstructor;

    // for event management
    std::int64_t _lastime;
};

#endif // MYOPENGLWIDGET_H
