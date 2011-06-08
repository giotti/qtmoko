#ifndef QTOM_H
#define QTOM_H

#include <QWidget>
#include <QProcess>
#include <QSpinBox>

namespace Ui {
    class QtOm;
}

class QtOm : public QWidget
{
    Q_OBJECT

public:
    explicit QtOm(QWidget *parent = 0);
    ~QtOm();
    QString program;


protected:
    virtual void initProcess( QSpinBox* sb, QStringList arguments );

protected slots:
    virtual void readOmOutput();
    virtual void writeOmOutput();

private:
    Ui::QtOm *ui;
};


/**
  Initialize a custom process which store associated item pointer
*/
class MyProcess : public QProcess
{
    Q_OBJECT

public:
    explicit MyProcess(QObject *parent = 0);
    ~MyProcess();

    void setAssociatedItem( QSpinBox* sb ) { _item = sb; };
    QSpinBox* getAssociatedItem() { return _item; };

private:
    QSpinBox* _item; //associated item
};

#endif // QTOM_H
