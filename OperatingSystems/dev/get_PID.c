#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

int main()
{
    pid_t pid, ppid, chpid;
    int a = 0;
	pthread_t id = pthread_self();
	printf("%u\n", pthread_self);
//    chpid = fork();
//
//	if (chpid < 0) {}
//	else if (chpid == 0) {
//		pthread_t id = pthread_self();
//		printf("Im a child %u\n", pthread_self);
//	}
//	else (chpid > 0) {
//		print("Im a parent %u\n", pthread_self);
//	}
//
// 	printf("my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

    return 0;

}
