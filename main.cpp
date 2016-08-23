#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *x=new MainWindow;
    x->show();

    return a.exec();
}
