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
#include "globalValue.h"

CVideoPlay::CVideoPlay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVideoPlay)
{
    ui->setupUi(this);
    mPlayer = "mplayer";
    pmProcess = new QProcess;
    mPlayState = false;
    mMuteFlag = false;

    //ui icon set
    ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    pmTimer = new QTimer(this);
    connect(pmTimer,SIGNAL(timeout()),this,SLOT(get_time_slots()));
    connect(pmProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(action_triggered()));
}

void CVideoPlay::handleCtrl2VideoSignal()
{
    if(this->isVisible()){
        on_btnScan_clicked();
        on_btnPlay_clicked();
    }

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
    if(pmProcess->Running)
    {
        if(mMediaFile.isEmpty())
        {
            int totalCount = ui->listWidgets->count();
            if(totalCount < 1)
            {
                on_btnScan_clicked();
            }
            ui->listWidgets->setCurrentItem(ui->listWidgets->item(0),
                                            QItemSelectionModel::Select);
            ui->listWidgets->setItemSelected(ui->listWidgets->item(0),
                                             true);
            mMediaFile = PATH+ui->listWidgets->currentItem()->text();
            play();
        }
        else
        {
            if(mPlayState)
            {
                mPlayState = false;
                pmTimer->stop();
                ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                pmProcess->write("pause\n");
            }
            else
            {
                mPlayState = true;
                pmProcess->write("play\n");
                pmProcess->write("get_time_length\n");
                ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
                pmTimer->start(1000);

            }
        }
    }

}

void CVideoPlay::on_listWidgets_itemDoubleClicked(QListWidgetItem *item)
{
    mMediaFile = PATH + item->text();
    play();
}

void CVideoPlay::action_triggered()
{
    while(pmProcess->canReadLine())
    {
        QString message(pmProcess->readLine());

        QStringList message_list=message.split("=");
        if(message_list[0]=="ANS_TIME_POSITION")
        {
            mCurrTime = message_list[1].toDouble();
            QTime time = int_to_time(mCurrTime);
            ui->labelPos->setText(time.toString("hh:mm:ss"));
            ui->sldProgress->setValue(100*mCurrTime/mTotalTime);
         //   qDebug()<<"time_stamp:"<<time.toString("hh:mm:ss");

        }
        else if(message_list[0]=="ANS_LENGTH")
        {
            mTotalTime = message_list[1].toDouble();
            QTime time = int_to_time(mTotalTime);
            ui->labelTotal->setText(time.toString("hh:mm:ss"));
       //     qDebug()<<"total_time:"<<time.toString("hh:mm:ss");
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
    if(pmProcess->waitForStarted())
    {
        pmProcess->write("get_time_pos\n");
    }
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
            //获取进度条的增加值
            curvalue(ui->sldProgress->value());
            pmTimer->start(1000);
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
    QString str2 = " \n";
    QString tempStr = str1 + QString::number(targetValue) + str2;
    const char *str = qPrintable(tempStr);//.toAscii();
    pmProcess->write(str);
    pmProcess->write("play\n");
}


void CVideoPlay::on_btnPrevious_clicked()
{
    int currow = ui->listWidgets->currentRow();
    int totalCount = ui->listWidgets->count();
    if(totalCount > 1)
    {
        if(currow != 0)
        {
            ui->listWidgets->setCurrentItem(ui->listWidgets->item(currow-1),
                                            QItemSelectionModel::Select);
            ui->listWidgets->setItemSelected(ui->listWidgets->item(currow-1),
                                             true);
        }
        else
        {
            ui->listWidgets->setCurrentItem(ui->listWidgets->item(0),
                                            QItemSelectionModel::Select);
            ui->listWidgets->setItemSelected(ui->listWidgets->item(0),
                                             true);
        }
        mMediaFile = PATH+ui->listWidgets->currentItem()->text();
        play();
    }
}

void CVideoPlay::on_btnNext_clicked()
{
    int currow = ui->listWidgets->currentRow();
    int totalCount = ui->listWidgets->count();
    if(totalCount > 1)
    {
        if( (currow+1) != totalCount)
        {
            ui->listWidgets->setCurrentItem(ui->listWidgets->item(currow+1),
                                            QItemSelectionModel::Select);
            ui->listWidgets->setItemSelected(ui->listWidgets->item(currow+1),
                                             true);
        }
        else
        {
            ui->listWidgets->setCurrentItem(ui->listWidgets->item(0),
                                            QItemSelectionModel::Select);
            ui->listWidgets->setItemSelected(ui->listWidgets->item(0),
                                             true);
        }
        mMediaFile = PATH+ui->listWidgets->currentItem()->text();
        play();
    }
}

void CVideoPlay::play()
{
    mPlayState = true;
    ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    QStringList cmd_list;
    cmd_list<< mMediaFile;
    cmd_list<<"-wid";
    cmd_list<<QString::number(ui->widget->winId());
    cmd_list<<"-slave";
    cmd_list<<"-quiet";
 //   cmd_list<<"-af volume=-10 -softvol -softvol-max 100";

    if(pmProcess->Running)
    {
        pmProcess->kill();
        pmProcess->waitForFinished(WAIT_FOR_FINISH_TIME);
    }
    pmProcess->start(mPlayer,cmd_list);
    pmTimer->start(1000);
    pmProcess->write("get_time_length\n");
}

void CVideoPlay::stop()
{
    mPlayState = false;
    ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    if(pmProcess->Running)
    {
        pmProcess->kill();
        pmProcess->waitForFinished(WAIT_FOR_FINISH_TIME);
    }
}
