/* Copyright Andrey N. Kuznetsov (Андрей Николаевич Кузнецов), linmedsoft@narod.ru,
Almaty
01-03 March 2010
28 June 2010

 Copyright 2010 Adrián Cereto Massagué <ssorgatem@gmail.com>

Qt-Pairs - classical memory game "Find the pairs".

This programme is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
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
#include <QApplication>
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
    MyProgramme.setApplicationName("CutePairs");
    MyProgramme.setApplicationVersion("1.0.0");
    MyProgramme.setOrganizationName("ssorgatem");

    //QString sLocale = MainWindow.sLocale;

    QLocale Locale;
    QString sLocale;
    //if(sLocale.isEmpty())
    //{
        sLocale = Locale.system().name();
        sLocale.truncate(2);
        //MainWindow.sLocale = sLocale;
    //}

    QString sQmFile = QString(":/translations/qt-pairs_" + sLocale + ".qm");

    QFileInfo QmFileInfo(sQmFile);
    if(!QmFileInfo.exists())
    {
#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
        QString sErrorMessage = "Can't find a translation file for the language \"" 
                                + /*sLocale*/sQmFile + "\"." + "\nIt will be used English (en) as a default language.";
        QMessageBox::warning(0, "Translation error", sErrorMessage);
#endif
        sLocale = "en";
        sQmFile = QString(":/translations/qt-pairs_" + sLocale + ".qm");
    }

    QTranslator* pQtTranslator = new QTranslator(0);
    pQtTranslator->load(("qt_" + sLocale), ":/translations");
    MyProgramme.installTranslator(pQtTranslator);

    QTranslator* pTranslator = new QTranslator(0);
    pTranslator->load(sQmFile, ".");
    MyProgramme.installTranslator(pTranslator);
    //MainWindow.LanguageChangeSlot();

    TMainForm MainWindow;




//#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
    MainWindow.show();
//#else
    //MainWindow.showMaximized();
//#endif
    return MyProgramme.exec();
}
//------------------------------------------------------------
