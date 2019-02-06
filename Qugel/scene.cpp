#include "scene.h"

Camera *Scene::getDefaultCam() const
{
    return defaultCam;
}

void Scene::setDefaultCam(Camera *value)
{
    defaultCam = value;
}

/*
QRenderer *Scene::getRenderer() const
{
    return renderer;
}*/

void Scene::Render()
{
    renderer->Render();
}

void Scene::setWindowWidth(float width)
{
    renderer->setWidth(width);
}

void Scene::setWindowHeight(float height)
{
    renderer->setHeight(height);
}

void Scene::setDevicePixelRatio(float ratio)
{
    renderer->setDevicePixelRatio(ratio);
}

void Scene::setRefreshRate(float rate)
{
renderer->setRefreshRate(rate);
}

void Scene::Initialize()
{
    renderer->Initialize();

}


Scene::Scene(QObject* parent): QObject(parent)
{
    defaultCam=new Camera(this);
    renderer= new QRenderer(this);
}
