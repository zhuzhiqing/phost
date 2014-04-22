#ifndef CONSTVAULE_H
#define CONSTVAULE_H

#define PATH "/home/media/"

#define WAIT_FOR_FINISH_TIME 1000

//define ops_type
#define MEDIA_CTRL      0X001

//define ops_code
//media ctrl ops code define
#define MEDIA_PLAY      0X001
#define MEDIA_PAUSE     0X002
#define MEDIA_FORWARD   0X003
#define MEDIA_BACKWARD  0X004
#define MEDIA_NEXT      0X005
#define MEDIA_PREVIOUS  0X006
#define MEDIA_VOLUME_MUTEX  0X007
#define MEDIA_VOLUME_QUERY  0X008
#define MEDIA_VOLUME_SET    0X009

/**define max tcp connection num  */
#define MAX_TCP_CONNECTION  5
#define SERVER_PORT         1234

#endif // CONSTVAULE_H
