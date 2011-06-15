#include <QtGui/QApplication>
#include "qtom.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOm w;
    w.show();

    return a.exec();
}
