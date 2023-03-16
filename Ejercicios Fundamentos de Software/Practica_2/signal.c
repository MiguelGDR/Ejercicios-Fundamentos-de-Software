#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void m(int num_signal)
{
    printf("LLego la señal.\n");

    signal(SIGINT, m);
}

int main()
{
    if(signal(SIGINT, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }
    
    while(1)
    {
        pause();
    }
    return 0;
}