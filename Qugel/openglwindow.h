#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>





class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class QuOpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit QuOpenGLWindow(QWindow *parent = 0);
    ~QuOpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};



#endif // OPENGLWINDOW_H
