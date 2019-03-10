#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[]="server.c";
    key_t  key;
    int len, maxlen;
    float num;

    struct mymsgbuf
    {
        long mtype;
        struct a {
            float f;
            int id;
        } ap;
    } mybuf;

    len = sizeof(mybuf.ap);

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    printf("Killer start\n");

    mybuf.mtype = 2;
    mybuf.ap.id = 0;
    mybuf.ap.f = 0;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Killer end\n");

    return 0;
}
