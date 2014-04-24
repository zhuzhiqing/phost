#ifndef UTILS_H
#define UTILS_H
#include <QObject>
#include <QMutex>

//enum MsgFrameStatus
//{
//    /****************define communicate state*************************/
//    MSG_FRAME_EMPTY = 1,    //haven't receive data
//    MSG_FRAME_FILLING,      //receive data, but not complete
//    MSG_FRAME_READY,        //receive data, and complete,but not used
//    MSG_FRAME_IN_USE
//};


class CMessage : public QObject
{
    Q_OBJECT
public:
    char     type;
    char     code;
    int     length;     //content length
    int     filledLength; //the count that has filled
    char    *content;
    int     status;
    QMutex  mutex;
public:
    CMessage();

    ~CMessage();

    void InitializeMsg(char _type,char _code,int _length,char *_content);

    void DeleteMsg();

};




#endif // UTILS_H
