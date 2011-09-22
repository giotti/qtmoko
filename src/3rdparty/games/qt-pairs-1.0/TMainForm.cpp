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
#include "TStatisticsForm.h"
//------------------------------------------------------------
#include <QtGui>
#include <QtXml>
//------------------------------------------------------------
TMainForm::TMainForm(QWidget* pParent, Qt::WindowFlags Flag)
    : QMainWindow(pParent, Flag)
{
    Initialise();
    CreateActions();
    CreateToolBar();
    LoadSettings();
    CreateMenus();
    CreateStatusBar();
    LanguageChangeSlot();
}
//------------------------------------------------------------
void TMainForm::LanguageChangeSlot()
{
    this->setWindowTitle(tr("Find the pairs"));

    pGameExitAction->setText(tr("Quit"));
    pGameExitAction->setStatusTip(tr("Finish programme's work"));

    pGameNewAction->setText(tr("New"));
    pGameNewAction->setStatusTip(tr("Start a new game"));

    pToolsStatisticsAction->setText(tr("Statistics"));
    pToolsStatisticsAction->setStatusTip(tr("Show records' table"));

    pToolsSaveSettingsAction->setText(tr("Save settings"));
    pToolsSaveSettingsAction->setStatusTip(tr("Save the current view of the programme"));

    pAboutAction->setText(tr("About the programme"));
    pAboutAction->setStatusTip(tr("Show brief information about this programme"));

    pAboutQtAction->setText(tr("About Qt library"));
    pAboutQtAction->setStatusTip(tr("Show brief information about Qt library"));

    pGameMenu->setTitle(tr("Game"));
    pToolsMenu->setTitle(tr("Tools"));
    pLanguageMenu->setTitle(tr("Language"));

    pGameToolBar->setWindowTitle(tr("\"Game\" panel"));
    pToolsToolBar->setWindowTitle(tr("\"Tools\" panel"));
    pAboutToolBar->setWindowTitle(tr("\"About\" panel"));

    pMovesLabel->setText(tr("Moves: 0"));
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

    pToolsStatisticsAction = new QAction(this);
    pToolsStatisticsAction->setIcon(QIcon(":/images/stat.png"));
    pToolsStatisticsAction->setShortcut(QKeySequence("F4"));
    connect(pToolsStatisticsAction, SIGNAL(activated()), this, SLOT(ToolsStatisticsSlot()));

    pToolsSaveSettingsAction = new QAction(this);
    pToolsSaveSettingsAction->setIcon(QIcon(":/images/save.png"));
    pToolsSaveSettingsAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(pToolsSaveSettingsAction, SIGNAL(activated()), this, SLOT(ToolsSaveSettingsSlot()));


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

    pToolsMenu = this->menuBar()->addMenu(tr("Tools"));
    pToolsMenu->addAction(pToolsStatisticsAction);
    pToolsMenu->addSeparator();
    pToolsMenu->addAction(pToolsSaveSettingsAction);

    CreateLanguageMenu();

    pAboutMenu = this->menuBar()->addMenu("?");
    pAboutMenu->addAction(pAboutAction);
    pAboutMenu->addAction(pAboutQtAction);
}
//------------------------------------------------------------
void TMainForm::CreateToolBar()
{
    pGameToolBar = this->addToolBar(tr("\"Game\" panel"));
    pGameToolBar->addAction(pGameExitAction);
    pGameToolBar->addSeparator();
    pGameToolBar->addAction(pGameNewAction);
    pGameToolBar->addSeparator();
    pGameToolBar->setMovable(false);

    pToolsToolBar = this->addToolBar(tr("\"Tools\" panel"));
    pToolsToolBar->addAction(pToolsStatisticsAction);
    pToolsToolBar->addSeparator();
    pToolsToolBar->addAction(pToolsSaveSettingsAction);
    pToolsToolBar->addSeparator();
    pToolsToolBar->setMovable(false);

    pAboutToolBar = this->addToolBar(tr("\"About\" panel"));
    pAboutToolBar->addAction(pAboutAction);
    pAboutToolBar->addAction(pAboutQtAction);
    pAboutToolBar->addSeparator();
    pAboutToolBar->setMovable(false);
}
//------------------------------------------------------------
void TMainForm::CreateStatusBar()
{
    pDateTimeLabel = new QLabel(this);
    pDateTimeLabel->setContentsMargins(3, 3, 3, 3);
    pMovesLabel = new QLabel(this);
    pMovesLabel->setContentsMargins(3, 3, 3, 3);
    //statusBar()->addWidget(pMovesLabel);
    //statusBar()->addWidget(pDateTimeLabel);
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
void TMainForm::ToolsStatisticsSlot()
{
    TStatisticsForm* pStatisticsDialog = new TStatisticsForm(this);
    connect(pStatisticsDialog, SIGNAL(ClearStatistics()), this, SLOT(ClearStatisticsSlot()));

    LoadScores();
    for(int i = 0; i < Scores.size(); ++i)
    {
        QTableWidgetItem* pFirstItem = new QTableWidgetItem(QString::number((int)Scores.at(i).iMoves));
        pStatisticsDialog->pScoresBox->setItem(i, 0, pFirstItem);
        QTableWidgetItem* pSecondItem = new QTableWidgetItem(Scores.at(i).sName);
        pStatisticsDialog->pScoresBox->setItem(i, 1, pSecondItem);
    }
    pStatisticsDialog->exec();
    delete pStatisticsDialog;
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
void TMainForm::CreateLanguageMenu()
{
    pLanguageMenu = menuBar()->addMenu(tr("Language"));

    pLanguageActions = new QActionGroup(this);

    QString sQmPath = qApp->applicationDirPath() + "/translations";
    QDir TranslationsFolder(sQmPath);
    QStringList sTranslations = TranslationsFolder.entryList(QStringList("qt-pairs_*.qm"));

    for(int i = 0; i < sTranslations.size(); i++)
    {
        QTranslator Translator;
        Translator.load(sTranslations[i], sQmPath);
        QString sLanguage = Translator.translate("TMainForm", "English");

        QAction* pAction = new QAction(sLanguage, this);
        pAction->setCheckable(true);
        pLanguageMenu->addAction(pAction);
        pLanguageActions->addAction(pAction);

        sTranslations[i].remove("qt-pairs_", Qt::CaseSensitive);
        sTranslations[i].remove(".qm", Qt::CaseSensitive);

        pAction->setData(sTranslations[i]);

        if(sLocale == sTranslations[i])
        {
            pAction->setChecked(true);
        }
    }

    connect(pLanguageActions, SIGNAL(triggered(QAction*)), this, SLOT(SwitchLanguageSlot(QAction*)));
}
//------------------------------------------------------------
void TMainForm::SwitchLanguageSlot(QAction* pAction)
{
    QString sQmPath = qApp->applicationDirPath() + "/translations";
    sLocale = pAction->data().toString();

    QTranslator* pTranslator = new QTranslator(0);
    pTranslator->load(("qt-pairs_" + sLocale + ".qm"), sQmPath);
    qApp->installTranslator(pTranslator);

    LanguageChangeSlot();
}
//------------------------------------------------------------
void TMainForm::NewMoveSlot(int iMoves)
{
    QString sMoves = tr("Moves: ") + QString::number(iMoves);
    pMovesLabel->setText(sMoves);
}
//------------------------------------------------------------
void TMainForm::ClearStatisticsSlot()
{
    QDir HomeDirectory;
    QString sScoresPath = HomeDirectory.homePath() + "/.qt-pairs";
    QString sScoresFilePath = sScoresPath + "/scores.txt";
    QDir ScoresDirectory(sScoresPath);
    if(ScoresDirectory.exists() == false)
    {
        ScoresDirectory.mkdir(sScoresPath);
    }
    QFile InputFile(sScoresFilePath);
    InputFile.remove();
}
//------------------------------------------------------------
void TMainForm::LoadScores()
{
    Scores.clear();
    QDir HomeDirectory;
    QString sScoresPath = HomeDirectory.homePath() + "/.qt-pairs";
    QString sScoresFilePath = sScoresPath + "/scores.txt";

    if(QFile::exists(sScoresFilePath))
    {
        QFile OutputFile(sScoresFilePath);
        if(OutputFile.open(QIODevice::ReadOnly))
        {
            QTextStream TextStream(& OutputFile);
            int iPosition;
            QString sTmpString, sScore;
            do
            {
                sTmpString = TextStream.readLine();
                iPosition = sTmpString.indexOf(" - ", 0, Qt::CaseSensitive);
                sScore = sTmpString.left(iPosition);
                TChampion Champion;
                if(!sTmpString.isNull())
                {
                    Champion.iMoves = sScore.toInt(0, 10);
                    Champion.sName = sTmpString.remove((sScore + " - "), Qt::CaseSensitive);
                    Scores.push_back(Champion);
                }
            }
            while(!sTmpString.isNull());
        }
    }
    else
    {
        QDir ScoresDirectory(sScoresPath);
        if(ScoresDirectory.exists() == false)
        {
            ScoresDirectory.mkdir(sScoresPath);
        }
        QFile InputFile(sScoresFilePath);
        if(InputFile.open(QIODevice::WriteOnly))
        {
            InputFile.close();
        }
    }
}
//------------------------------------------------------------
/*QString TMainForm::sAddChampion()*/
//{
    //bool bDone;
    //QString sName = QInputDialog::getText(this, tr("New record!"),
                                          //tr("Input your name:"),
                                          //QLineEdit::Normal,
                                          //tr("Unknown"), & bDone);
    //if(sName.isEmpty()) sName = tr("Unknown");

    //return sName;
/*}*/
//------------------------------------------------------------
/*void TMainForm::WriteScores(int iPosition)*/
//{
    //QString sName = sAddChampion();
    //QStringList sRecords;
    //QString sRecord;
    //TChampion Champion;
    //Champion.iMoves = pCentralWidget->iMoves;
    //Champion.sName = sName;
    //Scores.insert(iPosition, Champion);
    //if(Scores.size() > 10) Scores.removeAt(10);

    //for(int i = 0; i < Scores.size(); ++i)
    //{
        //sRecord = QString::number((int)Scores.at(i).iMoves) + " - " + Scores.at(i).sName;
        //sRecords << sRecord;
    //}

    //QDir HomeDirectory;
    //QString sScoresPath = HomeDirectory.homePath() + "/.qt-pairs";
    //QString sScoresFilePath = sScoresPath + "/scores.txt";

    //QFile InputFile(sScoresFilePath);
    //if(InputFile.open(QIODevice::WriteOnly))
    //{
        //QTextStream TextStream(& InputFile);
        //for(int i = 0; i < sRecords.size(); ++i)
        //{
            //TextStream << sRecords.at(i) << endl;
        //}
        //InputFile.close();
    //}
/*}*/
//------------------------------------------------------------
void TMainForm::GameFinishedSlot()
{
    LoadScores();
    bool bDone = false;
    int i = 0;
    if(Scores.size() == 0)
    {
        //WriteScores(0);
        bDone = true;
    }
    else
    {
        for( ; i < Scores.size(); ++i)
        {
            if(pCentralWidget->iMoves <= Scores.at(i).iMoves)
            {
                //WriteScores(i);
                bDone = true;
                break;
            }
        }
    }

    //if((bDone == false) && (Scores.size() <= 9)) WriteScores(i);
}
//------------------------------------------------------------
void TMainForm::ToolsSaveSettingsSlot()
{
    QDomDocument SettingsDocument;
    QDomElement Root = SettingsDocument.createElement("settings");

    QDomElement LanguageElement = SettingsDocument.createElement("lang");
    QDomText LanguageText = SettingsDocument.createTextNode(sLocale);

    QDomElement BarsElement = SettingsDocument.createElement("bars");
    QDomElement GameElement = SettingsDocument.createElement("game");
    QDomElement ToolsElement = SettingsDocument.createElement("tools");
    QDomElement AboutElement = SettingsDocument.createElement("about");
    QDomText GameText;
    if(pGameToolBar->isVisible()) GameText = SettingsDocument.createTextNode("on");
    else GameText = SettingsDocument.createTextNode("off");
    QDomText ToolsText;
    if(pToolsToolBar->isVisible()) ToolsText = SettingsDocument.createTextNode("on");
    else ToolsText = SettingsDocument.createTextNode("off");
    QDomText AboutText;
    if(pAboutToolBar->isVisible()) AboutText = SettingsDocument.createTextNode("on");
    else AboutText = SettingsDocument.createTextNode("off");

    SettingsDocument.appendChild(Root);
    Root.appendChild(LanguageElement);
    LanguageElement.appendChild(LanguageText);
    Root.appendChild(BarsElement);
    BarsElement.appendChild(GameElement);
    GameElement.appendChild(GameText);
    BarsElement.appendChild(ToolsElement);
    ToolsElement.appendChild(ToolsText);
    BarsElement.appendChild(AboutElement);
    AboutElement.appendChild(AboutText);

    QDir HomeDirectory;
    QString sSettingsPath = HomeDirectory.homePath() + "/.qt-pairs";
    QString sSettingsFilePath = sSettingsPath + "/settings.xml";
    QDir SettingsDirectory(sSettingsPath);
    if(SettingsDirectory.exists() == false)
    {
        SettingsDirectory.mkdir(sSettingsPath);
    }
    QFile InputFile(sSettingsFilePath);
    if(InputFile.open(QIODevice::WriteOnly))
    {
        QTextStream TextStream(&InputFile);
        QDomNode ProcessingElement = SettingsDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
        SettingsDocument.insertBefore(ProcessingElement, SettingsDocument.firstChild());
        SettingsDocument.save(TextStream, 4);
        InputFile.close();
    }
}
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
    QString sGameOff, sToolsOff, sAboutOff;
    QDomNode ChildNode = DomElement.firstChild();
    while(!ChildNode.isNull())
    {
        if(ChildNode.toElement().tagName() == "game")
        {
            QDomNode DomNode = ChildNode.firstChild();
            if(DomNode.nodeType() == QDomNode::TextNode)
            {
                sGameOff = DomNode.toText().data();
                if(sGameOff == "off") pGameToolBar->hide();
            }
        }
        if(ChildNode.toElement().tagName() == "tools")
        {
            QDomNode DomNode = ChildNode.firstChild();
            if(DomNode.nodeType() == QDomNode::TextNode)
            {
                sToolsOff = DomNode.toText().data();
                if(sToolsOff == "off") pToolsToolBar->hide();
            }
        }
        if(ChildNode.toElement().tagName() == "about")
        {
            QDomNode DomNode = ChildNode.firstChild();
            if(DomNode.nodeType() == QDomNode::TextNode)
            {
                sAboutOff = DomNode.toText().data();
                if(sAboutOff == "off") pAboutToolBar->hide();
            }
        }
        ChildNode = ChildNode.nextSibling();
    }
}
//------------------------------------------------------------
