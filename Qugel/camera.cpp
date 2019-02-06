#include "camera.h"

QuTransform Camera::getTransform() const
{
    return transform;
}

void Camera::setTransform(const Transform &value)
{
    transform = value;
}

void Camera::Update()
{
    view=QMatrix4x4();
    view.perspective(FOV,aspect,nearClip,farClip);
}

Camera::Camera(QObject* parent):QObject (parent){


}
