#ifndef QUFRAMEBUFFER_H
#define QUFRAMEBUFFER_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

class QuFrameBuffer : public QObject, protected QOpenGLExtraFunctions
{
public:
    QuFrameBuffer(QObject* parent);
    virtual ~QuFrameBuffer();
    void Init(int width,int height);
    void resize(int width,int height);
    void bind();
    void release();
    void render();
private:
    int width;
    int height;

    GLuint fbo;
    GLuint fboTexture;
    GLuint depthTexture;

    QOpenGLShaderProgram *quadProgramm;
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;
    GLuint depthrenderbuffer;
    GLuint rtexID;
    GLuint dtexID;
    void createColorTexture();
    void createDepthTexture();
};

#endif // QUFRAMEBUFFER_H
