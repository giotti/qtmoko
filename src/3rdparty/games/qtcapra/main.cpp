#ifdef QTOPIA

#include <qtopiaapplication.h>
//#include <QtopiaApplication>
#include "qtcapra.h"
#include "rotate.h"
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, QtCapra)
QTOPIA_MAIN
//#include <QtopiaServiceRequest>
//#include <QValueSpaceItem>

#else

#include <QtGui/QApplication>
#include "qtcapra.h"
#include "rotate.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtCapra w;
    w.show();
    return a.exec();
}


#endif
