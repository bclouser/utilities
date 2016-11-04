#ifndef __TIMEOUT__
#define __TIMEOUT__


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <vector>
#include <map>
#include <pthread.h>


static int& signalCount() {
    static int signalCount = 0;
    return signalCount;
}

static std::map<unsigned,void*> signalKeyInstanceMap;

template<typename SomeClass>
class Timeout {
public:
	Timeout()
	{
		m_signum = 0;
		m_callBackClassInstance = 0;
		m_callBackMemberFunction = 0;
	}
	~Timeout()
	{

	}

	bool createIterativeTimeout(SomeClass* classInstancePointer, void (SomeClass::* memberFunctionPointer)(void) )
	{
		m_callBackClassInstance = classInstancePointer;
		m_callBackMemberFunction = memberFunctionPointer;

		// Populates the m_signum variable
		// We always register this local callback. IT IS STATIC
	    if( !registerHandlerWithSignal( &Timeout::callCallBack ) )
	    {
	    	return false;
	    }

	    // Register signal number and this pointer in static map
	    signalKeyInstanceMap[m_signum] = (void*)this;

	    if( !registerTimerWithSignal() )
	    {
	    	return false;
	    }

	    // default 10, It doesn't matter. Gets wiped out with activate
		m_timerSpec = createTimer(10);

		return true;
	}



	bool activate(unsigned seconds)
	{
		printf("activating timer on signal %d for %d seconds\n", m_signum, seconds);
		m_timerSpec.it_value.tv_sec = seconds;
		m_timerSpec.it_value.tv_nsec = 0;
		m_timerSpec.it_interval.tv_sec = seconds;
		m_timerSpec.it_interval.tv_nsec = 0;


		if (timer_settime(m_timerId, 0, &m_timerSpec, NULL) == -1)
		{
		    return false;
		}
		return true;
	}

	bool deActivate()
	{
		printf("deActivating timer on signal %d\n", m_signum );
		// Setting everything to zero disables timeout
		m_timerSpec.it_value.tv_sec = 0;
		m_timerSpec.it_value.tv_nsec = 0;
		m_timerSpec.it_interval.tv_sec = 0;
		m_timerSpec.it_interval.tv_nsec = 0;

		if (timer_settime(m_timerId, 0, &m_timerSpec, NULL) == -1)
		{
		    return false;
		}
		return true;
	}

private:
	unsigned m_signum;
	timer_t m_timerId;
	struct itimerspec m_timerSpec;
	struct sigevent m_signalEvent;

	SomeClass* m_callBackClassInstance; 
	void (SomeClass::* m_callBackMemberFunction)(void);


	bool registerHandlerWithSignal( void (*handlerFunc)(int, siginfo_t*, void*) )
	{
		m_signum = SIGRTMIN + signalCount(); 
		struct sigaction sa;

		printf("Establishing handler for signal %d\n", m_signum);
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = handlerFunc;
		sigemptyset(&sa.sa_mask);

		if (sigaction(m_signum, &sa, NULL) == -1)
		{
			return false;
		}
		// Be sure to increment our static signal count
		++signalCount();
		return true;
	}

	bool registerTimerWithSignal()
	{
		// Create the timer
		m_signalEvent.sigev_notify = SIGEV_SIGNAL;
		m_signalEvent.sigev_signo = m_signum;
		m_signalEvent.sigev_value.sival_ptr = &m_timerId;

		if (timer_create(CLOCK_REALTIME, &m_signalEvent, &m_timerId) == -1)
		{
			return false;
		}

		printf("timer ID is 0x%lx\n", (long)m_timerId);

		return true;
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

	void testFunction()
	{
		printf("Made it to dummy function \n");
	}

	// This is what the OS will call when a timer signal is fired
	static void callCallBack(int sigNum, siginfo_t*, void*)
	{
		printf("Made it to callCallBack for signal %d\n", sigNum);

		Timeout* instance = (Timeout*)signalKeyInstanceMap[sigNum];

		if(!instance)
		{
			//bad things
		}

		// Yeah, ok. So we recovered the instance of this class (Timeout).
		// Now we we want to call the real callback registered by the user
		// of this class when they called createIterativeTimeout()

		// Its a member function so we saved their class instance as well.
		// Gross. Yes. Here we go!
		( *(instance->m_callBackClassInstance).*(instance->m_callBackMemberFunction) )();
		//(*object.*function)();

		//instance->testFunction();

	}
};

#endif