#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
char *script[]= {"osascript","-e","'display alert \"Hello World!\" message \"longer text can be added in the message.\"\'",NULL};
//char *script[]= {"ls",NULL};
int
main(int argc,char ** argv){
    if(argc>2){
        perror("picard-notice [minute]");
    }
    int timewait,minu;
    if(argc == 1){
        minu = 30;
    }
    else{
        minu = atoi(argv[1]);
    }
    timewait = minu*60*1000;
    if(timewait<0){
        perror("second overflow");
        exit(0);
    }
    int set_ticker(int);
    void on_alarm(int);
    
    set_ticker(timewait);
    signal(SIGALRM,on_alarm);
    while(1){
        //printf("waiting...\n");
        fflush(stdout);
        pause();
    }
}
void on_alarm(int signum){
    int fortret = fork();
    if(fortret == 0){
        //printf("%s %s %s\n",script[0],script[1],script[2]);
        int ret = execlp("picard-prtnotice","picard-prtnotice",(char *)0);
        //int ret = execlp("ls",script[0],(char *)0);
        printf("%d\n",ret);   
        exit(0);
    }
    int result;
    wait(&result);
    exit(0);
}
int set_ticker(int n_msecs){
    struct itimerval new_timeset;
    long n_sec,n_usecs;
    n_sec = n_msecs/1000;
    n_usecs = (n_msecs%1000)*1000;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;
    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    return setitimer(ITIMER_REAL,&new_timeset,NULL);
}