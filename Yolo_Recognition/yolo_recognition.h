#ifndef YOLO_RECOGNITION_H
#define YOLO_RECOGNITION_H

#include <QWidget>
#include <QResizeEvent>

#include "recognition_draw.h"

class Yolo_Recognition : public QWidget
{
    Q_OBJECT

public:
    explicit Yolo_Recognition(QWidget *parent = nullptr);
    ~Yolo_Recognition();

private:
    Recognition_Draw *draws;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // YOLO_RECOGNITION_H
