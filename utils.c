#include <utils.h>

int64_t micros(){
    
    struct timespec tms;
    
    /* The C11 way */
    /* if (! timespec_get(&tms, TIME_UTC)) { */
    
    /* POSIX.1-2008 way */
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }

    return micros;
}

int msleep(long msec){
    struct timespec ts;
    int res;
    
    if (msec < 0){
        errno = EINVAL;
        return -1;
    }
    
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    
    return res;
}
