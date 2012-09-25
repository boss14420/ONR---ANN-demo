#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <QWidget>

class QImage;

class Canvas : public QWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = 0);
    ~Canvas();

    QImage const* getImage() const { return buffer; };

public slots:
    void clear();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    QImage *buffer; 
    QPainter *painter;
    bool mousePress;
    QPoint lastPoint;
};

#endif
