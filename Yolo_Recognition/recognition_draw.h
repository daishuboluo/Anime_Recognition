#ifndef RECOGNITION_DRAW_H
#define RECOGNITION_DRAW_H

#include <QTimer>
#include <QImage>
#include <QResizeEvent>
#include <QOpenGLWidget> // 包含OpenGL小部件类
#include "thread_control.h"
#include <QTime>

class Recognition_Draw : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit Recognition_Draw(QWidget *parent = nullptr);
    ~Recognition_Draw();

    QTimer* timer; // 定时器

    // 窗口宽度和高度
    int W;
    int H;

    // 图片计算并得出居中位置
    QRect centerRect;
    void Center_Image(QImage& image);

protected:
    virtual void initializeGL() override; // 初始化OpenGL环境
    virtual void resizeGL(int w, int h) override; // 窗口大小调整
    virtual void paintGL() override; // 绘制场景

private:
    Thread_Control* thread_control; // 线程控制类
};

#endif // RECOGNITION_DRAW_H
