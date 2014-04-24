#ifndef CNETWORK_H
#define CNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QMutex>
#include "constvaule.h"
#include "utils.h"

class CConnection : public QObject
{
    Q_OBJECT
public:
    bool        mStatus;//-1 indicate the socket is invalid; else is the global id of the connection
    QTcpSocket* pmTcpSocket;
    QLinkedList<CMessage*>  rcvMsgList;      //msg queue

public:
    CConnection()
    {
        mStatus = false;
        pmTcpSocket = NULL;
      //  rcvMsgList = new QLinkedList<CMessage*>;
    }

    ~CConnection()
    {
        if (pmTcpSocket!=NULL)
        {
            delete pmTcpSocket;
        }

//        if (rcvMsgList != NULL)
//        {
//            delete rcvMsgList;
//        }
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

class CNetwork : public QObject
{
    Q_OBJECT
public:
    explicit CNetwork(QObject *parent = 0);

    bool startServer();

    void error(int errno);

    void showMessage(QString message);

    void deleteConnection(int connectID);
    
signals:
    void neworkCtrl2VideoSignal(QLinkedList<CMessage*> message);
    
public slots:
    void handleNewConnection();

    void handleDisconnected();

    void handNewMessage();

    void handleSocketError();

private:
    QTcpServer  *pmTcpServer;
    CConnection *pmConnection;
    QMutex      mConnectionMutex;
    int         mTcpConnectionSessionNum;

private:
    void readSessionMsg(CConnection * session);

};

#endif // CNETWORK_H
