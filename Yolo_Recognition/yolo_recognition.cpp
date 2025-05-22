#include "yolo_recognition.h"

Yolo_Recognition::Yolo_Recognition(QWidget *parent)
    : QWidget(parent)
{
    resize(1280, 720);
    draws = new Recognition_Draw(this);
}

Yolo_Recognition::~Yolo_Recognition()
{

}

void Yolo_Recognition::resizeEvent(QResizeEvent* event)
{
    draws->resize(event->size());
}
