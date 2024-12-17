#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <parameterwindow.h>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void loadImage();
    void loadVideo();
    void startCamera();
    void processFrame();
    void processAndDisplay();
    Mat applySegmentation(const Mat &inputFrame);
    void openSettingsWindow();
    void updateParameters(int tValue, int eT1, int eT2, Scalar lower, Scalar upper);
    void maskRCNN();



private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QComboBox *segmentationComboBox;
    QTimer *timer;
    VideoCapture capture;
    Mat frame;
    ParameterWindow *parameterWindow = nullptr;
    Mat processedFrame;

    // Parameters
    int thresholdValue = 128;
    int edgeThreshold1 = 100;
    int edgeThreshold2 = 200;
    int lowerHue = 0, lowerSaturation = 0, lowerValue = 0;
    int upperHue = 180, upperSaturation = 255, upperValue = 255;

    cv::dnn::Net maskRcnnNet;
    float confidenceThreshold = 0.5;  // Confidence threshold for detections
    std::string modelPath = "C:\\Users\\Me\\Documents\\opencv_cpp\\full_segmentation\\full_segmentation\\frozen_inference_graph.pb";
    std::string configPath = "C:\\Users\\Me\\Documents\\opencv_cpp\\full_segmentation\\full_segmentation\\mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
};

#endif // MAINWINDOW_H






