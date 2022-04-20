#include <thread.h>

int main(int argc, char **argv){
	/* Child */
        int efd;
        int pipefd[2];
        int logfd = 0;
        char buffer[4096];
        int size;
        
        sigset_t set;
        sigemptyset(&set);
        
        sigaddset(&set,SIGUSR1);
        sigaddset(&set,SIGUSR2);
        
        if(sigprocmask(SIG_BLOCK,&set,0) == -1){
            perror("sigprocmask");
            exit(EXIT_FAILURE);
        }
        
        if (pipe2(pipefd,O_CLOEXEC) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        
        if((efd = eventfd(0, EFD_CLOEXEC)) == -1){
            perror("eventfd");
            exit(EXIT_FAILURE);
        }
        
        //open log file descriptor
        if((logfd = open("receive.log",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | O_CLOEXEC)) == -1){
            perror("failed to open log file");
            exit(EXIT_FAILURE);
        }
        
        pthread_t thread[2];
        ThreadData thid[] = {{pipefd[1],1,efd},{pipefd[1],2,efd}};
        
        pthread_create(&thread[0],0,signal_handler_thread,&thid[0]);
        pthread_create(&thread[1],0,signal_handler_thread,&thid[1]);
        
        for(;;){
            uint64_t tmp;
            if(read(efd,&tmp,sizeof(uint64_t)) != sizeof(uint64_t)){
                perror("read eventfd");
                exit(EXIT_FAILURE);
            }
            

            while((size = read(pipefd[0],buffer,sizeof(buffer))) > 0){
                write(logfd,buffer,size);
                fsync(logfd);
            
            }
        }

        close(pipefd[0]);
        exit(EXIT_SUCCESS);
}
