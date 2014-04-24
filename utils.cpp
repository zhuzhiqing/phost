#include "utils.h"
#include "QLinkedList"

CMessage::CMessage()
{
    content = NULL;
    filledLength = 0;
}

CMessage::~CMessage()
{
    if (content!=NULL)
    {
        delete content;
    }
}


void CMessage::InitializeMsg(char _type, char _code, int _length, char *_content)
{
    type = _type;
    code = _code;
    length = _length;
    content = _content;
}


void CMessage::DeleteMsg()
{
    delete content;
    content = NULL;
 //   status = MSG_FRAME_EMPTY;
}
