#ifndef EXECUTE_INFERENCE_H
#define EXECUTE_INFERENCE_H

#include <QObject>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <random>

#include <QPainter>
#include <QDebug>

#include "inference.h"

class Execute_Inference : public QObject
{
    Q_OBJECT

public:
    explicit Execute_Inference(QObject *parent = nullptr);
    ~Execute_Inference();

    void Detector(YOLO_V11 *&p);
    void Draw(QImage &image);
    void Classifier(YOLO_V11 *&p);
    int ReadCocoYaml(YOLO_V11 *p);
    void DetectTest();
    void ClsTest();

private:
    YOLO_V11* yoloDetector;
};

#endif // EXECUTE_INFERENCE_H
