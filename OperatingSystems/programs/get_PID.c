#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid, ppid, chpid;
    int a = 0;

    chpid = fork();

	if (chpid < 0) {}
	else if (chpid == 0) {
		print("Im a child");
	}
	else (chpid > 0) {
		print("Im a parent");
	}

 	print("my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

    return 0;

}
