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
#include "TChampion.h"
//------------------------------------------------------------
class QAction;
class QActionGroup;
class QDomElement;
class QLabel;
class QMenu;
class QTimer;
class QToolBar;
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

    QList<TChampion> Scores; // Рекорды.

    QTimer* pTimer;
    QLabel* pDateTimeLabel;
    QLabel* pMovesLabel;

    QAction* pGameExitAction;
    QAction* pGameNewAction;
    QAction* pToolsStatisticsAction;
    QAction* pToolsSaveSettingsAction;
    QActionGroup* pLanguageActions;
    QAction* pAboutAction;
    QAction* pAboutQtAction;

    QMenu* pGameMenu;
    QMenu* pToolsMenu;
    QMenu* pLanguageMenu;
    QMenu* pAboutMenu;

    QToolBar* pGameToolBar;
    QToolBar* pToolsToolBar;
    QToolBar* pAboutToolBar;

    void Initialise();
    void CreateActions();
    void CreateMenus();
    void CreateToolBar();
    void CreateStatusBar();
    void CreateLanguageMenu();
    void LoadSettings();
    void ParseBars(QDomElement&);
    void LoadScores();
    //QString sAddChampion();
    //void WriteScores(int);

public slots:
    void LanguageChangeSlot();

private slots:
    void TimeSlot();
    void GameNewSlot();
    void ToolsStatisticsSlot();
    void ToolsSaveSettingsSlot();
    void AboutSlot();
    void AboutQtSlot();
    void SwitchLanguageSlot(QAction*);
    void NewMoveSlot(int);
    void ClearStatisticsSlot();
    void GameFinishedSlot();
};
//------------------------------------------------------------
#endif
