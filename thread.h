#ifndef _THREAD_H_
#define _THREAD_H_

#include <utils.h>

void* signal_handler_thread(void* data);

typedef struct _ThreadData ThreadData;
struct _ThreadData{
    int _fd;
    int _id;
    int _ev;
};

#endif
