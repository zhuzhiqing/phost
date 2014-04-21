#include "widget.h"
#include "cnetwork.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置中文显示放乱码

 /*
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
*/

    Widget w;
    w.show();
    cnetwork network;
    network.startServer();
    
    return a.exec();
}
