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

    maxlen = sizeof(mybuf.ap);
    len = sizeof(mybuf.ap);

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    int pid = getpid();
    printf("Client start %d\nYou number: ", pid);
    scanf("%f", &num);

    mybuf.mtype = 1;
    mybuf.ap.id = pid;
    mybuf.ap.f = num;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, pid, 0)) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }

    printf("I get %f from %d\n", mybuf.ap.f, mybuf.ap.id);

    return 0;
}
