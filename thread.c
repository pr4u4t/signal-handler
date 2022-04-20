#include <thread.h>

volatile sig_atomic_t sigcounter = 0;

void* signal_handler_thread(void* data){
    //this variable preserves value across calls
    ThreadData *thd = (ThreadData*) data;
    siginfo_t info;
    sigset_t set;
    sig_atomic_t sig;
    uint64_t tmp = 1;
    FILE *out = 0;
    
    sigemptyset(&set);
    sigaddset(&set,thd->_id == 1 ? SIGUSR1 : SIGUSR2);
    
    if(sigprocmask(SIG_BLOCK,&set,0) == -1){
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    if(!(out = fdopen(thd->_fd, "w"))){
        perror("failed to open log stream");
        exit(EXIT_FAILURE);
    }
    
    for(;;){
        if((sig = sigwaitinfo(&set, &info)) > 0){
            
            fprintf(out,"%d thread #%d catched %"PRId64" signal %s\n",sigcounter,thd->_id,micros(),sig == SIGUSR1 ? "SIGUSR1" : "SIGUSR2");
            ++sigcounter;
            fflush(out);
            if(sigcounter % 20 == 0){
                write(thd->_ev,&tmp,sizeof(uint64_t));
            }
        }
    }
    
    fclose(out);
    return 0;
}
