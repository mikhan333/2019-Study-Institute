#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int msqid;
    char pathname[]="server.c";
    key_t  key;
    int len, maxlen;

    struct mymsgbuf
    {
        long mtype;
        struct a {
           float f;
           int id;
        } ap;
    } mybuf;

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    printf("Server start\n");

    maxlen = sizeof(mybuf.ap);
    len = sizeof(mybuf.ap);
    while(1){

        if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0)) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        if (mybuf.mtype == 1){
            printf("I get from %d - ", mybuf.ap.id);
            printf("Float %f\n", mybuf.ap.f);
            mybuf.mtype = mybuf.ap.id;
            mybuf.ap.id = 1;
            mybuf.ap.f = mybuf.ap.f * mybuf.ap.f;

            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
        }
        else {
            printf("Server end\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            break;
        }

    }


    return 0;
}