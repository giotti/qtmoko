#include "./qtom.h"
#include "./ui_qtom.h"
#include <QProcess>

QtOm::QtOm(QWidget *parent)
: QWidget(parent),
    ui(new Ui::QtOm) {
    ui->setupUi(this);

    connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)),
 ui->battery_chargerLimitValue,  SLOT(setValue(int)));
    connect(ui->battery_chargerLimitValue, SIGNAL(valueChanged(int)),
 ui->battery_chargerLimit,  SLOT(setValue(int)));

    QObject *obj_parent = new QObject();

    QString program = "/home/jack/GIT/qtmoko/src/3rdparty/applications/qtom-build-desktop/tmp_om/om";
    QProcess *myProcess = new QProcess(obj_parent);

    QStringList arguments;
    arguments << "battery" << "energy";
    myProcess->start(program, arguments);

    int value;// = 100;
    myProcess->read(value);

    ui->battery_chargerLimitValue->setValue(value);
}

QtOm::~QtOm() {
    //myProcess->finish();
    delete ui;
}

