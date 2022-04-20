#define _GNU_SOURCE

#include <utils.h>
#include <thread.h>

int main(int argc, char **argv){
    pid_t cpid;
    int wstatus;
    int sig;
    
    srand(time(NULL));
    
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (cpid == 0) {
        execl("./catcher","./catcher",NULL);
    } else {            
        FILE *out = 0;
        /* Parent  */
        int logfd;
        if((logfd = open("send.log",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | O_CLOEXEC)) == -1){
            perror("failed to open log file");
            exit(EXIT_FAILURE);
        }
        
        if(!(out = fdopen(logfd, "w"))){
            perror("parent:failed to open log stream");
            exit(EXIT_FAILURE);
        }
        
        sig_atomic_t killcounter = 0;
        
        for(;;){
            
            if(msleep((rand() % 10)+1) == -1){
                perror("msleep");
                exit(EXIT_FAILURE);
            }
            
            sig = rand() % 2 ? SIGUSR1 : SIGUSR2;
            
            if(kill(cpid,sig) == -1){
                perror("kill");
                exit(EXIT_FAILURE);
            }
            
            fprintf(out,"%d send signal microseconds: %"PRId64" %s\n",killcounter,micros(),sig == SIGUSR1 ? "SIGUSR1" : "SIGUSR2");
            fflush(out);
            ++killcounter;
            
            if(cpid == waitpid(cpid,&wstatus,WNOHANG)){              // Wait for child 
                fclose(out);
                exit(EXIT_SUCCESS);
            }
        }
    }
}
