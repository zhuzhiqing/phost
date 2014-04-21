#ifndef CNETWORK_H
#define CNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QMutex>
#include "constvaule.h"
#include "cconnection.h"

class cnetwork : public QObject
{
    Q_OBJECT
public:
    explicit cnetwork(QObject *parent = 0);

    bool startServer();

    void error(int errno);

    void showMessage(QString message);

    void deleteConnection(int connectID);
    
signals:
    
public slots:
    void handleNewConnection();

    void handleDisconnected();

    void handNewMessage();

    void handleSocketError();

private:
    QTcpServer  *pmTcpServer;
   // CConnection     mConnection[MAX_TCP_CONNECTION];
    CConnection     *pmConnection;
    QMutex      mConnectionMutex;
    int mTcpConnectionSessionNum;


};

#endif // CNETWORK_H
