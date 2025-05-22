#include "recognition_draw.h"

Recognition_Draw::Recognition_Draw(QWidget* parent)
    : QOpenGLWidget(parent)
{
    thread_control = new Thread_Control(this);
    thread_control->start();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] {
        update();
    });
    timer->start(16);
}

Recognition_Draw::~Recognition_Draw()
{
    thread_control->requestInterruption();
    thread_control->wait();
}

void Recognition_Draw::initializeGL()
{
}

void Recognition_Draw::resizeGL(int w, int h)
{
    W = w;
    H = h;
    Center_Image(thread_control->img);
}

void Recognition_Draw::paintGL()
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(centerRect, thread_control->img);
}

void Recognition_Draw::Center_Image(QImage& image)
{
    int imgWidth = image.width();
    int imgHeight = image.height();
    float imgRatio = float(imgWidth) / imgHeight;

    int scaledWidth, scaledHeight;
    float windowRatio = float(W) / H;

    if (windowRatio > imgRatio)
    {
        scaledHeight = H;
        scaledWidth = int(H * imgRatio);
    }
    else
    {
        scaledWidth = W;
        scaledHeight = int(W / imgRatio);
    }

    int x = (W - scaledWidth) / 2;
    int y = (H - scaledHeight) / 2;

    centerRect = QRect(x, y, scaledWidth, scaledHeight);
}
