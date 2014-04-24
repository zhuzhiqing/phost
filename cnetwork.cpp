#include "cnetwork.h"
#include "utils.h"
#include <QTcpServer>
#include <QDebug>

CNetwork::CNetwork(QObject *parent) :
    QObject(parent)
{
    mTcpConnectionSessionNum = 0;
    pmConnection = new CConnection[MAX_TCP_CONNECTION];
}

void CNetwork::showMessage(QString message)
{
#ifdef  DEBUG
    qDebug()<<message;
#endif
}

bool CNetwork::startServer()
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

void CNetwork::handleNewConnection()
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

void CNetwork::handNewMessage()
{
    //get the connection which had received message.
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    CConnection* connection = qobject_cast<CConnection*>(socket->parent());

    readSessionMsg(connection);

    //判断是否需要发送消息
    if (connection->rcvMsgList.last()->filledLength == connection->rcvMsgList.last()->length)
    {
        switch(connection->rcvMsgList.last()->type)
        {
        case MEDIA_CTRL:
            emit neworkCtrl2VideoSignal((connection->rcvMsgList));
            break;
        default:
            break;

        }
    }
}

void CNetwork::readSessionMsg(CConnection *session)
{
    int totalLen = session->pmTcpSocket->bytesAvailable();
    QByteArray rcvMessage;
    int readLen, bufferLeftLen;
    CMessage *message;

    //若一次受到数据小于9，则跳过读取，留着下次一起读取
    if (totalLen > 9)
    {
        rcvMessage = session->pmTcpSocket->read(10);
        totalLen -= 10;
    }
    else
        return;

    if (rcvMessage.count("NMSG"))       //new message
    {
        //第一次开始传输或上次传输完成，则开始新的消息
        if (session->rcvMsgList.isEmpty() ||
                (session->rcvMsgList.last()->filledLength == session->rcvMsgList.last()->length))
        {
            //创建新的message 对象,填充相关信息

            message = new CMessage;
            message->type = rcvMessage[4];
            message->code = rcvMessage[5];
            message->length = rcvMessage[6]
                    | (rcvMessage[7]<<8)
                    | (rcvMessage[8]<<16)
                    | (rcvMessage[9]<<24);
            message->content = new char[message->length];
            session->rcvMsgList.append(message);

            //填充buffer
            while (totalLen != 0 )
            {
                bufferLeftLen = message->length - message->filledLength;
                readLen = session->pmTcpSocket->read(message->content + message->filledLength
                                           , bufferLeftLen );
                message->filledLength += readLen;
                totalLen -= readLen;

                //判断是否已经填充满了
                if ( bufferLeftLen == 0)
                    break;
            }

            /*如果buffer已经填充完毕，但是套接字缓冲区未读取完毕，则递归调用本函数
            4种情形:a.套接字缓冲区无剩余，buffer未填充满         函数返回
                   b.套接字缓冲区有剩余，buffer未填充满         不可能发生
                   c.套接字缓冲区有剩余，buffer填充满           递归调用
                   d.套接字缓冲区无剩余，buffer填充满           函数返回
            */
            if( (totalLen != 0) && (bufferLeftLen == 0) )
                readSessionMsg(session);
            else
                return;
        }
    }
    else    //complete last message
    {
        //消息成员显示:本次数据第一次开始传输或上次传输完成
        //表示出错，读取并丢弃本次数据
        if (session->rcvMsgList.isEmpty() ||
                (session->rcvMsgList.last()->filledLength == session->rcvMsgList.last()->length))
        {
            rcvMessage = session->pmTcpSocket->readAll();   //读取并丢弃本次数据
        }
        else
        {
            message = session->rcvMsgList.last();

            //此处存在bug,若当前消息未填充的字节数小于9，则数组越界
            //保存函数开头读取的9个字节
            memcpy(message->content + message->filledLength,
                   rcvMessage,10);
            message->filledLength += 10;


            //填充buffer
            while (totalLen != 0 )
            {
                bufferLeftLen = message->length - message->filledLength;
                readLen = session->pmTcpSocket->read(message->content + message->filledLength
                                           , bufferLeftLen );
                message->filledLength += readLen;
                totalLen -= readLen;

                //判断是否已经填充满了
                if ( bufferLeftLen == 0)
                    break;
            }

            if( (totalLen != 0) && (bufferLeftLen == 0) )
                readSessionMsg(session);
            else
                return;
        }
    }
}


void CNetwork::handleDisconnected()
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

void CNetwork::handleSocketError()
{
    qDebug()<<"socket error!";
    /*not realize !*/
}

void CNetwork::deleteConnection(int connectID)
{
    mConnectionMutex.lock();
    pmConnection[connectID].DeleteConnection();
    pmConnection[connectID].mStatus = false;
    mConnectionMutex.unlock();
}

