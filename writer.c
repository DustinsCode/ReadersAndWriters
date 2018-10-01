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


void sigHandler(int sigNum);
void shutdown(char * shmPtr, int shmId);

key_t key;

int main(){
    int shmId;
    char *shmPtr;
    int sendFlag;                   //0 if not received 1 if received

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
    printf("memory pointer: %lu \n", (unsigned long) shmPtr);
    if(shmdt(shmPtr) < 0 ){
        perror("error detaching\n");
        exit(1);
    }
    if(shmctl (shmId, IPC_RMID, 0) < 0) {
        perror("can't deallocate\n");
        exit(1);
    }

    return 0;
}

void shutdown(char * shmPtr, int shmId){

}

void sigHandler(int sigNum){
    printf("^C recieved.  Shutting down...");
    //TODO: detach the shared memory to prevent memory leakage.
    
    exit(0);
}
