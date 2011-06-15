#include "./qtcapra.h"
#include "./ui_qtcapra.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif

//------
MyProcess::MyProcess( QObject* )
: QProcess()
{
    _item = NULL;
}

MyProcess::~MyProcess() {}

//------



QtCapra::QtCapra(QWidget *parent)
: QWidget(parent),
    ui(new Ui::QtCapra) {
    ui->setupUi(this);
    //program = "/home/jack/GIT/qtmoko/src/3rdparty/applications/qtcapra/bin/om";


    //--- connections ---
    //connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)), ui->battery_chargerLimitValue,  SLOT(setValue(int)));
    //connect(ui->battery_chargerLimitValue, SIGNAL(valueChanged(int)), ui->battery_chargerLimit,  SLOT(setValue(int)));


    //--- Battery ---
    //initProcess( ui->battery_energy, arguments );
    //arguments << "battery" << "charger-limit";
    //initProcess( ui->battery_chargerLimitValue, arguments );
    //arguments << "battery" << "temperature";
    //initProcess( ui->battery_temperature, arguments );
    //arguments << "battery" << "consumption";
    //initProcess( ui->battery_consumption, arguments );
}

//void QtCapra::readOmOutput() {
//    MyProcess* proc = dynamic_cast<MyProcess*>(sender());

//    QByteArray myStdOut = proc->readAllStandardOutput();
//    QString val(myStdOut);

//    if( !proc->getAssociatedItem() )
//        return;

//    proc->getAssociatedItem()->setValue(val.toInt());
//}

//void QtCapra::writeOmOutput() {
//    MyProcess* proc = dynamic_cast<MyProcess*>(sender());

//    QByteArray myStdOut = proc->readAllStandardOutput();
//    QString val(myStdOut);

//    if( !proc->getAssociatedItem() )
//        return;

//    proc->getAssociatedItem()->setValue(val.toInt());
//}

void QtCapra::initProcess(QStringList prog, QStringList args ) {
    //TODO change absolute path with something smarter...


    MyProcess* p = new MyProcess();
    //p->setAssociatedItem(sb);
    //connect(p, SIGNAL(finished(int)), this, SLOT(readOmOutput()));

    //launch om
    p->start(prog, args);
}

QtCapra::~QtCapra() {
    delete ui;
}

void QtCapra::on_quitButton_clicked()
{
    delete ui;
}

void QtCapra::on_startButton_clicked()
{
    program = "/usr/bin/mplayer";
    QStringList arguments;
    arguments << "-volume 100" << "/opt/qtmoko/sounds/capra.mp3";
    initProcess(program, arguments);
}
