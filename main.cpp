// =====================================================================================
//
//       Filename:  main.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  09/08/2012 03:34:49 AM
//       Revision:  none
//       Compiler:  gcc
//
//         Author:  YOUR NAME (), 
//   Organization:  
//
// =====================================================================================

#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWidget mainWidget;
    mainWidget.show();
    return app.exec();
}
