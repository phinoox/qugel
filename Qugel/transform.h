#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <QtMath>
#include <QVector3D>
#include <QQuaternion>

class QuTransform : public QObject
{
    QuTransform();
    QuTransform(QObject* parent);
    QVector3D position;
    QVector3D scale;
    QQuaternion rotation;

};

#endif // TRANSFORM_H
