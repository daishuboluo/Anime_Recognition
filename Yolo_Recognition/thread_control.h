#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H

#include <QThread>
#include <QImage>
#include <QScreen>
#include <dxgi.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>  // 需要这个头文件来获取 IDXGIOutput1
#include <QPainter>
#include <dwmapi.h>   // 需要链接 Dwmapi.lib
#include <QTimer>

#include "execute_inference.h" // 引入 Execute_Inference 头文件

class Thread_Control : public QThread
{
    Q_OBJECT

public:
    explicit Thread_Control(QObject *parent = nullptr);
    ~Thread_Control();

    QImage img;    // 捕获的图像
    QImage image;  // 绘制的图像

    void Capture_DXGI_Image_And_Draw(); // 捕获 DXGI 图像并绘制
    bool initDXGI();                     // 初始化 DXGI
    void captureScreen();                // 捕获屏幕

protected:
    void run() override;                 // 线程运行函数

signals:
    void draw_signal();                  // 绘制信号

private:
    Execute_Inference* execute_inference; // 执行推理的指针

    DXGI_OUTDUPL_FRAME_INFO frameInfo;   // 框架信息
    IDXGIResource* resource = nullptr;    // DXGI 资源
    ID3D11Texture2D* texture = nullptr;   // D3D11 纹理
    D3D11_TEXTURE2D_DESC desc;            // 纹理描述
    ID3D11Texture2D* stagingTexture = nullptr; // 临时纹理
    D3D11_MAPPED_SUBRESOURCE mapped;      // 映射的子资源
};

#endif // THREAD_CONTROL_H
