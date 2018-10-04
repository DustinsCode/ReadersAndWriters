#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void sigHandler(int sigNum);

key_t key;
int shmId;
char *shmPtr;

struct Token {
    //if 0, message has not been read
    //if 1, message has been read and memory is ready to be written to
    int flag;
    char message[1024];
};

struct Token *tok;

int main(){

    tok = malloc(sizeof(struct Token));
    tok -> flag = 1;
    //Signal handler for nice shutdown
    signal(SIGINT, sigHandler);

    if((key = ftok("key.txt", 'R'))== -1){          //not sure what the proj_id 'R' does, got it from stack overflow
        perror("error creating key");
        exit(1);
    }

    if((shmId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
        perror("Error setting up shared memory\n");
        exit(1);
    }


    if((tok = shmat (shmId, 0, 0)) == (void*) -1){
        printf("Errno: %d\n", errno);
        perror("Can't attach\n");
        exit(1);
    }

    printf("ID: %d\n", shmId);
    while(1){

        if(tok -> flag == 0){
            // Print message from shared memory 
            printf("%s\n", tok -> message);
            //TODO: Put input into shmPtr and set flag to 0
            tok -> flag = 1;

        }
    }
   printf("memory pointer: %lu \n", (unsigned long) shmPtr);
       return 0;
}


void sigHandler(int sigNum){
    printf(" recieved.  Shutting down...\n");
    //TODO: detach the shared memory to prevent memory leakage.
     if(shmdt(tok) < 0 ){
        perror("error detaching\n");
        exit(1);
    }
    if(shmctl (shmId, IPC_RMID, 0) < 0) {
        perror("can't deallocate\n");
        exit(1);
    }


    exit(0);
}
