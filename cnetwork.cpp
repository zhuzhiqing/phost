#include "cnetwork.h"
#include <QTcpServer>
#include <QDebug>

cnetwork::cnetwork(QObject *parent) :
    QObject(parent)
{
    mTcpConnectionSessionNum = 0;
    pmConnection = new CConnection[MAX_TCP_CONNECTION];
}

void cnetwork::showMessage(QString message)
{
#ifdef  DEBUG
    qDebug()<<message;
#endif
}

bool cnetwork::startServer()
{
    pmTcpServer = new QTcpServer(this);
    if(!pmTcpServer->listen(QHostAddress::Any,SERVER_PORT))
    {
        showMessage("create server error!");
        return false;
    }
    connect(pmTcpServer,SIGNAL(newConnection()),this,SLOT(handleNewConnection()));
    return true;
}

void cnetwork::handleNewConnection()
{
    int i = 0;
    for(i = 0; i < MAX_TCP_CONNECTION; i++)
    {
        if (!pmConnection[i].mStatus)
        {
            //add new connection
            mConnectionMutex.lock();
            pmConnection[i].InitializeConnection(pmTcpServer->nextPendingConnection());
            pmConnection[i].mStatus = true;
            mConnectionMutex.unlock();
            mTcpConnectionSessionNum++;

            //set listener
            connect(pmConnection[i].pmTcpSocket,SIGNAL(readyRead()),
                    this,SLOT(handNewMessage()));
            connect(pmConnection[i].pmTcpSocket, SIGNAL(disconnected()),
                    this, SLOT(handleDisconnected()));
            connect(pmConnection[i].pmTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(handleSocketError()));
            showMessage("a new connection set up sucessfully.");
            qDebug()<<"mTcpConnectionSessionNum:"<<mTcpConnectionSessionNum;

            if(mTcpConnectionSessionNum == MAX_TCP_CONNECTION)
            {
              //  pmTcpServer->pauseAccepting();
                pmTcpServer->close();
                showMessage("Maxium connection reached, new connection request will be refused.");
            }
            return;
        }
    }


}

void cnetwork::handNewMessage()
{
    //get the connection which had received message.
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
 //   CConnection* connection = qobject_cast<CConnection*>(socket->parent());

    //read
    QByteArray rcvMessage = socket->readAll();
    qDebug()<<socket<<rcvMessage;
}

void cnetwork::handleDisconnected()
{
    //get the connection which disconnect from server.
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    CConnection* connection = qobject_cast<CConnection*>(socket->parent());

    //delete the connection
    mConnectionMutex.lock();
    connection->DeleteConnection();
    connection->mStatus = false;
    mConnectionMutex.unlock();
    mTcpConnectionSessionNum--;

    //if the connection is reduced from maximum, reopen the server, listen
    if(mTcpConnectionSessionNum == MAX_TCP_CONNECTION-1)
    {
      //  pmTcpServer->resumeAccepting();
        pmTcpServer->listen(QHostAddress::Any,SERVER_PORT);
    }
    showMessage("server disconnected");
    qDebug()<<"mTcpConnectionSessionNum:"<<mTcpConnectionSessionNum;
}

void cnetwork::handleSocketError()
{
    qDebug()<<"socket error!";
    /*not realize !*/
}

void cnetwork::deleteConnection(int connectID)
{
    mConnectionMutex.lock();
    pmConnection[connectID].DeleteConnection();
    pmConnection[connectID].mStatus = false;
    mConnectionMutex.unlock();
}
