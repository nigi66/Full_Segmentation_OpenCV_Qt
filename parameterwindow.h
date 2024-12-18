#ifndef PARAMETERWINDOW_H
#define PARAMETERWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QComboBox>
#include <QTimer>
#include <QImage>
#include <QDebug>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ParameterWindow : public QMainWindow{
    Q_OBJECT

public:
    ParameterWindow();
    ParameterWindow(QWidget *parent = nullptr);

signals:
    void parameterChanged(int thresholdValue, int edgeTHreshold, int edgeThreshold2, int lowerHue, int upperHue,
                          int lowerSaturation, int upperSaturation, int lowerValue, int upperValue);

private slots:
    void emitParameters(){
        emit parameterChanged(thresholdSlider->value(), edgeThreshold1Slider->value(), edgeThreshold2Slider->value(),
                              lowerHueSlider->value(), upperHueSlider->value(),
                              lowerSaturationSlider->value(), upperSaturationSlider->value(),
                              lowerValueSlider->value(), upperValueSlider->value());
    }

private:
    QSlider *thresholdSlider;

    QSlider *edgeThreshold1Slider;
    QSlider *edgeThreshold2Slider;

    QSlider *lowerHueSlider;
    QSlider *upperHueSlider;
    QSlider *lowerSaturationSlider;
    QSlider *upperSaturationSlider;
    QSlider *lowerValueSlider;
    QSlider *upperValueSlider;

};

#endif // PARAMETERWINDOW_H
