#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include <math.h>
#include "HighResClock.h"


ULONGLONG HighResClock::m_nFrequency = 0;
ULONGLONG HighResClock::m_nOverhead = 0;
LONGLONG  HighResClock::m_nTickOffset = 0;


/// <summary>
/// Measure overhead for interop calls and clock resolution.
/// The resultant values are stored in static fields.
/// The resultant clock frequency and function call overhead
/// measurement are used in ticks to ms conversion and 
/// duration calculations, respectively.
/// </summary>
/// <remarks>
/// <para>
/// This is called from the static constructor, so you typically
/// won't benefit from calling it, unless during construction
/// there were other very high priority things going on that
/// affected the timing of the statements in this method.
/// </para>
/// <para>
/// This temporarily boosts this process and thread priority
/// to avoid being affected by other processes/threads, so you 
/// definitely don't want to call this a lot.
/// </para>
/// </remarks>
void HighResClock::Calibrate()
{
	LARGE_INTEGER t;
	::QueryPerformanceFrequency(&t);
	m_nFrequency = t.QuadPart;
	if (m_nFrequency == 0)
		return;

	// boost process and thread priority temporarily
	HANDLE hThread = GetCurrentThread();
	int nOldThreadPriority = GetThreadPriority(hThread);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	HANDLE hProcess = ::GetCurrentProcess();
	BOOL bOldProcessPriorityBoost = FALSE;
	GetProcessPriorityBoost(hProcess, &bOldProcessPriorityBoost);
	SetProcessPriorityBoost(hProcess, FALSE);

	// TODO
	//	ULONGLONG nowTicks = NowTicks();
	//	ULONGLONG dtTicks = SDateTime::Now().GetUlonglong();
	m_nTickOffset = NowTicks() - SDateTime::Now().GetUlonglong();

	// calculate measurement overhead by taking a set of measurements
	ULONGLONG nStart = 0;
	ULONGLONG nStop = 0;
	m_nOverhead = ULONGLONG((LONGLONG)-1);

	for (int i = 0; i < 100; i++)
	{
		nStart = NowTicks();
		nStop = NowTicks();
		ULONGLONG nDT = SDateTime::Now().GetUlonglong();
		ULONGLONG nTicks = nStop - nStart;
		if (nTicks > 0 && nTicks < m_nOverhead)
			m_nOverhead = nTicks;

		LONGLONG nOffset = nStop - nDT;
		if (nOffset < m_nTickOffset)
			m_nTickOffset = nOffset;
	}

	// handle case where no measurement gave us a difference
	// in case the overhead is under the perf counter resolution
	if (m_nOverhead == ULONGLONG((LONGLONG)-1))
		m_nOverhead = 0;

	SetThreadPriority(hThread, nOldThreadPriority);
	SetProcessPriorityBoost(hProcess, bOldProcessPriorityBoost);

	/*
	LONGLONG nTickOffset2 = NowTicks() - SDateTime::Now().GetUlonglong();
	Sleep(100);
	LONGLONG nTickOffset3 = NowTicks() - SDateTime::Now().GetUlonglong();

	for (int i = 0; i < 2000; i++)
	{
	Sleep(10);
	}
	LONGLONG nTickOffset4 = NowTicks() - SDateTime::Now().GetUlonglong();

	// kill some time
	double res = 0.0;
	for (int i = 0; i < 50000; i++)
	{
	res += sin(cos(acos(1.0)));
	}
	LONGLONG nTickOffset5 = NowTicks() - SDateTime::Now().GetUlonglong();

	LONGLONG lTT = nTickOffset5 - m_nTickOffset;
	int tt = 0;
	*/
}


/// <summary>
/// A simple self test.
/// </summary>
/// <returns> bool - true for test passed, false for failed</returns>
bool HighResClock::TestSelf()
{
	Calibrate();
	printf("Measurement overhead: %I64d ticks", m_nOverhead);
	printf("Frequency: %I64d cpuTicks/sec", m_nFrequency);

	double realFreq = 1.4 * 1024 * 1024 * 1024;
	printf("1.4 GHz CPU clock freq / perf counter freq = %.3f", realFreq / m_nFrequency);

	// figure out how often high perf counter wraps
	double rangeSeconds = (double)((ULONGLONG)(LONGLONG(-1))) / m_nFrequency;
	double rangeDays = rangeSeconds / (3600 * 24);
	double rangeYears = rangeDays / 365;
	printf("Span of clock is %lf s, %lf d, %lf years",
		rangeSeconds, rangeDays, rangeYears);


	ULONGLONG start = NowTicks();
	ULONGLONG stop = NowTicks();
	ULONGLONG stop1 = NowTicks();

	printf("duration: %I64d ", stop - start);
	printf("duration1: %I64d ms", stop1 - start);
	printf("duration: %I64d ms", NowTicks() - start);


	// check against sleep
	start = NowTicks();
	int sleepTime = 100;
	Sleep(sleepTime);
	stop = NowTicks();
	printf("Slept %d ms and that gave measured %I64d ticks", sleepTime, stop - start);
	printf("This is typically a little off (5ms?), probably due to thread scheduling latencies.");

	/*
	// try TimeSpan
	TimeSpan span = HighResClock.CalcTimeSpan(start, stop);
	Console.WriteLine("The same duration as a time span is {0} ms", span.TotalMilliseconds);

	// try DateTime
	Console.WriteLine("My DateTime: {0},   .Net's DateTime: {1}",
	HighResClock.Now, DateTime.Now);

	//
	// time something with DateTime
	//
	DateTime startDateTime = DateTime.Now;
	DateTime startHrc = HighResClock.Now;

	// kill some time
	double res = 0.0;
	for (int i = 0; i < 50000; i++)
	{
	res += Math.Sin(Math.Cos(Math.Acos(1.0)));
	}

	span = DateTime.Now - startDateTime;
	TimeSpan spanHrc = HighResClock.Now - startHrc;
	Console.WriteLine("DateTime's timing: {0} ms", span.TotalMilliseconds);
	Console.WriteLine("HighResClock's timing: {0} ms", spanHrc.TotalMilliseconds);

	//
	// check DateTime's resolution
	//
	startDateTime = DateTime.Now;
	double maxMsGap = 100000;
	for (int i = 0; i < 100; i++)
	{
	double msGap = ((TimeSpan)(DateTime.Now - startDateTime)).TotalMilliseconds;
	Thread.Sleep(1);
	if ((msGap > 0) && (msGap < maxMsGap)) maxMsGap = msGap;
	}
	Console.WriteLine("DateTime's resolution is {0:f2} ms", maxMsGap);

	//
	// check HighResClock's DateTime resolution
	//
	startDateTime = HighResClock.Now;
	maxMsGap = 100000;
	for (int i = 0; i < 100; i++)
	{
	double msGap = ((TimeSpan)(HighResClock.Now - startDateTime)).TotalMilliseconds;
	Thread.Sleep(1);
	if ((msGap > 0) && (msGap < maxMsGap)) maxMsGap = msGap;
	}
	Console.WriteLine("HighResClock's DateTime resolution is {0} ms", maxMsGap);
	*/

	return true;
}
