#ifndef QTOM_H
#define QTOM_H

#include <QWidget>

namespace Ui {
    class QtOm;
}

class QtOm : public QWidget
{
    Q_OBJECT

public:
    explicit QtOm(QWidget *parent = 0);
    ~QtOm();

private:
    Ui::QtOm *ui;
};

#endif // QTOM_H
