#include "widget.h"
#include "utils.h"
#include <QApplication>

//static CMessage gMessageList[][MSG_FRAME_LIST_LENGTH];

//static CMessage*


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

    return a.exec();
}
