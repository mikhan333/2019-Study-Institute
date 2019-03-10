#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    int   semid;
    char pathname[]="cater.c";
    key_t key;
    struct sembuf mybuf;

    key = ftok(pathname, 0);

    if((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
    }

    mybuf.sem_flg = 0;

    int N = 10, m = 50, k = 5;

    int i = 0;
    for (i; i < k; i++)
    {
        //Init sems: start

        mybuf.sem_op  = 1;
        mybuf.sem_num = 1;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        mybuf.sem_op  = N;
        mybuf.sem_num = 2;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        mybuf.sem_op  = N;
        mybuf.sem_num = 0;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        //Init sems: end

        printf("Катер: Я готов принимать пассажиров №%i\n", i);

        mybuf.sem_op  = 0;
        mybuf.sem_num = 0;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        printf("Катер: Идет прогулка\n");

        mybuf.sem_op  = -1;
        mybuf.sem_num = 1;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        printf("Катер: Началась высадка\n");

        mybuf.sem_op  = 0;
        mybuf.sem_num = 2;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        printf("Катер: Закончилась высадка\n\n");

    }


    return 0;
}
