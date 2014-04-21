#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class CConnection : public QObject
{
    Q_OBJECT
public:
    bool        mStatus;//-1 indicate the socket is invalid; else is the global id of the connection
    QTcpSocket* pmTcpSocket;

public:
    CConnection()
    {
        mStatus = false;
        pmTcpSocket = NULL;
    }

    ~CConnection()
    {
        if (pmTcpSocket!=NULL)
        {
            delete pmTcpSocket;
        }
    }

public:

    void InitializeConnection(QTcpSocket* socket)
    {
        pmTcpSocket = socket;
        pmTcpSocket->setParent(this);
    }

    void DeleteConnection()
    {
        mStatus = false;
        pmTcpSocket->disconnect();
    //  delete m_pTcpSocket; 若delete会报错， ？
        pmTcpSocket = NULL;
    }
};

#endif // CCONNECTION_H

