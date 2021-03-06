#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "myopenglwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() final;

private slots:

    void on_action_Version_OpenGL_triggered();
    void on_action_infoScene_triggered();

    void on_action_shaderFacette_triggered();
    void on_action_shaderErreur_triggered();
    void on_action_shaderLambert_triggered();
    void on_action_shaderBlinnPhong_triggered();

    void on_action_basic_triggered();
    void on_action_3pointsC_triggered();
    void on_action_3points_triggered();

    void on_actionClear_triggered();
    void on_actionUV_Sphere_triggered();
    void on_actionGeo_Sphere_triggered();
    void on_actionDemo_triggered();
    void on_actionModel_triggered();

private:
    Ui::MainWindow *ui;
    MyOpenGLWidget *openglWidget;
};

#endif // MAINWINDOW_H
