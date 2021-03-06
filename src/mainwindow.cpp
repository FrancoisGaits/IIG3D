#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>

#include <iostream>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(3);
    QSurfaceFormat::setDefaultFormat(format);

    ui->setupUi(this);

    openglWidget = new MyOpenGLWidget(this);
    openglWidget->resize(openglWidget->sizeHint());
    openglWidget->setFocus();

    setCentralWidget(openglWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_action_Version_OpenGL_triggered() {
    std::stringstream message;
    message << "Qt version     : " << qVersion() << std::endl;
    message << "Renderer       : " << glGetString(GL_RENDERER) << std::endl;
    message << "Vendor         : " << glGetString(GL_VENDOR) << std::endl;
    message << "Version        : " << glGetString(GL_VERSION) << std::endl;
    message << "GLSL Version   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    QMessageBox::information(this, "OpenGL Information", message.str().c_str());
}

void MainWindow::on_action_infoScene_triggered() {
    QMessageBox::information(this, "Information Scene", openglWidget->sceneInfoString().c_str());
}

void MainWindow::on_action_shaderLambert_triggered() {
    openglWidget->switchFragmentShader(LAMBERT);
}

void MainWindow::on_action_shaderFacette_triggered() {
    openglWidget->switchFragmentShader(FACETTE);
}

void MainWindow::on_action_shaderErreur_triggered() {
    openglWidget->switchFragmentShader(ERREUR);
}

void MainWindow::on_action_shaderBlinnPhong_triggered() {
    openglWidget->switchFragmentShader(BLINNPHONG);
}

void MainWindow::on_actionClear_triggered() {
    openglWidget->switchState(CLEAR);
}

void MainWindow::on_action_3points_triggered() {
    openglWidget->switchLighting(THREE);
}

void MainWindow::on_action_3pointsC_triggered() {
    openglWidget->switchLighting(THREEC);
}

void MainWindow::on_action_basic_triggered() {
    openglWidget->switchLighting(BASIC);
}

void MainWindow::on_actionUV_Sphere_triggered() {
    openglWidget->switchState(UV);
}

void MainWindow::on_actionGeo_Sphere_triggered() {
    openglWidget->switchState(GEO);
}

void MainWindow::on_actionDemo_triggered() {
    openglWidget->switchState(DEMO);
}

void MainWindow::on_actionModel_triggered() {
    openglWidget->switchState(MODEL);
}

