
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

    setCentralWidget(centralWidget);

    connect(thresholdSlider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(edgeThreshold1Slider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
    connect(edgeThreshold2Slider, &QSlider::valueChanged, this, &ParameterWindow::emitParameters);
};
