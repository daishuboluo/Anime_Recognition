#include "thread_control.h"

static ID3D11Device* device = nullptr;
static ID3D11DeviceContext* contexts = nullptr;
static IDXGIOutputDuplication* duplication = nullptr;

Thread_Control::Thread_Control(QObject *parent)
    : QThread{parent}
{
    if (!initDXGI())
    {
        qDebug() << "初始化DXGI失败";
        return;
    }
    qDebug() << "初始化DXGI成功";

    execute_inference = new Execute_Inference(this);
}

Thread_Control::~Thread_Control()
{
    if (duplication) duplication->Release();
    if (contexts) contexts->Release();
    if (device) device->Release();
}

void Thread_Control::run()
{
    while (!isInterruptionRequested())
    {
        Capture_DXGI_Image_And_Draw();

        if (image.isNull()) continue;
        execute_inference->Draw(image);

        img = QImage(image);

        QThread::msleep(8);
    }
}

void Thread_Control::captureScreen()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QPixmap pixmap = screen->grabWindow(0);
        image = pixmap.toImage();
    }
}

bool Thread_Control::initDXGI()
{
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                   nullptr, 0, D3D11_SDK_VERSION, &device,
                                   &featureLevel, &contexts);
    if (FAILED(hr)) return false;

    IDXGIDevice* dxgiDevice = nullptr;
    hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if (FAILED(hr)) return false;

    IDXGIAdapter* adapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);
    dxgiDevice->Release();
    if (FAILED(hr)) return false;

    IDXGIOutput* output = nullptr;
    hr = adapter->EnumOutputs(0, &output);
    adapter->Release();
    if (FAILED(hr)) return false;

    IDXGIOutput1* output1 = nullptr;
    hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
    output->Release();
    if (FAILED(hr)) return false;

    hr = output1->DuplicateOutput(device, &duplication);
    output1->Release();
    if (FAILED(hr)) return false;

    return true;
}

void Thread_Control::Capture_DXGI_Image_And_Draw()
{
    HRESULT hr = duplication->AcquireNextFrame(0, &frameInfo, &resource);
    if (FAILED(hr))
    {
        return;
    }

    hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
    resource->Release();
    if (FAILED(hr)) {
        duplication->ReleaseFrame();
        return;
    }

    texture->GetDesc(&desc);

    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;

    hr = device->CreateTexture2D(&desc, nullptr, &stagingTexture);
    if (FAILED(hr)) {
        texture->Release();
        duplication->ReleaseFrame();
        return;
    }

    contexts->CopyResource(stagingTexture, texture);
    texture->Release();
    duplication->ReleaseFrame();

    hr = contexts->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        stagingTexture->Release();
        return;
    }

    image = QImage((const uchar*)mapped.pData, desc.Width, desc.Height, mapped.RowPitch, QImage::Format_ARGB32);

    contexts->Unmap(stagingTexture, 0);
    stagingTexture->Release();
}
