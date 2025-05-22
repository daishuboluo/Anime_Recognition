#include "yolo_recognition.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Yolo_Recognition w;
    w.show();
    return a.exec();
}
