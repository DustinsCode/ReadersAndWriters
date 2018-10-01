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
    int sendFlag = 0;                   //0 if not received 1 if received

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

    while(1){
        //TODO: get user input
        
        //TODO: Put input into shmPtr and set flag to 0
        
        //TODO: wait for flag to equal 1
    }

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
    /* Josh added some helpful information here to help with closing the shared memory nicely 
        the variables used can be figured out based off parameters :)
    if (shmctl(shared_mem_id, IPC_STAT, &shm_struct) < 0) {
        perror("Error filling out shm_struct");
    }

    if (shm_struct.shm_nattch == 0 && shmctl(shared_mem_id, IPC_RMID, &shm_struct) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
    exit(0);
}
