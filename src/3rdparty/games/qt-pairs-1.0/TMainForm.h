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
#ifndef _TMainForm_h_
#define _TMainForm_h_
//------------------------------------------------------------
#include <QMainWindow>
#include <QList>
//------------------------------------------------------------
//------------------------------------------------------------
class QAction;
class QActionGroup;
class QDomElement;
class QLabel;
class QMenu;
class QTimer;
class TGameBoard;
//------------------------------------------------------------
class TMainForm : public QMainWindow
{
    Q_OBJECT

public:
    TMainForm(QWidget* pParent = 0, Qt::WindowFlags Flag = Qt::Window);

    QString sLocale;

private:
    TGameBoard* pCentralWidget;

    QTimer* pTimer;
    QLabel* pDateTimeLabel;
    QLabel* pMovesLabel;

    QAction* pGameExitAction;
    QAction* pGameNewAction;
    QAction* pAboutAction;
    QAction* pAboutQtAction;

    QMenu* pGameMenu;
    QMenu* pAboutMenu;

    void Initialise();
    void CreateActions();
    void CreateMenus();
    void CreateStatusBar();
    void LoadSettings();
    void ParseBars(QDomElement&);

public slots:

private slots:
    void TimeSlot();
    void GameNewSlot();
    void AboutSlot();
    void AboutQtSlot();
    void NewMoveSlot(int);
    void GameFinishedSlot();
};
//------------------------------------------------------------
#endif
