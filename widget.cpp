#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //receive ctrl message from network
    mpNetwork = new CNetwork;
    cVideoPlay = new CVideoPlay;
    mpNetwork->startServer();

    qRegisterMetaType<QLinkedList<CMessage*> >("QLinkedList<CMessage*>");
    QObject::connect(mpNetwork, SIGNAL(neworkCtrl2VideoSignal(QLinkedList<CMessage*> )),
                     cVideoPlay,SLOT(handleCtrl2VideoSignal(QLinkedList<CMessage*> )),Qt::QueuedConnection);
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
