#include "./qtcapra.h"
#include "./ui_qtcapra.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif

//------
//MyProcess::MyProcess( QObject* )
//: QProcess()
//{
//    _item = NULL;
//}

//MyProcess::~MyProcess() {}

//------



QtCapra::QtCapra(QWidget *parent)
: QWidget(parent),
    ui(new Ui::QtCapra) {
    ui->setupUi(this);
    //program = "/home/jack/GIT/qtmoko/src/3rdparty/applications/qtcapra/bin/om";

    _rh     = new RotateHelper();
    _volume = 100;

    connect(_rh, SIGNAL(rotated(bool)), this, SLOT( on_startButton_clicked() ));
    connect(ui->volume, SIGNAL(valueChanged(int)), this,  SLOT(onVolumeChanged(int)));

    _rh->start(); // the default sample rate is 500ms

//    rh->stop();

//    rh->restore();

    //--- connections ---

}

//void QtCapra::initProcess(QString prog, QStringList args ) {
//    //TODO change absolute path with something smarter...


//    MyProcess* p = new MyProcess();
//    //p->setAssociatedItem(sb);
//    //connect(p, SIGNAL(finished(int)), this, SLOT(readOmOutput()));

//    //launch om
//    p->start(prog, args);
//}

QtCapra::~QtCapra() {
    delete ui;
}

void QtCapra::on_quitButton_clicked()
{
    _rh->stop();
    close();
}

void QtCapra::on_startButton_clicked()
{
    QString command = "/usr/bin/mplayer -volume " + QString::number(_volume) + " /opt/qtmoko/sounds/capra.mp3";

    system(qPrintable(command));
    //system( command.toStdString().c_str() );
}
