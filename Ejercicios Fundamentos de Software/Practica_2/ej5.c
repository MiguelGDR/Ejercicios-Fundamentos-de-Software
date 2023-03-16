#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

#define T 256

void man_alarm(int signo)
{
    signal(SIGALRM, man_alarm);
}

int main()
{
    int rd, wr;
    char buffer[T];

    signal(SIGALRM, man_alarm);

    alarm(3);

    if((rd = read(0, buffer, T)) < 0)
    {
        perror("read");
        exit(1);
    }

    if((wr = write(1, buffer, rd)) < 0)
    {
        perror("write");
        exit(1);
    }

    return 0;
}