#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum)
{
    printf("Exiting with signal %d\n", signum); 
    exit(signum);
}

int main(void)
{
    int s;

    // set handler for all signals
    // warning: _NSIG is potentially Linux-only
    for (s = 0; s < _NSIG; ++s)
    {
        signal(s, signal_handler);
    }
    // infinite loop waiting to be interrupted... 
    while (1)
    {
        printf("Doing nothing...\n");
        sleep(1);
    }

    return EXIT_SUCCESS;
}