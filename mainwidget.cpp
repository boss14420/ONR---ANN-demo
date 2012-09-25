// =====================================================================================
//
//       Filename:  mainwidget.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  09/08/2012 02:25:14 AM
//       Revision:  none
//       Compiler:  gcc
//
//         Author:  YOUR NAME (), 
//   Organization:  
//
// =====================================================================================

#include "mainwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QDebug>

#include <iostream>

#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    // Interface
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resize(150, 300);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout *hLayout = new QHBoxLayout;
    canvas = new Canvas(this);
    canvas->resize(100, 140);
    hLayout->addWidget(canvas); 
    hLayout->addStretch();
    mainLayout->addLayout(hLayout);

    QHBoxLayout *hLayout0 = new QHBoxLayout;
    clearButton = new QPushButton("clear", this);
    hLayout0->addWidget(clearButton);
    hLayout0->addStretch();
    mainLayout->addLayout(hLayout0);

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    predictButton = new QPushButton("predict", this);
    hLayout1->addWidget(predictButton);
    predictResult = new QLabel(this);
    hLayout1->addWidget(predictResult);
    hLayout1->addStretch();
    mainLayout->addLayout(hLayout1);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    learnButton = new QPushButton("learn", this);
    hLayout2->addWidget(learnButton); 
    trueResult = new QLineEdit(this);
    trueResult->setMaxLength(1);
//    trueResult->setInputMask("9");
    hLayout2->addWidget(trueResult);
    hLayout2->addStretch();
    mainLayout->addLayout(hLayout2);
    
    mainLayout->addStretch();

    // Artifical Neural Network
    cv::Mat_<int> layerSize(1, 3);
    layerSize << 140, 75, 10;
    ann.create(layerSize, CvANN_MLP::SIGMOID_SYM, 1.0f, 1.0f);
    lastResult = -1;

    train();

    // Signal & slot
    connect(clearButton, SIGNAL(pressed()), canvas, SLOT(clear()));
//    connect(canvas, SIGNAL(modified()), SLOT(canvasModified()));
    connect(predictButton, SIGNAL(pressed()), this, SLOT(predict()));
    connect(learnButton, SIGNAL(pressed()), this, SLOT(learn()));
}

void MainWidget::predict() {
    using cv::Mat_;

    QImage bitmap = canvas->getImage()->scaled(10, 14, 
            Qt::KeepAspectRatio, Qt::SmoothTransformation);

//    canvas->getImage()->save("tmp.png");
    bitmap.save("tmp.png");

    Mat_<float> input(1, 140), output(1, 10);

    std::cerr << "Input:\n";
    for(int y = 0; y != 14; ++y) {
        for(int x = 0; x != 10; ++x) {
            input(0, y*10 + x) = (bitmap.pixel(x,y) != 0xffffffff) ? 0.0f : 1.0f;
//            qDebug() << input(0, x*14+y);
            std::cerr << input(0, y*10+x) << ' ';
//            std::cerr << std::hex << bitmap.pixel(x,y) << ' ';
        }
        std::cerr << '\n';
    }
    std::cerr << '\n';

    ann.predict(input, output);

    std::cerr << "Output:\n";
    for(int i = 0; i != 10; ++i)
        std::cerr << output(0, i) << ' ';
    std::cerr << '\n';

    lastResult = -1;
    float maxFit = 0.0;
    for(int i = 0; i != 10; ++i)
        if(output(0, i) > maxFit) {
            maxFit = output(0, i);
            lastResult = i;
        }

    predictResult->setText(QString::number(lastResult));
}

void MainWidget::learn() {
//    using cv::Mat_;
//
//    int digit = trueResult->text().toInt();
//
//    if(digit == lastResult) return;
//
//    Mat_<float> input(1, 150), output(1, 10);
//    for(int i = 0; i != digit; ++i) output(0, i) = 0;
//    for(int i = digit + 1; i != 10; ++i) output(0, i) = 0;
//    output(0, digit) = 1;
//
//    QImage bitmap = canvas->getImage()->scaled(10, 15, Qt::KeepAspectRatio);
//    for(int x = 0; x != 10; ++x)
//        for(int y = 0; y != 14; ++y)
//            input(0, x*14+y) = (bitmap.pixel(x,y) << 8) ? 0 : 1;
//
//    ann.train(input, output, cv::Mat(), cv::Mat(),
//            CvANN_MLP_TrainParams(
//                cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 10, 0.01),
//                CvANN_MLP_TrainParams::BACKPROP, 0.01, 0.1),
//            CvANN_MLP::UPDATE_WEIGHTS);
}

void MainWidget::train() {
    cv::Mat_<float> inputs(250, 140);
    cv::Mat_<float> outputs(250, 10);

    cv::Mat_<uchar> image = cv::imread("onr_testData.png", 0);
    unsigned test_count = -1;
    for(unsigned y0 = 0; y0 < 193; y0 += 20)
        for(unsigned x0 = 0; x0 < 369; x0+= 15) {
            unsigned digit = y0 / 20;
            
            ++test_count;

//            std::cerr << "Input " << digit << '\n';
            for(unsigned y = 0; y != 14; ++y) {
                for(unsigned x = 0; x != 10; ++x) {
                    inputs(test_count, y * 10 + x) = 
                        (image(y0+y, x0+x) < 128) ? 0.0f : 1.0f;
//                    std::cerr << inputs(test_count, y*10+x) << ' ';
//                    std::cerr << (uint) image(y0+y, x0+x) << ' ';
                }
//                std::cerr << '\n';
            }
//            std::cerr << '\n';

            for(unsigned i = 0; i != digit; ++i) 
                outputs(test_count, i) = 0.0f;
            outputs(test_count, digit) = 1.0f;
            for(unsigned i = digit; i != 10; ++i)
                outputs(test_count, i) = 0.0f;
        }

    CvANN_MLP_TrainParams trainParam(
            cvTermCriteria(CV_TERMCRIT_EPS, 10000, 0.001f),
            CvANN_MLP_TrainParams::BACKPROP,
            0.01f, 0.3f
            );

    ann.train(inputs, outputs, cv::Mat(), cv::Mat(), trainParam);
}
