#ifndef CAMERA_H
#define CAMERA_H
#include <transform.h>
#include <QObject>
#include <QMatrix4x4>

class QuCamera : public QObject
{
public:
    QuCamera(QObject* parent);
    Transform getTransform() const;
    void setTransform(const Transform &value);
    void Update();

    enum class CamMode{
     Perspective,
     Orthographic
    };

 private:
    Transform transform;
    QMatrix4x4 view;
    float FOV=65;
    float nearClip=0.1f;
    float farClip=1000.0f;
    float aspect;
    CamMode mode=CamMode::Perspective;


};

#endif // CAMERA_H
