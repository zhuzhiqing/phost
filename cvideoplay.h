#ifndef CVIDEOPLAY_H
#define CVIDEOPLAY_H

#include <QDialog>
#include <QProcess>
#include <QListWidget>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QThread>
#include <QTextCodec>
#include <QLinkedList>

#include "utils.h"



namespace Ui {
class CVideoPlay;
}

class CVideoPlay : public QDialog
{
    Q_OBJECT
    
public:
    explicit CVideoPlay(QWidget *parent = 0);
    ~CVideoPlay();

    void play();

    void stop();
    
private slots:
    void on_btnScan_clicked();

    void on_btnPlay_clicked();

    void on_listWidgets_itemDoubleClicked(QListWidgetItem *item);

    void action_triggered();

    void get_time_slots();

    void on_btnForward_clicked();

    void on_btnBackward_clicked();

    void on_listWidgets_itemClicked(QListWidgetItem *item);

    void on_checkBoxMutex_toggled(bool checked);

    void on_sldProgress_actionTriggered(int action);

    void on_btnPrevious_clicked();

    void on_btnNext_clicked();

 public slots:
    //network communication
    void handleCtrl2VideoSignal(QLinkedList<CMessage*> message);

private:
    Ui::CVideoPlay *ui;

    QProcess    *pmProcess;
    QTimer      *pmTimer;
    QString     mPlayer;
    QString     mMediaFile;

    int         mCurrTime;
    int         mTotalTime;
    int         mVolume;

    bool        mPlayState;
    bool        mMuteFlag;

protected:
    QTime int_to_time(int second);
    void curvalue(int val);

};

#endif // CVIDEOPLAY_H
