/**
 * Program that writes user entered text
 * into shared memory.
 *
 * @authors Dustin Thurston & Dylan Kernohan
 *
 * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>

void sigHandler(int sigNum);

key_t key;
int shmId;
char *shmPtr;
int sendFlag = 0;                   //0 if not received 1 if received


int main(){
        //Signal handler for nice shutdown
    signal(SIGINT, sigHandler);

    if((key = ftok("key.txt", 'R'))== -1){          //not sure what the proj_id 'R' does, got it from stack overflow
        perror("error creating key");
        exit(1);
    }

    if((shmId = shmget(key, 1024, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
        perror("Error setting up shared memory\n");
        exit(1);
    }
    printf("shmId: %u", shmId);

    if((shmPtr = shmat (shmId, 0,0)) == (void*) -1){
        perror("Can't attach\n");
        exit(1);
    }
 
    while(1){
        //TODO: get user input
        char buff[1024];
        printf(">");
        fgets(buff, 1024, stdin);
        strcpy(buff,shmPtr);
        printf("%s\n", buff);
        //TODO: Put input into shmPtr and set flag to 0
        
        //TODO: wait for flag to equal 1
    }
   printf("memory pointer: %lu \n", (unsigned long) shmPtr);
       return 0;
}


void sigHandler(int sigNum){
    printf(" recieved.  Shutting down...\n");
    //TODO: detach the shared memory to prevent memory leakage.
     if(shmdt(shmPtr) < 0 ){
        perror("error detaching\n");
        exit(1);
    }
    if(shmctl (shmId, IPC_RMID, 0) < 0) {
        perror("can't deallocate\n");
        exit(1);
    }


    exit(0);
}
