#include <signal.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>


void callback(int signum)
{
	printf("Ok so were called. This was our signal number: %d\n", signum);
}

int main()
{
	signal(SIGALRM, callback);
	alarm(5);

	while(1)
	{

	}

}