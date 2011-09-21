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
#ifndef _TStatisticsForm_h_
#define _TStatisticsForm_h_
//------------------------------------------------------------
#include <QDialog>
//------------------------------------------------------------
class QVBoxLayout;
class QLabel;
class QTableWidget;
class QHBoxLayout;
class QPushButton;
//------------------------------------------------------------
class TStatisticsForm : public QDialog
{
    Q_OBJECT

public: 
    TStatisticsForm(QWidget* pParent = 0, Qt::WindowFlags Flag = Qt::Dialog);

    QTableWidget* pScoresBox;

private:
    QVBoxLayout* pMainLayout;
    QHBoxLayout* pButtonsLayout;

    QLabel* pMovesLabel;
    QLabel* pSeparator;
    QPushButton* pAcceptButton;
    QPushButton* pClearButton;

signals:
    void ClearStatistics();

protected slots:
    void ClearStatisticsSlot();
};
//------------------------------------------------------------
#endif
