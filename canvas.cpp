/*
 * =====================================================================================
 *
 *       Filename:  canvas.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/08/2012 01:32:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "canvas.h"
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <cstdlib>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMaximumSize(100, 140);
    setMinimumSize(100, 140);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resize(maximumSize());
    setBackgroundRole(QPalette::Base);

    buffer = new QImage(maximumSize(), QImage::Format_ARGB32);
//    buffer = new QImage(maximumSize(), QImage::Format_Mono);
    buffer->fill(Qt::white);
    
    painter = new QPainter();
    painter->begin(buffer);

    QPen pen(Qt::black);
    pen.setWidth(10);
    painter->setPen(pen);

    mousePress = false;
}

Canvas::~Canvas() {
    painter->end();
    delete painter;
    delete buffer;
}

void Canvas::clear() {
    buffer->fill(Qt::white);
    repaint();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    mousePress = true;
    lastPoint = event->pos();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(!mousePress) return;

    painter->drawLine(lastPoint, event->pos());
//    repaint(std::min(lastPoint.x(), event->pos().x()),
//            std::min(lastPoint.y(), event->pos().y()),
//            std::abs(lastPoint.x()- event->pos().x()),
//            std::abs(lastPoint.y()- event->pos().y()));
    repaint();

    lastPoint = event->pos();
}

void Canvas::mouseReleaseEvent(QMouseEvent *) {
    mousePress = false;
}

void Canvas::paintEvent(QPaintEvent *) {
    QPainter p(this);
//    p.setClipRect(event->rect());
    p.drawImage(0, 0, *buffer);
}
