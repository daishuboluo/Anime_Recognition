#include "execute_inference.h"

Execute_Inference::Execute_Inference(QObject *parent)
    : QObject{parent}
{
    this->DetectTest();
}

Execute_Inference::~Execute_Inference()
{
    cv::destroyAllWindows();
}

void Execute_Inference::Detector(YOLO_V11*& p)
{
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path imgs_path = current_path / "images";

    for (auto& i : std::filesystem::directory_iterator(imgs_path))
    {
        if (i.path().extension() == ".jpg" || i.path().extension() == ".png" || i.path().extension() == ".jpeg")
        {
            std::string img_path = i.path().string();
            cv::Mat img = cv::imread(img_path);
            std::vector<DL_RESULT> res;
            p->RunSession(img, res);

            for (auto& re : res)
            {
                cv::RNG rng(cv::getTickCount());
                cv::Scalar color(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

                cv::rectangle(img, re.box, color, 3);

                float confidence = floor(100 * re.confidence) / 100;
                std::cout << std::fixed << std::setprecision(2);
                std::string label = p->classes[re.classId] + " " +
                                    std::to_string(confidence).substr(0, std::to_string(confidence).size() - 4);

                cv::rectangle(
                    img,
                    cv::Point(re.box.x, re.box.y - 25),
                    cv::Point(re.box.x + label.length() * 15, re.box.y),
                    color,
                    cv::FILLED
                    );

                cv::putText(
                    img,
                    label,
                    cv::Point(re.box.x, re.box.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.75,
                    cv::Scalar(0, 0, 0),
                    2
                    );
            }

            std::cout << "Press any key to exit" << std::endl;
            cv::imshow("Result of Detection", img);
            cv::destroyAllWindows();
        }
    }
}

void Execute_Inference::Draw(QImage& image)
{
    QPainter image_painter(&image);
    image_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    cv::Mat cv_image = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits());

    std::vector<DL_RESULT> res;
    yoloDetector->RunSession(cv_image, res);

    QFont font;
    font.setPixelSize(image.height() / 40);
    font.setBold(true);
    image_painter.setFont(font);

    for (auto& re : res)
    {
        image_painter.setPen(QPen(yoloDetector->colors[re.classId], 5));
        image_painter.drawRect(re.box.x, re.box.y, re.box.width, re.box.height);
        if (yoloDetector->classes[re.classId] == "YakishioRemon")
        {
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,"青梅竹马");
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));
        }
        else if (yoloDetector->classes[re.classId] == "YanamiAnna")
        {
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,"青梅竹马");
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));
        }
        else if (yoloDetector->classes[re.classId] == "AsagumoChihaya")
        {
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,"偷腥猫");
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));
        }
        else if (yoloDetector->classes[re.classId] == "TsukinokiKoto")
        {
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,"青梅竹马");
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));
        }
        else if (yoloDetector->classes[re.classId] == "KomariChika")
        {
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,"偷腥猫");
            image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));
        }

        qInfo() << " x: " << re.box.x << " y: " << re.box.y
                << " w: " << re.box.width << " h: " << re.box.height;
        qInfo() << "类别  : " << yoloDetector->classes[re.classId];
        qInfo() << "类别 ID: " << re.classId;
        qInfo() << "置信度: " << re.confidence;
        qInfo() << "\n\t";
    }

    image_painter.end();
}

void Execute_Inference::Classifier(YOLO_V11*& p)
{
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path imgs_path = current_path;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    for (auto& i : std::filesystem::directory_iterator(imgs_path))
    {
        if (i.path().extension() == ".jpg" || i.path().extension() == ".png")
        {
            std::string img_path = i.path().string();
            cv::Mat img = cv::imread(img_path);
            std::vector<DL_RESULT> res;
            char* ret = p->RunSession(img, res);

            float positionY = 50;
            for (int i = 0; i < res.size(); i++)
            {
                int r = dis(gen);
                int g = dis(gen);
                int b = dis(gen);

                cv::putText(img, std::to_string(i) + ":", cv::Point(10, positionY),
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(b, g, r), 2);
                cv::putText(img, std::to_string(res.at(i).confidence), cv::Point(70, positionY),
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(b, g, r), 2);
                positionY += 50;
            }

            cv::imshow("TEST_CLS", img);
            cv::waitKey(0);
            cv::destroyAllWindows();
        }
    }
}
int Execute_Inference::ReadCocoYaml(YOLO_V11* p)
{
    std::ifstream file("TrainSet.yaml");
    if (!file.is_open())
    {
        qDebug() << "无法打开文件";
        return 1;
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    std::size_t start = 0;
    std::size_t end = 0;
    for (std::size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("names:") != std::string::npos)
        {
            start = i + 1;
        }
        else if (start > 0 && lines[i].find(':') == std::string::npos)
        {
            end = i;
            break;
        }
    }

    std::vector<std::string> names;
    QVector<QColor> colors;
    for (std::size_t i = start; i < end; i++)
    {
        std::stringstream ss(lines[i]);
        std::string name;
        std::getline(ss, name, ':');
        std::getline(ss, name);
        name.erase(remove(name.begin(), name.end(), ' '), name.end());
        names.push_back(name);

        cv::RNG rng(cv::getTickCount());
        colors.push_back(QColor(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)));
    }

    p->classes = names;
    p->colors = colors;

    return 0;
}

void Execute_Inference::DetectTest()
{
    yoloDetector = new YOLO_V11(this);

    if (ReadCocoYaml(yoloDetector) == 1)
    {
        qDebug() << "读取 COCO YAML 文件失败";
        return;
    }
    else
    {
        qDebug() << "读取 COCO YAML 文件成功";
    }

    DL_INIT_PARAM params;
    params.rectConfidenceThreshold = 0.9;
    params.iouThreshold = 0.5;
    params.modelPath = "Defeated_Dog.onnx";
    params.imgSize = { 640, 640 };

#ifdef USE_CUDA
    params.cudaEnable = true;
    params.modelType = YOLO_DETECT_V11;
#else
    params.modelType = YOLO_DETECT_V11;
    params.cudaEnable = false;
#endif

    yoloDetector->CreateSession(params);
}

void Execute_Inference::ClsTest()
{
    YOLO_V11* yoloDetector = new YOLO_V11();
    std::string model_path = "Defeated_Dog.onnx";
    ReadCocoYaml(yoloDetector);
    DL_INIT_PARAM params{ model_path, YOLO_CLS, {224, 224} };
    yoloDetector->CreateSession(params);
    Classifier(yoloDetector);
}



