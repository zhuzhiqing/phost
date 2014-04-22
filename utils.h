#ifndef UTILS_H
#define UTILS_H
#include <QObject>
class CMessage : public QObject
{
    Q_OBJECT
public:
    int type;
    int code;
    int length;     //content length
    QByteArrayData *content;

public:
    CMessage()
    {
        content = NULL;
    }

    ~CMessage()
    {
        if (content!=NULL)
        {
            delete content;
        }
    }

public:

    void InitializeConnection(int _type,int _code,int _length,QByteArrayData *_content)
    {
        type = _type;
        code = _code;
        length = _length;
        content = _content;
        content->setParent(this);
    }

    void DeleteConnection()
    {
        delete content;
        content = NULL;
    }
};


#endif // UTILS_H
