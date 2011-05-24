#include <QtGui/QApplication>
#include "qtom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOm w;
    w.show();

    return a.exec();
}
