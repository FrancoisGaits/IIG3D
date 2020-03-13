#include "myopenglwidget.h"

#include <QMessageBox>
#include <QApplication>
#include <QDateTime>

#include <iostream>
#include <stdexcept>


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), currFs(BLINNPHONG), currPrec(1), currState(DEMO), drawfill(true),
                                                                        _scene(nullptr), _lastime(0){


    _sceneconstructor = ([this](int width, int height) -> Scene * {
        return new Scene(width, height, currFs);
    });
}

void MyOpenGLWidget::switchFragmentShader(FragmentShader fs) {
    currFs = fs;
    resetScene();
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
    _scene = nullptr;
}

void MyOpenGLWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyOpenGLWidget::cleanup);
    
    if (!initializeOpenGLFunctions()) {
        QMessageBox::critical(this, "OpenGL initialization error",
                              "MyOpenGLWidget::initializeGL() : Unable to initialize OpenGL functions");
        exit(1);
    }
    // Initialize OpenGL and all OpenGL dependent stuff below
    resetScene();
}

void MyOpenGLWidget::paintGL() {
    std::int64_t starttime = QDateTime::currentMSecsSinceEpoch();

    if (drawfill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        case Qt::Key_Left:
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Down:
            _scene->keyboardmove(event->key() - Qt::Key_Left, 1. / 100/*double(_lastime)/10.*/);
            update();
            break;
            // Wireframe key
        case Qt::Key_W:
            drawfill = !drawfill;
            update();
            break;
        case Qt::Key_C: {
            if (currPrec > 1) {
                --currPrec;
            } else {
                currPrec = 1;
            }
            resetScene();
        }
            break;
        case Qt::Key_V:{
            if (currPrec < 30) {
                ++currPrec;
            } else {
                currPrec = 30;
            }
            resetScene();
        }
            break;
        case Qt::Key_R:
            resetScene();
            break;
        // Other keys are transmitted to the scene
        default :
            if (_scene->keyboard(event->text().toStdString()[0]))
                update();
            break;
    }
}

void MyOpenGLWidget::switchState(State s) {
    currPrec = 1;
    currState = s;
    resetScene();
}

void MyOpenGLWidget::resetScene() {
    makeCurrent();
    _scene = _sceneconstructor(width(), height());
    switch(currState) {
        case UV :
            _scene->addUVSphere(0.35, currPrec, glm::vec3(0));
            break;
        case GEO :
            _scene->addGeoSphere(0.35, currPrec, glm::vec3(0));
            break;
        case DEMO:
            _scene->addGeoSphere(0.2, 2, glm::vec3(-0.7,0.2,-0.3));
            _scene->addUVSphere(0.4, 4, glm::vec3(0,0,-0.8));
            _scene->addGeoSphere(0.1, 5, glm::vec3(0.2,-0.1,0.2));
            break;
        case CLEAR:
        default:
            break;
    }
    doneCurrent();
    update();
}

