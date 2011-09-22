/* Copyright Andrey N. Kuznetsov (Андрей Николаевич Кузнецов), linmedsoft@narod.ru,
Almaty
01-03 March 2010
28 June 2010

Qt-Pairs - classical memory game "Find the pairs".

This programme is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
This programme is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this programme.  */
//------------------------------------------------------------
#include "TMainForm.h"
//------------------------------------------------------------
//#include <QApplication>
#include <QtGui/QApplication>
#include <QLocale>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
//------------------------------------------------------------
#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif
//------------------------------------------------------------
int main(int iArgc, char* psArgv[])
{
    QApplication MyProgramme(iArgc, psArgv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    TMainForm MainWindow;
    QString sLocale = MainWindow.sLocale;

    QLocale Locale;
    if(sLocale.isEmpty()) 
    {
        sLocale = Locale.system().name();
        sLocale.truncate(2);
        MainWindow.sLocale = sLocale;
    }

    QString sQmFile = QString("qt-pairs_" + sLocale + ".qm");
    QString sQmPath = MyProgramme.applicationDirPath() + "/translations";
    sQmFile = sQmPath + "/" + sQmFile;

    QFileInfo QmFileInfo(sQmFile);
    sLocale = "en";

    QTranslator* pTranslator = new QTranslator(0);
    pTranslator->load(sQmFile, ".");
    MyProgramme.installTranslator(pTranslator);
    MainWindow.LanguageChangeSlot();

    MainWindow.show();
    return MyProgramme.exec();
}
//------------------------------------------------------------
