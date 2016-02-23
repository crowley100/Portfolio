#include "stdafx.h"
#include "conio.h"	// _getch()
#include "assert.h"
#include "time.h"
#include <iomanip>
#include <iostream>

using namespace std;

#define REG_WINDOWS	16 // Register sets 16 -> 8 -> 6

int ackermann(int x, int y)
{
	if(x == 0)
	{
		return y+1;
	}
	else if(y == 0)
	{
		return ackermann(x-1, 1);
	}
	else
	{
		return ackermann(x-1, ackermann(x, y-1));
	}
}

// global variables for ackermann metrics
int calls = 0;
int win_depth = 0;
int max_depth = 0;
int win_used = 1;
int overflows = 0;
int underflows = 0;

// instrumented code...
int ackermann_instrumented(int x, int y)
{
	int result;
	win_depth++;
	calls++;
	if(win_depth > max_depth)
	{
		max_depth = win_depth;
	}
	assert((wused >= 1) && (wused <= REG_WINDOWS-1)); // 1 window must be kept 'free'
	// check for overflow before running ackermann
	if(win_used == (REG_WINDOWS-1))
	{
		overflows++;
	}
	else
	{
		win_used++;
	}

	// *** ackermann start ***
	if(x == 0)
	{
		result = y+1;
	}
	else if(y == 0)
	{
		result = ackermann_instrumented(x-1, 1);
	}
	else
	{
		result = ackermann_instrumented(x-1, ackermann_instrumented(x, y-1));
	}
	// *** ackermann end ***

	win_depth--;
	assert((win_used >= 1) && (win_used <= REG_WINDOWS-1)); // 1 window must be kept 'free'
	// check for underflow after running ackermann
	if(win_used > 1)
	{
		win_used--;
	}
	else
	{
		underflows++;
	}
	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int result;
	// Q2
	// running instrumented code...
	result = ackermann_instrumented(3, 6);
	cout << "ackermann(3, 6) = " << result << endl
		 << "calls = " << calls << endl 
		 << "max depth = " << max_depth << endl
		 << "REG_WINDOWS = " << REG_WINDOWS << endl
		 << "overflows = " << overflows << endl
		 << "underflows = " << underflows << endl;
	
	// error handling...
	if(overflows != underflows)
	{
		cout << "error!" << endl
			 << "overflows = " << overflows << endl
			 << "underflows = " << underflows << endl
			 << "they should be the same..." << endl;
	}
	if(win_used != 1)
	{
		cout << "error!" << endl
			 << "win_used = " << win_used << endl
			 << "it should = 1" << endl;
	}
	

	// Q3
	// calculating ackermann runtime...
	result = 0;

	// hack to stop the compiler 'optimising' the for-loop away...
	volatile int num_3 = 3;

	cout << endl << "timing ackermann runtime..." << endl;
	int timer = clock();

	// As the runtime is less than a millisecond, running ackermann 5000 times
	// and dividing the runtime by 5000 to get an accurate approximation of the runtime of ackermann.
	for(int i = 0; i < 5000; i++)
	{
		result = ackermann(num_3, 6);
	}

	timer = clock()-timer;
	// calculating timer in ms...
	cout << "ackermann(3, 6) = " << result << endl
	     << "runtime: " << fixed << setprecision(2) << (double)timer * 1000.0 / CLOCKS_PER_SEC / 5000
		 << "ms" << endl
	     << "finished." << endl;

	_getch();

	return 0;
}


