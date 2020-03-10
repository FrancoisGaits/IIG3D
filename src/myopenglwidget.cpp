#include "myopenglwidget.h"

#include <QMessageBox>
#include <QApplication>
#include <QDateTime>

#include <iostream>
#include <stdexcept>


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)/*, QOpenGLFunctions_4_1_Core()*/, currScene(0),
                                                  currFs(ERREUR), drawfill(true), _scene(nullptr), _lastime(0){

    // add all demo constructors here
    _sceneconstructors.emplace_back([this](int width, int height) -> Scene * {
        return new Scene(width, height, drawfill, currPrec, currFs);
    });

    /*
    _sceneconstructors.emplace_back([this](int width, int height) -> Scene * {
        std::cout << "UV Sphere : \n\tprecision : " << precisionFactor << std::endl;
        return new UVSphere(width, height, currentFs, precisionFactor, drawfill);
    });

    _sceneconstructors.emplace_back([this](int width, int height) -> Scene * {
        std::cout << "Geo Sphere : \n\tprecision : " << precisionFactor << std::endl;
        return new GeoSphere(width, height, currentFs, precisionFactor, drawfill);
    });
     */
}

void MyOpenGLWidget::switchFragmentShader(FragmentShader fs) {
    currFs = fs;
    activateScene(currScene);
}

MyOpenGLWidget::~MyOpenGLWidget() {

}

QSize MyOpenGLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize MyOpenGLWidget::sizeHint() const {
    return QSize(768, 576);
}

void MyOpenGLWidget::cleanup() {
    _scene.reset(nullptr);
}

void MyOpenGLWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyOpenGLWidget::cleanup);
    
    if (!initializeOpenGLFunctions()) {
        QMessageBox::critical(this, "OpenGL initialization error",
                              "MyOpenGLWidget::initializeGL() : Unable to initialize OpenGL functions");
        exit(1);
    }
    // Initialize OpenGL and all OpenGL dependent stuff below
    _scene.reset(_sceneconstructors[currScene](width(), height()));
}

void MyOpenGLWidget::paintGL() {
    std::int64_t starttime = QDateTime::currentMSecsSinceEpoch();
    _scene->draw();
    glFinish();
    std::int64_t endtime = QDateTime::currentMSecsSinceEpoch();
    _lastime = endtime - starttime;
}

void MyOpenGLWidget::resizeGL(int width, int height) {
    _scene->resize(width, height);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    // buttons are 0(left), 1(right) to 2(middle)
    int b;
    Qt::MouseButton button = event->button();
    if (button & Qt::LeftButton) {
        if ((event->modifiers() & Qt::ControlModifier))
            b = 2;
        else
            b = 0;
    } else if (button & Qt::RightButton)
        b = 1;
    else if (button & Qt::MiddleButton)
        b = 2;
    else
        b = 3;
    _scene->mouseclick(b, event->x(), event->y());
    _lastime = QDateTime::currentMSecsSinceEpoch();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    _scene->mousemove(event->x(), event->y());
    update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        // Demo keys
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            activateScene(event->key() - Qt::Key_0);
            break;
            // Move keys
        case Qt::Key_Left:
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Down:
            _scene->keyboardmove(event->key() - Qt::Key_Left, 1. / 100/*double(_lastime)/10.*/);
            update();
            break;
            // Wireframe key
        case Qt::Key_W:
            _scene->toggledrawmode();
            drawfill = !drawfill;
            update();
            break;
        case Qt::Key_C: {
            if (currPrec > 1) {
                --currPrec;
            } else {
                currPrec = 1;
            }
            activateScene(currScene);
        }
            break;
        case Qt::Key_V:{
            if (currPrec < 30) {
                ++currPrec;
            } else {
                currPrec = 30;
            }
            activateScene(currScene);
        }
            break;

        // Other keys are transmitted to the scene
        default :
            if (_scene->keyboard(event->text().toStdString()[0]))
                update();
            break;
    }
}

void MyOpenGLWidget::activateScene(unsigned int numScene) {
    if (numScene < _sceneconstructors.size()) {
	    currScene = numScene;
        makeCurrent();
        _scene.reset(_sceneconstructors[numScene](width(), height()));
        doneCurrent();
        update();
    }
}

