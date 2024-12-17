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
    void parameterChanged(int thresholdValue, int edgeTHreshold, int edgeThreshold2, Scalar lower, Scalar upper);

private slots:
    void emitParameters(){
        emit parameterChanged(thresholdSlider->value(), edgeThreshold1Slider->value(), edgeThreshold2Slider->value(), Scalar(), Scalar());
    }

private:
    QSlider *thresholdSlider;
    QSlider *edgeThreshold1Slider;
    QSlider *edgeThreshold2Slider;

};

#endif // PARAMETERWINDOW_H
