#ifndef QTCAPRA_H
#define QTCAPRA_H

#include <QWidget>
#include <QProcess>
#include <QSpinBox>

namespace Ui {
    class QtCapra;
}

class QtCapra : public QWidget
{
    Q_OBJECT

public:
    explicit QtCapra(QWidget *parent = 0);
    ~QtCapra();
    QString program;


protected:
    virtual void initProcess( QSpinBox* sb, QStringList arguments );

protected slots:
    virtual void readOmOutput();
    virtual void writeOmOutput();

private:
    Ui::QtCapra *ui;
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
