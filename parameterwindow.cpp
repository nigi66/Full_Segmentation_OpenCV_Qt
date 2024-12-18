
#include <parameterwindow.h>
#include <ui_parameterwindow.h>

ParameterWindow::ParameterWindow(QWidget *parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    thresholdSlider = new QSlider(Qt::Horizontal, this);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(128);
    layout->addWidget(new QLabel("Threshold Value:"));
    layout->addWidget(thresholdSlider);

    edgeThreshold1Slider = new QSlider(Qt::Horizontal, this);
    edgeThreshold1Slider->setRange(0, 500);
    edgeThreshold1Slider->setValue(100);
    layout->addWidget(new QLabel("Edge Threshold 1:"));
    layout->addWidget(edgeThreshold1Slider);

    edgeThreshold2Slider = new QSlider(Qt::Horizontal, this);
    edgeThreshold2Slider->setRange(0, 500);
    edgeThreshold2Slider->setValue(200);
    layout->addWidget(new QLabel("Edge Threshold 2:"));
    layout->addWidget(edgeThreshold2Slider);



    lowerHueSlider = new QSlider(Qt::Horizontal, this);
    lowerHueSlider->setRange(0, 500);
    lowerHueSlider->setValue(0);
    layout->addWidget(new QLabel("lower Hue:"));
    layout->addWidget(lowerHueSlider);

    upperHueSlider = new QSlider(Qt::Horizontal, this);
    upperHueSlider->setRange(0, 500);
    upperHueSlider->setValue(180);
    layout->addWidget(new QLabel("upper Hue:"));
    layout->addWidget(upperHueSlider);

    lowerSaturationSlider = new QSlider(Qt::Horizontal, this);
    lowerSaturationSlider->setRange(0, 500);
    lowerSaturationSlider->setValue(0);
    layout->addWidget(new QLabel("lower Saturation:"));
    layout->addWidget(lowerSaturationSlider);

    upperSaturationSlider = new QSlider(Qt::Horizontal, this);
    upperSaturationSlider->setRange(0, 500);
    upperSaturationSlider->setValue(255);
    layout->addWidget(new QLabel("upper Saturation:"));
    layout->addWidget(upperSaturationSlider);

    lowerValueSlider = new QSlider(Qt::Horizontal, this);
    lowerValueSlider->setRange(0, 500);
    lowerValueSlider->setValue(0);
    layout->addWidget(new QLabel("lower Value:"));
    layout->addWidget(lowerValueSlider);

    upperValueSlider = new QSlider(Qt::Horizontal, this);
    upperValueSlider->setRange(0, 500);
    upperValueSlider->setValue(255);
    layout->addWidget(new QLabel("upper Value:"));
    layout->addWidget(upperValueSlider);

    setCentralWidget(centralWidget);

    connect(thresholdSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(edgeThreshold1Slider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(edgeThreshold2Slider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);

    connect(lowerHueSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(upperHueSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(lowerSaturationSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(upperSaturationSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(lowerValueSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(upperValueSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
};
