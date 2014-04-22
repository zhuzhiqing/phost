#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "globalValue.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //receive ctrl message from network
    mpNetwork = new CNetwork;
    cVideoPlay = new CVideoPlay;
    mpNetwork->startServer();
    QObject::connect(mpNetwork, SIGNAL(neworkCtrl2VideoSignal()),cVideoPlay,SLOT(handleCtrl2VideoSignal()),Qt::QueuedConnection);
 //   QObject::connect(this, SIGNAL(ctrlMsg2Video()),cVideoPlay,SLOT(handleCtrlMsg2Video()),Qt::QueuedConnection);
 //   QObject::connect(mpNetwork, SIGNAL(neworkCtrlMsg()),this,SIGNAL(ctrlMsg2Video()),Qt::QueuedConnection);
}

Widget::~Widget()
{
    delete ui;
    delete mpNetwork;
    delete cVideoPlay;
}

void Widget::on_btnVideoMode_clicked()
{
    cVideoPlay->show();
}

void Widget::on_showConnect_clicked()
{

}
