#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

pid_t pidp1, pidp2;
int j = 0;
int flag;

void my_handler(int nsig) {
    if (nsig == SIGUSR1) {
        printf("get(number=%d)\n", j++);
    }
    flag = 0;
}

int main(void) {
    pidp1 = getpid();
    printf("pid_p1 = %d\nenter value: ", (int) pidp1);

    int value;
    scanf("%d", &value);
    printf("value = %d\nenter pid_p2: ", value);
    scanf("%d", &pidp2);
    printf("pid_p2 = %d\n", (int) pidp2);

    int error;
    error = kill(pidp2, 0);
    if (error == 0) {
        printf("p2 work\n");
    } else {
        printf("p2 doesnt work\n");
    }

    (void) signal(SIGUSR1, my_handler);
    (void) signal(SIGUSR2, my_handler);

    int i;
    int p;
    for (i = 31; i >= 0; i--) {
        flag = 1;
        p = (value >> i) & 0x01;
        printf("send(%d)\n", p);
        if (p == 1) {
            kill(pidp2, SIGUSR1);
        } else {
            kill(pidp2, SIGUSR2);
        }
        while (flag != 0);
    }

    return 0;

}