//#include "mainwindow.h"
#include "loading.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<Loading> ld = Loading::getInstance();
    ld->show();
    return a.exec();
}
