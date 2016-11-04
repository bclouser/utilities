#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <vector>


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                       } while (0)


// for reference counting
std::vector<struct sigevent*> signals;
std::vector<struct itimerspec*> timers;

static void handler(int sig, siginfo_t *si, void *uc)
{
   /* 
    Note: calling printf() from a signal handler is not
    strictly correct, since printf() is not async-signal-safe;
    see signal(7) 
    */

   printf("Caught signal %d\n", sig);
   print_siginfo(si);
   //signal(sig, SIG_IGN);
}

static void handler2(int sig, siginfo_t *si, void *uc)
{
    printf("Caught signal %d\n", sig);
}


unsigned registerHandlerWithSignal( void (*handlerFunc)(int, siginfo_t*, void*) )
{   
    // First time only #static
    static unsigned thisSignal = SIGRTMIN; 
    struct sigaction sa;

    printf("Establishing handler for signal %d\n", thisSignal);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handlerFunc;
    sigemptyset(&sa.sa_mask);

    if (sigaction(thisSignal, &sa, NULL) == -1)
        errExit("sigaction");

    // Be sure to increment our signal count
    return thisSignal++;
}


timer_t registerTimerWithSignal(const unsigned sigNum)
{
    struct sigevent* sev = new sigevent;
    signals.push_back(sev);
    timer_t timerid;

    // Create the timer
    sev->sigev_notify = SIGEV_SIGNAL;
    sev->sigev_signo = sigNum;
    sev->sigev_value.sival_ptr = &timerid;

    if (timer_create(CLOCK_REALTIME, sev, &timerid) == -1)
    {
        delete signals.back();
        errExit("timer_create");
    }

    printf("timer ID is 0x%lx\n", (long) timerid);

    return timerid;
}


struct itimerspec createTimer(unsigned seconds)
{
    struct itimerspec timeSpec;

    timeSpec.it_value.tv_sec = seconds;
    timeSpec.it_value.tv_nsec = 0;
    timeSpec.it_interval.tv_sec = seconds;
    timeSpec.it_interval.tv_nsec = 0;

    return timeSpec;
}


void activateTimer(struct itimerspec* timeSpec, timer_t timerId)
{   
    if (timer_settime(timerId, 0, timeSpec, NULL) == -1)
    {
        errExit("timer_settime");
    }
    int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
}

void deActivateTimer(struct itimerspec* timeSpec, timer_t timerId)
{   
    // struct itimerspec* timeSpec = new struct itimerspec;
    // Add this to our list for deleting later
    // timers.push_back(timeSpec);

    timeSpec->it_value.tv_sec = 0;
    timeSpec->it_value.tv_nsec = 0;
    timeSpec->it_interval.tv_sec = 0;
    timeSpec->it_interval.tv_nsec = 0;

    if (timer_settime(timerId, 0, timeSpec, NULL) == -1)
    {
        //delete timers.back();
        errExit("timer_settime");
    }
}


int main(int argc, char *argv[])
{
    unsigned signum = 0;
    timer_t timerId;

    // Create the first timer
    signum = registerHandlerWithSignal( handler );
    printf("Just reqistered signal %d\n", signum);
    timerId = registerTimerWithSignal(signum);
    struct itimerspec timer1 = createTimer(3);
    activateTimer(&timer1, timerId);

/*
    // Create the second timer
    signum = registerHandlerWithSignal( handler );
    printf("Just registered signal %d\n", signum );
    timerId = registerTimerWithSignal( signum );
    activateTimer(3, timerId);
*/
    volatile int x = 0;

    while(1<2)
    {
        for(int i=0; i<0xFFFFFFFF; i++)
        {
            if(x==1)
            {
                // do something
            }
        }

        printf("disable timer %08X\n", timerId);
        deActivateTimer(&timer1, timerId);

        for(int i=0; i<0xFFFFFFFF; i++)
        {
            if(x==1)
            {
                // do something
            }
        }

        printf("enabling timer\n");
        activateTimer(&timer1, timerId);
    }

    for(int i = 0; i<timers.size(); i++)
    {   
        delete timers[i];
    }

    for(int i = 0; i<signals.size(); i++)
    {
        delete signals[i];

    }

    exit(EXIT_SUCCESS);
}