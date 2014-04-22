#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "cvideoplay.h"
#include "cnetwork.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    CNetwork *mpNetwork;
    
private slots:

    void on_btnVideoMode_clicked();

    void on_showConnect_clicked();

signals:
    void ctrlMsg2Video();

public slots:
   //network communication
//   void videoCtrolFromNet();

private:
    Ui::Widget *ui;
    CVideoPlay *cVideoPlay;
};

#endif // WIDGET_H
