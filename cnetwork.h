#ifndef CNETWORK_H
#define CNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QMutex>
#include "constvaule.h"

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
    void neworkCtrl2VideoSignal();
    
public slots:
    void handleNewConnection();

    void handleDisconnected();

    void handNewMessage();

    void handleSocketError();

private:
    QTcpServer  *pmTcpServer;
    CConnection     *pmConnection;
    QMutex      mConnectionMutex;
    int         mTcpConnectionSessionNum;


};

#endif // CNETWORK_H
