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
#include "TGameBoard.h"
//#include "TStatisticsForm.h"
//------------------------------------------------------------
#include <QtGui>
#include <QtXml>
//------------------------------------------------------------
TMainForm::TMainForm(QWidget* pParent, Qt::WindowFlags Flag)
    : QMainWindow(pParent, Flag)
{
    Initialise();
    CreateActions();
    LoadSettings();
    CreateMenus();
    CreateStatusBar();
}
//------------------------------------------------------------
void TMainForm::Initialise()
{
    this->setWindowIcon(QIcon(":/images/main.png"));
    pCentralWidget = new TGameBoard(this);
    this->setCentralWidget(pCentralWidget);
    connect(pCentralWidget, SIGNAL(GameFinished()), this, SLOT(GameFinishedSlot()));
    connect(pCentralWidget, SIGNAL(NewGame()), this, SLOT(GameNewSlot()));
    connect(pCentralWidget, SIGNAL(Clicked(int)), this, SLOT(NewMoveSlot(int)));
}
//------------------------------------------------------------
void TMainForm::CreateActions()
{
    pGameExitAction = new QAction(this);
    pGameExitAction->setIcon(QIcon(":/images/exit.png"));
    pGameExitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(pGameExitAction, SIGNAL(activated()), qApp, SLOT(quit()));

    pGameNewAction = new QAction(this);
    pGameNewAction->setIcon(QIcon(":/images/new.png"));
    pGameNewAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(pGameNewAction, SIGNAL(activated()), this, SLOT(GameNewSlot()));

    pAboutAction = new QAction(this);
    pAboutAction->setIcon(QIcon(":/images/about.png"));
    pAboutAction->setShortcut(QKeySequence("F1"));
    connect(pAboutAction, SIGNAL(activated()), this, SLOT(AboutSlot()));

    pAboutQtAction = new QAction(this);
    pAboutQtAction->setIcon(QIcon(":/images/Qt.png"));
    pAboutQtAction->setShortcut(QKeySequence("F2"));
    connect(pAboutQtAction, SIGNAL(activated()), this, SLOT(AboutQtSlot()));
}
//------------------------------------------------------------
void TMainForm::CreateMenus()
{
    pGameMenu = this->menuBar()->addMenu(tr("Game"));
    pGameMenu->addAction(pGameNewAction);
    pGameMenu->addSeparator();
    pGameMenu->addAction(pGameExitAction);

    pAboutMenu = this->menuBar()->addMenu("?");
    pAboutMenu->addAction(pAboutAction);
    pAboutMenu->addAction(pAboutQtAction);
}
//------------------------------------------------------------
void TMainForm::CreateStatusBar()
{
    pDateTimeLabel = new QLabel(this);
    pDateTimeLabel->setContentsMargins(3, 3, 3, 3);
    pMovesLabel = new QLabel(this);
    pMovesLabel->setContentsMargins(3, 3, 3, 3);
    pTimer = new QTimer(this);
    TimeSlot();
    connect(pTimer, SIGNAL(timeout()), this, SLOT(TimeSlot()));
    pTimer->start(1000);
}
//------------------------------------------------------------
void TMainForm::TimeSlot()
{
    QDateTime Time = QDateTime::currentDateTime();
    pDateTimeLabel->setText(Time.toString("dd.MM.yyyy - hh:mm:ss"));
}
//------------------------------------------------------------
void TMainForm::GameNewSlot()
{
    pCentralWidget->Shuffle();
    pCentralWidget->iFreeCells = 8;
    pMovesLabel->setText(tr("Moves: 0"));
}
//------------------------------------------------------------
void TMainForm::AboutSlot()
{
    QMessageBox::about(this, tr("About the programme"), tr("<h2>Qt-Pairs 1.0</h2> <p>Copyright &copy; Andrey N. Kuznetsov, 2010 (<a href=http://linmedsoft.narod.ru>http://linmedsoft.narod.ru</a>) <p>Find the pairs.</p> <p>A classic memory game.</p> <p>Uses the icons' kit of ArcticLine Software, 2006 (<a href=http://www.ReadyIcons.com>http://www.ReadyIcons.com</a>)</p> <p>Uses the Cyberella's Archigraph Plasticfarm icons' kit, 2008 (<a href=http://www.archigraphs.com>http://www.archigraphs.com</a>)</p>"));
}
//------------------------------------------------------------
void TMainForm::AboutQtSlot()
{
    QMessageBox::aboutQt(this, "Qt-Pairs");
}
//------------------------------------------------------------
void TMainForm::NewMoveSlot(int iMoves)
{
    QString sMoves = tr("Moves: ") + QString::number(iMoves);
    pMovesLabel->setText(sMoves);
}
//------------------------------------------------------------
void TMainForm::GameFinishedSlot() { }
//------------------------------------------------------------
void TMainForm::LoadSettings()
{
    QDomDocument SettingsDocument;
    QDir HomeDirectory;
    QString sSettingsPath = HomeDirectory.homePath() + "/.qt-pairs";
    QString sSettingsFilePath = sSettingsPath + "/settings.xml";
    if(QFile::exists(sSettingsFilePath))
    {
        QFile OutputFile(sSettingsFilePath);
        if(OutputFile.open(QIODevice::ReadOnly))
        {
            if(!SettingsDocument.setContent(& OutputFile))
            {
                OutputFile.close();
                return;
            }
            QDomElement DomElement = SettingsDocument.documentElement();
            QDomNode DomNode = DomElement.firstChild();
            while(!DomNode.isNull())
            {
                DomElement = DomNode.toElement();
                if(DomElement.tagName() == "lang")
                {
                    QDomNode DomNode = DomElement.firstChild();
                    if(DomNode.nodeType() == QDomNode::TextNode)
                    {
                        sLocale = DomNode.toText().data();
                    }
                }
                if(DomElement.tagName() == "bars")
                {
                    ParseBars(DomElement);
                }
                DomNode = DomNode.nextSibling();
            }
        }
    }
    else
    {
        QDir SettingsDirectory(sSettingsPath);
        if(SettingsDirectory.exists() == false)
        {
            SettingsDirectory.mkdir(sSettingsPath);
        }
        QFile InputFile(sSettingsFilePath);
        if(InputFile.open(QIODevice::WriteOnly))
        {
            InputFile.close();
        }
    }
}
//------------------------------------------------------------
void TMainForm::ParseBars(QDomElement& DomElement)
{
    QString sGameOff, sAboutOff;
    QDomNode ChildNode = DomElement.firstChild();
    while(!ChildNode.isNull())
    {
        if(ChildNode.toElement().tagName() == "game")
        {
            QDomNode DomNode = ChildNode.firstChild();
            if(DomNode.nodeType() == QDomNode::TextNode)
            {
                sGameOff = DomNode.toText().data();
                //if(sGameOff == "off") pGameToolBar->hide();
            }
        }
        if(ChildNode.toElement().tagName() == "tools")
        {
            QDomNode DomNode = ChildNode.firstChild();
            //if(DomNode.nodeType() == QDomNode::TextNode)
            //{
                //sToolsOff = DomNode.toText().data();
		//if(sToolsOff == "off") pToolsToolBar->hide();
            //}
        }
        if(ChildNode.toElement().tagName() == "about")
        {
            QDomNode DomNode = ChildNode.firstChild();
            //if(DomNode.nodeType() == QDomNode::TextNode)
            //{
                //sAboutOff = DomNode.toText().data();
		//if(sAboutOff == "off") pAboutToolBar->hide();
            //}
        }
        ChildNode = ChildNode.nextSibling();
    }
}
//------------------------------------------------------------
