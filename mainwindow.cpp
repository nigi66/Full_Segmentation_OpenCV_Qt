#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    imageLabel = new QLabel("No image/video loaded", this);
    imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLabel);

    QPushButton *loadImageButton = new QPushButton("Load Image", this);
    QPushButton *loadVideoButton = new QPushButton("Load Video", this);
    QPushButton *startCameraButton = new QPushButton("Start Camera", this);
    layout->addWidget(loadImageButton);
    layout->addWidget(loadVideoButton);
    layout->addWidget(startCameraButton);

    // Segmentation method dropdown
    segmentationComboBox = new QComboBox(this);
    segmentationComboBox->addItem("None");
    segmentationComboBox->addItem("Thresholding");
    segmentationComboBox->addItem("Edge Detection");
    segmentationComboBox->addItem("Color Segmentation");
    segmentationComboBox->addItem("Mask RCNN");
    layout->addWidget(segmentationComboBox);

    QPushButton *applyProcess = new QPushButton("Apply", this);
    layout->addWidget(applyProcess);

    QPushButton *settingsButton = new QPushButton("Settings", this);
    layout->addWidget(settingsButton);

    setCentralWidget(centralWidget);

    // Timer for video/camera updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::processFrame);

    // Signals and slots
    connect(loadImageButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(loadVideoButton, &QPushButton::clicked, this, &MainWindow::loadVideo);
    connect(startCameraButton, &QPushButton::clicked, this, &MainWindow::startCamera);
    connect(applyProcess, &QPushButton::clicked, this, &MainWindow::onApplyProcess);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(){
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()){
        capture.release();
        timer->stop();
        frame = imread(filePath.toStdString());
        processAndDisplay();
    }

}


void MainWindow::loadVideo(){
    QString filePath = QFileDialog::getOpenFileName(this, "Open Video", "", "Videos (*.avi *.mp4 *.mkv)");
    if (!filePath.isEmpty()) {
        capture.open(filePath.toStdString());
        if (capture.isOpened()) {
            timer->start(30);
        }
    }
};


void MainWindow::startCamera(){
    capture.open(0);
    if (capture.isOpened()); timer->start(30);
};


void MainWindow::processFrame(){
    if (capture.isOpened()){
        capture >> frame;
        if (!frame.empty()){
            processAndDisplay();
        }
    }
};



void MainWindow::processAndDisplay(){
    Mat processedFrame = applySegmentation(frame);
    if (processedFrame.empty()){
        cout << "No image is loaded." << endl;
    }
    QImage image = QImage(processedFrame.data, processedFrame.cols, processedFrame.rows, processedFrame.step, QImage::Format_RGB888).rgbSwapped();
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(imageLabel->size(), Qt::KeepAspectRatio));
};

Mat MainWindow::applySegmentation(const Mat &inputFrame){

    Mat output;
   int method = segmentationComboBox->currentIndex();

   switch (method) {
   case 0: // None
       output = inputFrame.clone();
       break;

   case 1: // Thresholding
       cvtColor(inputFrame, output, COLOR_BGR2GRAY);
       threshold(output, output, thresholdValue, 255, THRESH_BINARY);
       cvtColor(output, output, COLOR_GRAY2BGR);
       break;

   case 2: // Edge Detection
       cvtColor(inputFrame, output, COLOR_BGR2GRAY);
       Canny(output, output, edgeThreshold1, edgeThreshold2);
       cvtColor(output, output, COLOR_GRAY2BGR);
       break;

   case 3: // Color Segmentation
       inRange(inputFrame, Scalar(lowerHue, lowerSaturation, lowerValue), Scalar(upperHue, upperSaturation, upperValue), output);
       cvtColor(output, output, COLOR_GRAY2BGR);
       break;

   case 4: {

       try {
           maskRcnnNet = cv::dnn::readNetFromTensorflow(modelPath, configPath);
           maskRcnnNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
           maskRcnnNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);  // Use CUDA if available
           qDebug() << "Mask R-CNN model loaded successfully!";
       } catch (cv::Exception &e) {
           qDebug() << "Error loading Mask R-CNN model: " << e.what();
       }

       cvtColor(inputFrame, output, COLOR_BGR2RGB);
       cv::Mat blob = cv::dnn::blobFromImage(output, 1.0, Size(1024, 1024), Scalar(0, 0, 0), true, false);

       vector<std::string> layerNames = maskRcnnNet.getLayerNames();

       maskRcnnNet.setInput(blob);

       std::vector<std::string> outputNames = {"detection_out_final", "detection_masks"};
       std::vector<cv::Mat> outputs;
       maskRcnnNet.forward(outputs, outputNames);

       cv::Mat detection = outputs[0];
       cv::Mat masks = outputs[1];

       const float* detectionData = (float*)detection.data;

       // Each detection row has 7 values: [batch_id, class_id, confidence, x1, y1, x2, y2]
       int numDetections = detection.size[2];
       int detectionStep = detection.size[3];

       for (int i = 0; i < numDetections; i++) {
           float confidence = detectionData[i * detectionStep + 2];
           if (confidence > confidenceThreshold) {
               int classId = static_cast<int>(detectionData[i * detectionStep + 1]);
               int x1 = static_cast<int>(detectionData[i * detectionStep + 3] * inputFrame.cols);
               int y1 = static_cast<int>(detectionData[i * detectionStep + 4] * inputFrame.rows);
               int x2 = static_cast<int>(detectionData[i * detectionStep + 5] * inputFrame.cols);
               int y2 = static_cast<int>(detectionData[i * detectionStep + 6] * inputFrame.rows);


               // Extract and process the mask for the detected class
               Mat mask = masks.row(classId).reshape(1, 15);  // Mask size 15x15
               cv::resize(mask, mask, Size(x2 - x1, y2 - y1));

               try {
                    Mat roi = output(Rect(x1, y1, x2 - x1, y2 - y1));
                    //roi.setTo(Scalar(0, 100, 0), mask > 0.5);
                    qDebug() << "roi selected properly";
                } catch (cv::Exception &e) {
                    qDebug() << "roi can not be selected" << e.what();
                }

               rectangle(output, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 2);

               // Add a label with confidence
               std::string label = format("class %d  %.2f", classId, confidence);
               cv::putText(output, label, cv::Point(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
           }
       }
       cvtColor(output, output, COLOR_RGB2BGR);
       break;
   }

   default:
       output = inputFrame.clone();
       break;
   }

   return output;
};


void MainWindow::openSettingsWindow(){
    if (!parameterWindow) {
        parameterWindow = new ParameterWindow(this);
        connect(parameterWindow, &ParameterWindow::parameterChanged, this, &MainWindow::updateParameters);

    }
    parameterWindow->show();
};


void MainWindow::updateParameters(int tValue, int eT1, int eT2, int lH, int uH, int lS, int uS, int lV, int uV){
    thresholdValue = tValue;
    edgeThreshold1 = eT1;
    edgeThreshold2 = eT2;
    lowerHue = lH; lowerSaturation = lS; lowerValue = lV;
    upperHue = uH; upperSaturation = uS; upperValue = uV;

    processAndDisplay();

};

void MainWindow::onApplyProcess(){
    processAndDisplay();
}

