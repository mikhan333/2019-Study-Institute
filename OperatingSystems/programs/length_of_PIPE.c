#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
   int fd[2], result;

   size_t size1, size2;

   if(pipe(fd) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   result = fork();

   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {

      if(close(fd[1]) < 0){
         exit(-1);
      }


      sleep(2);

      char prr[100000];
      int m2 = 1;

      size2 = read(fd[0], prr, 100000);

      if(size2 < 0){
         printf("Can\'t read string from pipe11\n");
         exit(-1);
      }

      printf("%d - result\n", size2 - 2); // Вычел "2", так как успел выполниться write,
                                          // который ждал


      if(close(fd[0]) < 0){
         exit(-1);
      }

   } else {

      if(close(fd[0]) < 0){
         exit(-1);
      }


      time_t start, end;
      while(1)
      {
         start = time(NULL); // Проверяю задержку по времени

         size1 = write(fd[1], "a", 2);
         if(size1 != 2){
            printf("Can\'t write all string to pipe10\n");
            exit(-1);
         }

         end = time(NULL);
         if (difftime(end,start) >= 1) { break; }
            // Можно без этой проверки, но тогда успеет
            // несколько write-ов выполнитсья за один read в соседнем процессе,
            // что приведет к немного завышенному результату 65536 ~ 65590


      }


      if(close(fd[1]) < 0){
         exit(-1);
      }
   }

   return 0;
}

