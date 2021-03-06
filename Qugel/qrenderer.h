#ifndef QRENDERER_H
#define QRENDERER_H
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include "quframebuffer.h"

class Scene;

class QRenderer : public QObject,protected QOpenGLExtraFunctions
{
public:
    QRenderer(QObject* parent);
    void Initialize();
    void Render();
    float getRefreshRate() const;
    void setRefreshRate(float value);

    int getWidth() const;
    void setWidth(int value);

    int getHeight() const;
    void setHeight(int value);

    float getDevicePixelRatio() const;
    void setDevicePixelRatio(float value);

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
    float refreshRate;
    int width;
    int height;
    float devicePixelRatio;
    Scene* scene;

    ///fbo
    ///
    QuFrameBuffer* fbo=nullptr;
};

#endif // QRENDERER_H
