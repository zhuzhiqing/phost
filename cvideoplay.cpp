#include "cvideoplay.h"
#include "ui_cvideoplay.h"
#include <QDir>
#include <QString>
#include <QDebug>
#include <QFile>

#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QTime>
#include <QSlider>

#include "constvaule.h"


CVideoPlay::CVideoPlay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVideoPlay)
{
    ui->setupUi(this);
    mPlayer = "mplayer";
    pmProcess = new QProcess;
    mPlayState = false;
    mMuteFlag = false;

    pmTimer = new QTimer(this);
    connect(pmTimer,SIGNAL(timeout()),this,SLOT(get_time_slots()));
    pmTimer->start(1000);
    connect(pmProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(action_triggered()));

}

CVideoPlay::~CVideoPlay()
{
    delete ui;
    delete pmProcess;
    delete pmTimer;
}

void CVideoPlay::on_btnScan_clicked()
{
    QDir dir(PATH);
    QStringList fileList;
    fileList<<"*.avi"<< "*.wmv"<< "*.mp3" <<"*.rmvb"<< "*.mkv"<<"*.mp4";

    if(dir.exists())
    {
        QStringList list = dir.entryList(fileList);
        ui->listWidgets->clear();
        ui->listWidgets->insertItems(0,list);
    }
}


void CVideoPlay::on_btnPlay_clicked()
{
    QStringList cmd_list;

    cmd_list<<"-wid";
    cmd_list<<QString::number(ui->widget->winId());
    cmd_list<<"-slave";
    cmd_list<<"-quiet";

    pmProcess->start(mPlayer, cmd_list);
    pmProcess->write("get_time_length\n");
    pmTimer->start(1000);


}

void CVideoPlay::on_listWidgets_itemDoubleClicked(QListWidgetItem *item)
{
    QString filename = item->text();

    mMediaFile = PATH+filename;
    QStringList cmd_list;
    cmd_list << mMediaFile;
    cmd_list << "-wid";
    cmd_list << QString ::number(ui->widget->winId());
    cmd_list << "-slave";
    cmd_list << "-quiet";
    if(pmProcess->Running)
    {
        pmProcess->kill();
        pmProcess->waitForFinished(WAIT_FOR_FINISH_TIME);

    }

    pmProcess->start(mPlayer , cmd_list);
    pmTimer->start(1000);
    pmProcess->write("get_time_length\n");
}

void CVideoPlay::action_triggered()
{
    while(pmProcess->canReadLine())
    {
        QString message(pmProcess->readLine());
        qDebug()<<message;

        QStringList message_list=message.split("=");
        if(message_list[0]=="ANS_TIME_POSITION")
        {
            mCurrTime = message_list[1].toDouble();
            QTime time = int_to_time(mCurrTime);
            ui->labelPos->setText(time.toString("hh:mm:ss"));
            ui->sldProgress->setValue(100*mCurrTime/mTotalTime);
            qDebug()<<time.toString("hh:mm:ss");

        }
        else if(message_list[0]=="ANS_LEHGTH")
        {
            mTotalTime = message_list[1].toDouble();
            QTime time = int_to_time(mTotalTime);
            ui->labelTotal->setText(time.toString("hh:mm:ss"));
        }
    }
}

//秒数转换时间函数
QTime CVideoPlay::int_to_time(int second)
{
    int sec=0,min=0,hour=0;

    QTime time;
    if(second<60)
    {
        sec=second;
        min=0;
        hour=0;

    }
    if(second>=60&&second<3600)
    {
        sec=second%60;
        min=second/60;
        hour=0;

    }
    if(second>=3600)
    {
        sec=second%60;
        min=(second/60)%60;
        hour=second/3600;
    }
    time.setHMS(hour,min,sec);
    return time ;

}

void CVideoPlay::get_time_slots()
{
//    qDebug("__get_time_slots__");
//    if(pmProcess->waitForStarted())
//    {
//        pmProcess->write("get_time_pos\n");
//    }
}

void CVideoPlay::on_btnForward_clicked()
{
    if(pmProcess->Running)
    {
        pmProcess->write("seek +5\n");
    }
}

void CVideoPlay::on_btnBackward_clicked()
{
    if(pmProcess->Running)
    {
        pmProcess->write("seek -5\n");
    }
}

void CVideoPlay::on_listWidgets_itemClicked(QListWidgetItem *item)
{
    QString filename = item->text();
    mMediaFile = PATH + filename;
}

void CVideoPlay::on_checkBoxMutex_toggled(bool checked)
{
    if(!checked)
    {
        mMuteFlag = false;
        ui->checkBoxMutex->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        pmProcess->write("mute 1\n");
    }
    else
    {
        mMuteFlag = true;
        ui->checkBoxMutex->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        pmProcess->write("mute 0\n");
    }
}

void CVideoPlay::on_sldProgress_actionTriggered(int action)
{
    if(pmProcess->Running)
    {
        if(pmTimer->isActive())
        {
            pmTimer->stop();
            pmProcess->write("pause\n");
            //获取进度条的增加值
            curvalue(ui->sldProgress->value());
            pmTimer->start(1000);
            pmProcess->write("pause\n");
        }
        else
        {
            curvalue(ui->sldProgress->value());
        }
    }
}

void CVideoPlay::curvalue(int val)
{
    pmProcess->write("pause\n");
    int targetValue = val;
    QString str1 = "seek ";
    QString str2 = " 1\n";
    QString tempStr = str1 + QString::number(targetValue) + str2;
    //const char *str = tempStr.toAscii();
    pmProcess->write("pause\n");
   // pmProcess->write(str);
}


void CVideoPlay::on_btnPrevious_clicked()
{

}

void CVideoPlay::on_btnNext_clicked()
{

}
