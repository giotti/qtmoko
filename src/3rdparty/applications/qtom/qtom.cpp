#include "qtom.h"
#include "ui_qtom.h"

QtOm::QtOm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtOm)
{
    ui->setupUi(this);

    connect(ui->battery_chargerLimit, SIGNAL(valueChanged(int)),    ui->battery_chargerLimitValue,  SLOT(setValue(int)));
}

QtOm::~QtOm()
{
    delete ui;
}
