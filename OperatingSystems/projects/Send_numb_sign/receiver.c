#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int i = 0;
pid_t pidp1, pidp2;
int value = 0;
int p = 0;

void my_handler(int nsig) {
    if (nsig == SIGUSR1) {
        p = 1;
    }
    if (nsig == SIGUSR2) {
        p = 0;
    }
    printf("%d", p);
    value = (p | value);
    kill(pidp1, SIGUSR1);
    if (i++ == 31) {
        printf("\nresult: %d\n", (int) value);
        exit(0);
    }
    value = value << 1;
}

int main(void) {
    pidp2 = getpid();
    printf("pid_p2 = %d\n", (int) pidp2);

    int value;
    printf("enter pid_p1: ");
    scanf("%d", &pidp1);
    printf("pid_p1 = %d\n", (int) pidp1);

    (void) signal(SIGUSR1, my_handler);
    (void) signal(SIGUSR2, my_handler);

    while (1);

    return 0;

}