#include "glwidget.h"
#include <QScreen>
#include <QGuiApplication>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget (parent)
{
scene = new Scene(this);
}

void GLWidget::initializeGL(){

    scene->setWindowWidth(width());
    scene->setWindowHeight(height());
    scene->setRefreshRate(QGuiApplication::primaryScreen()->refreshRate());
    scene->setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
    scene->Initialize();
}

void GLWidget::paintGL(){
    scene->Render();

    this->update();
}

void GLWidget::resizeGL(int w,int h){
scene->setWindowWidth(w);
scene->setWindowHeight(h);
}
