#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd[2], result;

   size_t size;
   char  resstring[14];

   if(pipe(fd) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }


   int N = 5;
   int  semid;
   char pathname[]="program.c";
   key_t key;
   struct sembuf mybuf;

   key = ftok(pathname, 0);

   if((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
   }

   mybuf.sem_num = 0;
   mybuf.sem_op  = 1;
   mybuf.sem_flg = 0;

   if(semop(semid, &mybuf, 1) < 0){
      printf("Break1\n");
      exit(-1);
   }

   mybuf.sem_num = 1;
   mybuf.sem_op  = 1;
   mybuf.sem_flg = 0;

   if(semop(semid, &mybuf, 1) < 0){
      printf("Break2\n");
      exit(-1);
   }

   result = fork();

   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {
      /* Parent process */
      while (N >= 0)
      {
         //D(S1,1)
         mybuf.sem_num = 0;
         mybuf.sem_op  = -1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         size = write(fd[1], "Parent", 7);

         if(size != 7){
           printf("Can\'t write all string to pipe\n");
           exit(-1);
         }

         //A(S2,1)
         mybuf.sem_num = 1;
         mybuf.sem_op  = 1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         //D(S1,1)
         mybuf.sem_num = 0;
         mybuf.sem_op  = -1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         size = read(fd[0], resstring, 7);

         if(size < 0){
            printf("Can\'t read string from pipe\n");
            exit(-1);
         }

         printf("%s\n", resstring);

         //A(S2,1)
         mybuf.sem_num = 1;
         mybuf.sem_op  = 1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         N--;

      }

   } else {
      /* Child process */
      while (N >= 0)
      {
         //D(S2,1)
         mybuf.sem_num = 1;
         mybuf.sem_op  = -1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         size = read(fd[0], resstring, 7);

         if(size < 0){
            printf("Can\'t read string from pipe\n");
            exit(-1);
         }

         printf("%s\n", resstring);


         //A(S1,1)
         mybuf.sem_num = 0;
         mybuf.sem_op  = 1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         //D(S2,1)
         mybuf.sem_num = 1;
         mybuf.sem_op  = -1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         size = write(fd[1], "Child!", 7);

         if(size != 7){
           printf("Can\'t write all string to pipe\n");
           exit(-1);
         }

         //A(S1,1)
         mybuf.sem_num = 0;
         mybuf.sem_op  = 1;

         if(semop(semid, &mybuf, 1) < 0){
            printf("Break\n");
            exit(-1);
         }

         N--;
      }

   }


   if(close(fd[0]) < 0){
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
   }

   if(close(fd[1]) < 0){
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
   }

   return 0;
}
