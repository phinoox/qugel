#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <camera.h>
#include "qrenderer.h"

class QuScene : public QObject
{
private:
    QuCamera* defaultCam;
    QuRenderer* renderer;
public:
    QuScene(QObject* parent);
    QuCamera *getDefaultCam() const;
    void setDefaultCam(QuCamera *value);
    //QRenderer *getRenderer() const;
    void Render();
    void setWindowWidth(float width);
    void setWindowHeight(float height);
    void setDevicePixelRatio(float ratio);
    void setRefreshRate(float rate);
    void Initialize();
};

#endif // SCENE_H
