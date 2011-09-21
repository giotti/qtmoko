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
#include "TStatisticsForm.h"
//------------------------------------------------------------
#include <QtGui>
//------------------------------------------------------------
TStatisticsForm::TStatisticsForm(QWidget* pParent, Qt::WindowFlags Flag)
               : QDialog(pParent, Flag)
{
    this->setWindowTitle(tr("Records' table"));
    this->setModal(true);

    pMainLayout = new QVBoxLayout(this);

    pMovesLabel = new QLabel(this);
    pMovesLabel->setText(tr("<p><b>Сhampions' list:</b></p>"));
    pMainLayout->addWidget(pMovesLabel);

    pScoresBox = new QTableWidget(this);
    QStringList sLabels;
    sLabels << tr("Moves number") << tr("Name");
    pScoresBox->setColumnCount(2);
    pScoresBox->setColumnWidth(0, 150);
    pScoresBox->setColumnWidth(1, 150);
    pScoresBox->setRowCount(10);
    pScoresBox->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pScoresBox->setHorizontalHeaderLabels(sLabels);
    pMainLayout->addWidget(pScoresBox);

    pSeparator = new QLabel(this);
    pSeparator->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    pSeparator->setLineWidth(1);
    pSeparator->setMargin(2);
    pMainLayout->addWidget(pSeparator);

    pButtonsLayout = new QHBoxLayout(0);
    pButtonsLayout->addStretch(0);

    pAcceptButton = new QPushButton(this);
    pAcceptButton->setText(tr("Accept"));
    pAcceptButton->setFocus();
    pButtonsLayout->addWidget(pAcceptButton);

    pClearButton = new QPushButton(this);
    pClearButton->setText(tr("Clear"));
    pButtonsLayout->addWidget(pClearButton);

    pMainLayout->addLayout(pButtonsLayout);

    this->setMinimumWidth(360);
    this->setMinimumHeight(360);
    this->setMaximumWidth(360);
    this->setMaximumHeight(360);
    /*this->setMinimumHeight(500);
    if(this->width() > this->height()){
        this->setMinimumWidth(500);
        this->setMinimumHeight(360);
    }*/
    connect(pAcceptButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(pClearButton, SIGNAL(clicked()), this, SLOT(ClearStatisticsSlot()));
}
//------------------------------------------------------------
void TStatisticsForm::ClearStatisticsSlot()
{
    int iAnswer = QMessageBox::warning(this, tr("Erase all records?"), tr("Do you really wish to clear the records' list?"), tr("Yes"), tr("No"), 0, 1);
    switch(iAnswer)
    {
        case 0:
        {
            pScoresBox->clear();
            QStringList sLabels;
            sLabels << tr("Moves number") << tr("Name");
            pScoresBox->setHorizontalHeaderLabels(sLabels);
            emit ClearStatistics();
        }
    }
}
//------------------------------------------------------------
