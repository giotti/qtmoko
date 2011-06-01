#include "qtom.h"
#include "ui_qtom.h"
#include <QProcess>

QtOm::QtOm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtOm)
{
    ui->setupUi(this);

    QObject *parent;
      ...
    QString program = "./path/to/om";
    QProcess *myProcess = new QProcess(parent);

    QStringList arguments;
    arguments << "battery" << "energy";
    myProcess->start(program, arguments);
    myProcess->read(int value);
    connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)),    ui->battery_chargerLimitValue,  SLOT(setValue(int)));

    connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)),    ui->battery_chargerLimitValue,  SLOT(setValue(int)));
}

QtOm::~QtOm()
{
    delete ui;
}
