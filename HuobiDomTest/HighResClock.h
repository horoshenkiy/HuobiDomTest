#pragma once

#include "DateTime.h"

class HighResClock
{
private:
	static ULONGLONG m_nFrequency;		// System frequency in ticks
	static ULONGLONG m_nOverhead;		// The minimum duration that this class is able to measure. 
	static LONGLONG  m_nTickOffset;		// This stores the offset between Kernel32's ticks and SDateTime ticks

public:
	static ULONGLONG NowTicks()
	{
		//LARGE_INTEGER;
		LARGE_INTEGER t;
		::QueryPerformanceCounter(&t);
		// convert to SDateTime ticks
		return (ULONGLONG)((double) 1e7 * t.QuadPart / m_nFrequency);
	}

	static SDateTime Now()
	{
		return SDateTime(NowTicks() - m_nTickOffset);
	}

	// Measure overhead for interop calls and clock resolution. The resultant values are stored in static fields.
	static void Calibrate();

	static bool TestSelf();
};

