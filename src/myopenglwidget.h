#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QKeyEvent>

#include <memory>

#include "scene.h"
#include "src/Sphere/GeoSphere.h"
#include "src/Sphere/UVSphere.h"
#include "shaders.h"

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

public:
    explicit MyOpenGLWidget(QWidget *parent = 0);

    ~MyOpenGLWidget();

    // size hints for the widget
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    // Demo management
    void activateScene(unsigned int numScene);

    void switchFragmentShader(FragmentShader fs);
    
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
    unsigned currScene;
    FragmentShader currFs;
    unsigned currPrec;
    bool drawfill;

    std::unique_ptr<Scene> _scene;

    using SceneConstructors=std::function<Scene*(int, int)>;
    std::vector<SceneConstructors> _sceneconstructors;
    
    // for event management
    std::int64_t _lastime;
};

#endif // MYOPENGLWIDGET_H
