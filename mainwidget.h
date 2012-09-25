#ifndef _MAINWIDGET_H_
#define _MAINWIDGET_H_ 

#include "canvas.h"
#include <opencv2/ml/ml.hpp>

class QPushButton;
class QLineEdit;
class QLabel;


class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);

public slots:

    void predict();
    void learn();

private:
    void train();

    Canvas *canvas;
    QPushButton *clearButton, *learnButton, *predictButton;
    QLabel *predictResult;
    int lastResult;

    QLineEdit *trueResult;

    CvANN_MLP ann;
};

#endif
