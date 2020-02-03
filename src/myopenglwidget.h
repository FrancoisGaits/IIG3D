#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QKeyEvent>

#include <memory>

#include "opengldemo.h"


class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

public:
    explicit MyOpenGLWidget(QWidget *parent = 0);

    ~MyOpenGLWidget();

    // size hints for the widget
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    // Demo management
    void activatedemo(unsigned int numdemo);

public slots:
    void cleanup();

protected:
    // OpenGL management
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    // Event maagement
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private :
    std::unique_ptr<OpenGLDemo> _openglDemo;

    using DemoConstructors=std::function<OpenGLDemo*(int, int)>;
    std::vector<DemoConstructors> _democonstructors;

    // for event management
    std::int64_t _lastime;
};

#endif // MYOPENGLWIDGET_H
