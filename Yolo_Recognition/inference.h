// Ultralytics 🚀 AGPL-3.0 License - https://ultralytics.com/license
#ifndef YOLO_V11_H // 替换为适当的头文件保护宏
#define YOLO_V11_H // 确保头文件只被包含一次

#define RET_OK nullptr // 定义成功返回值为 nullptr

#ifdef _WIN32
#include <Windows.h> // Windows 特定头文件
#include <direct.h>  // Windows 目录操作头文件
#include <io.h>      // Windows I/O 头文件
#endif

#include <string>     // 引入字符串库
#include <vector>     // 引入向量库
#include <opencv2/opencv.hpp> // 引入 OpenCV 库
#include "onnxruntime_cxx_api.h" // 引入 ONNX Runtime C++ API

#ifdef USE_CUDA
#include <cuda_fp16.h> // 引入 CUDA 半精度浮点数库
#endif

#include <qDebug>      // 引入 Qt 调试输出库
#include <QColor>      // 引入 Qt 颜色库
#include <QVector>     // 引入 Qt 向量库
#include <QObject>

enum MODEL_TYPE
{
    // FLOAT32 MODEL
    YOLO_DETECT_V11 = 1, // YOLO V11 检测模型
    YOLO_POSE = 2,       // YOLO 姿态估计模型
    YOLO_CLS = 3,        // YOLO 分类模型

    // FLOAT16 MODEL
    YOLO_DETECT_V11_HALF = 4, // YOLO V11 半精度检测模型
    YOLO_POSE_V11_HALF = 5,   // YOLO 姿态估计半精度模型
    YOLO_CLS_HALF = 6         // YOLO 分类半精度模型
};

// DL 初始化参数结构体
typedef struct _DL_INIT_PARAM
{
    std::string modelPath; // 模型路径
    MODEL_TYPE modelType = YOLO_DETECT_V11; // 模型类型，默认为 YOLO_DETECT_V11
    std::vector<int> imgSize = { 640, 640 }; // 图像尺寸，默认为 640x640
    float rectConfidenceThreshold = 0.6; // 置信度阈值，默认为 0.6
    float iouThreshold = 0.5; // IOU 阈值，默认为 0.5
    int keyPointsNum = 2; // 关键点数量，默认为 2（用于姿态估计）
    bool cudaEnable = false; // 是否启用 CUDA，默认为 false
    int logSeverityLevel = 3; // 日志严重性级别，默认为 3
    int intraOpNumThreads = 1; // 内部操作线程数，默认为 1
} DL_INIT_PARAM; // DL 初始化参数结构体定义结束

// DL 推理结果结构体
typedef struct _DL_RESULT
{
    int classId;        // 类别 ID
    float confidence;   // 置信度
    cv::Rect box;       // 边界框
    std::vector<cv::Point2f> keyPoints; // 关键点
} DL_RESULT; // DL 结果结构体定义结束

// YOLO_V11 类
class YOLO_V11 : public QObject
{
public:
    explicit YOLO_V11(QObject* parent = nullptr); // 构造函数
    ~YOLO_V11(); // 析构函数

public:
    const char* CreateSession(DL_INIT_PARAM& iParams); // 创建会话
    char* RunSession(cv::Mat& iImg, std::vector<DL_RESULT>& oResult); // 运行会话
    char* WarmUpSession(); // 预热会话

    /**
     * @brief 张量处理模板函数
     *
     * @tparam N 模板参数
     * @param starttime_1 开始时间
     * @param iImg 输入图像
     * @param blob blob 数据
     * @param inputNodeDims 输入节点维度
     * @param oResult 输出结果
     * @return char* 返回状态
     */
    template<typename N>
    char* TensorProcess(clock_t& starttime_1, cv::Mat& iImg, N& blob, std::vector<int64_t>& inputNodeDims,
                        std::vector<DL_RESULT>& oResult);

    char* PreProcess(cv::Mat& iImg, std::vector<int> iImgSize, cv::Mat& oImg); // 图像预处理

    std::vector<std::string> classes{}; // 类别名称列表
    QVector<QColor> colors; // QColor颜色列表，用于绘制边界框

private:
    Ort::Env env; // ONNX 环境
    Ort::Session* session; // 会话指针
    bool cudaEnable; // CUDA 启用状态
    Ort::RunOptions options; // 运行选项
    std::vector<const char*> inputNodeNames; // 输入节点名称列表
    std::vector<const char*> outputNodeNames; // 输出节点名称列表

    MODEL_TYPE modelType; // 模型类型
    std::vector<int> imgSize; // 图像尺寸
    float rectConfidenceThreshold; // 置信度阈值
    float iouThreshold; // IOU 阈值
    float resizeScales; // 缩放比例（用于 LetterBox）
};

#endif // YOLO_V11_H
