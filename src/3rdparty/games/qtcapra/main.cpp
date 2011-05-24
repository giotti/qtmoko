#ifdef QTOPIA

#include "qtcapra.h"
#include <qtopiaapplication.h>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,QrCapra)
QTOPIA_MAIN

#else

#include <QtGui/QApplication>
#include "qtcapra.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtCapra w;
    w.show();
    return a.exec();
}


#endif
