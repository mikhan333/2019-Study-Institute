#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    int   semid;
    int   result;
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
    for(i; i < m; i++) {
        result = fork();
        if (result < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        } else if (result > 0) {
            /* Parent process */

        } else {
            /* Child process */
            if (i % 10 == 0) {
                printf("Я есть %i!\n", i);
            }

            mybuf.sem_op  = -1;
            mybuf.sem_num = 0;

            if(semop(semid, &mybuf, 1) < 0){
                printf("Can\'t wait for condition\n");
                exit(-1);
            }

            if (i % 10 == 0) {
                printf("Я сел %i!\n", i);
            }

            mybuf.sem_op  = 0;
            mybuf.sem_num = 1;

            if(semop(semid, &mybuf, 1) < 0){
                printf("Can\'t wait for condition\n");
                exit(-1);
            }

            if (i % 10 == 0) {
                printf("Я в пути %i!\n", i);
            }

            mybuf.sem_op  = -1;
            mybuf.sem_num = 2;

            if(semop(semid, &mybuf, 1) < 0){
                printf("Can\'t wait for condition\n");
                exit(-1);
            }
            if (i % 10 == 0) {
                printf("Я вышел %i!\n", i);
            }

            break;
        }
    }
    return 0;
}
