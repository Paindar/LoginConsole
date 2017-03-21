#include "mainwindow.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>
#include "globals.h"
#include<QDebug>
#include <fstream>

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("#MainWindow\
    { border:2px groove gray;\
      border-radius:10px;\
      padding:2px 4px;\
      background-color: rgba(255, 255, 255,196);\
      border-image: url(bg.jpg)\
    }\
");
    w.show();

    return a.exec();
}
