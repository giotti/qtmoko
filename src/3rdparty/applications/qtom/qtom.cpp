#include "./qtom.h"
#include "./ui_qtom.h"

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



QtOm::QtOm(QWidget *parent)
: QWidget(parent),
    ui(new Ui::QtOm) {
    ui->setupUi(this);
    //program = "/home/jack/GIT/qtmoko/src/3rdparty/applications/qtom/bin/om";
    program = "/usr/bin/om";

    //--- connections ---
    connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)), ui->battery_chargerLimitValue,  SLOT(setValue(int)));
    connect(ui->battery_chargerLimitValue, SIGNAL(valueChanged(int)), ui->battery_chargerLimit,  SLOT(setValue(int)));

    QStringList arguments;

    //--- Battery ---
    arguments << "battery" << "energy";
    initProcess( ui->battery_energy, arguments );
    arguments << "battery" << "charger-limit";
    initProcess( ui->battery_chargerLimitValue, arguments );
    arguments << "battery" << "temperature";
    initProcess( ui->battery_temperature, arguments );
    arguments << "battery" << "consumption";
    initProcess( ui->battery_consumption, arguments );
}

void QtOm::readOmOutput() {
    MyProcess* proc = dynamic_cast<MyProcess*>(sender());

    QByteArray myStdOut = proc->readAllStandardOutput();
    QString val(myStdOut);

    if( !proc->getAssociatedItem() )
        return;

    proc->getAssociatedItem()->setValue(val.toInt());
}

void QtOm::writeOmOutput() {
    MyProcess* proc = dynamic_cast<MyProcess*>(sender());

    QByteArray myStdOut = proc->readAllStandardOutput();
    QString val(myStdOut);

    if( !proc->getAssociatedItem() )
        return;

    proc->getAssociatedItem()->setValue(val.toInt());
}

void QtOm::initProcess( QSpinBox* sb, QStringList args ) {
    //TODO change absolute path with something smarter...


    MyProcess* p = new MyProcess();
    p->setAssociatedItem(sb);
    connect(p, SIGNAL(finished(int)), this, SLOT(readOmOutput()));

    //launch om
    p->start(program, args);
}

QtOm::~QtOm() {
    delete ui;
}
