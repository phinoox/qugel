#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QOpenGLWidget>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    QGridLayout* GuiLayout;
    QOpenGLWidget* sceneWidget;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_actionExit_triggered();

};

#endif // MAINWINDOW_H
