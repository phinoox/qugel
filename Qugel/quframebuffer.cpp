#include "quframebuffer.h"

static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};

QuFrameBuffer::QuFrameBuffer(QObject *parent) : QObject(parent)
{

}

QuFrameBuffer::~QuFrameBuffer()
{
    if(this->fboTexture!=0)
        glDeleteTextures(1,&fboTexture);
    if(this->depthTexture!=0)
        glDeleteTextures(1,&depthTexture);
    glDeleteBuffers(1,&quad_vertexbuffer);

}

void QuFrameBuffer::Init(int width, int height)
{
    this->width=width;
    this->height=height;
    static_cast<QOpenGLExtraFunctions*>(this)->initializeOpenGLFunctions();
    glGenFramebuffers(1, &fbo);
   this->createColorTexture();
    this->createDepthTexture();
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    quadProgramm = new QOpenGLShaderProgram(this);
    quadProgramm->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/Shaders/fboquad.vert");
    quadProgramm->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/Shaders/fboquad.frag");
    quadProgramm->link();

    rtexID = quadProgramm->uniformLocation("renderedTexture");
    dtexID = quadProgramm->uniformLocation("depthTexture");
}

void QuFrameBuffer::resize(int width, int height)
{
    this->width=width;
    this->height=height;
    if(this->fboTexture!=0)
        glDeleteTextures(1,&fboTexture);
    if(this->depthTexture!=0)
        glDeleteTextures(1,&depthTexture);
    this->createColorTexture();
    this->createDepthTexture();

}

void QuFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //glDrawBuffer(GL_BACK);

    GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glViewport(0, 0, width , height );
    glClearColor(1,0,0,1);


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

}

void QuFrameBuffer::release()
{
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     glBindTexture(GL_TEXTURE_2D,0);
}

void QuFrameBuffer::render()
{
    glViewport(0, 0, width , height);
    glClearColor(0,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    quadProgramm->bind();

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    // Set our "renderedTexture" sampler to use Texture Unit 0
    glUniform1i(rtexID, 0);
    glUniform1i(dtexID, 1);



    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    glBindTexture(GL_TEXTURE_2D,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glDisableVertexAttribArray(0);
    quadProgramm->release();
}

void QuFrameBuffer::createColorTexture()
{
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenTextures(1, &fboTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, fboTexture);

    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D,0);


    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void QuFrameBuffer::createDepthTexture()
{
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

}
