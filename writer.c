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

struct Token {
    //if 0, message has not been read
    //if 1, message has been read and memory is ready to be written to
    int flag;
    char message[1024];
};

int main(){
    
    //struct Token token;
    struct Token *tok;
    tok -> flag = 1;
    //Signal handler for nice shutdown
    signal(SIGINT, sigHandler);

    if((key = ftok("key.txt", 'R'))== -1){          //not sure what the proj_id 'R' does, got it from stack overflow
        perror("error creating key");
        exit(1);
    }

    if((shmId = shmget(key, sizeof(tok), IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
        perror("Error setting up shared memory\n");
        exit(1);
    }

    while(1){
        if(tok -> flag == 1){
            // get user input
            printf(">");
            fgets(tok -> message, 1024, stdin);
            printf("%s\n", tok -> message);
            //TODO: Put input into shmPtr and set flag to 0
            tok -> flag= 0;
            if((tok = shmat (shmId, NULL,0)) == (void*) -1){
                perror("Can't attach\n");
                exit(1);
            }
        }
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
