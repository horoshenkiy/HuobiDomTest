#pragma once
#include <time.h>

#define _FT_MICROSECOND ((__int64) 10)
#define _FT_MILLISECOND ((__int64) 10000)
#define _FT_SECOND      (1000 * _FT_MILLISECOND)
#define _FT_MINUTE      (60 * _FT_SECOND)
#define _FT_HOUR        (60 * _FT_MINUTE)
#define _FT_DAY         (24 * _FT_HOUR)
extern __int64 _FT_EPOCH;

struct STimeSpan;

// A wrapper for FILETIME structure (UTC time)
// Size of this structure is sizeof(ULONGLONG)
// The methods of this structure don't modify the stored value, but return a new 
// object when needed. The only exception is '=' operator
struct SDateTime
{
	SDateTime() : m_time(0) {}
	//	SDateTime(const FILETIME &ft) : m_time(AsUlonglong(ft)) {}
	SDateTime(const SDateTime &dt) : m_time(dt.m_time) {}
	explicit SDateTime(const ULONGLONG &n) : m_time(n) {}
	SDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond, int nMS);
	SDateTime& operator = (const SDateTime &dt) { m_time = dt.m_time; return *this; }

	FILETIME ToFileTime() const { return AsFileTime(m_time); }
	//operator FILETIME&() { return time; }
	//operator const FILETIME&() const { return time; }

	static SDateTime Now();
	static SDateTime UnixNow() { return SDateTime((ULONGLONG)time(0) * _FT_SECOND); }
	ULONGLONG ToTwimeTime() { return (m_time - _FT_EPOCH) * 100; }
	static SDateTime FromTwimeTime(UINT64 tt) { return SDateTime(UINT64(0.01 * tt + _FT_EPOCH)); }


	bool operator == (const SDateTime& dt) const { return m_time == dt.m_time; }
	bool operator != (const SDateTime& dt) const { return m_time != dt.m_time; }
	bool operator <  (const SDateTime& dt) const { return m_time <  dt.m_time; }
	bool operator >  (const SDateTime& dt) const { return m_time >  dt.m_time; }
	bool operator <= (const SDateTime& dt) const { return m_time <= dt.m_time; }
	bool operator >= (const SDateTime& dt) const { return m_time >= dt.m_time; }

	// Adds the specified number of milliseconds to the stored time
	// Method doesn't change the data of current object, but returns a new SDateTime object
	SDateTime AddMicroseconds(double mks) { return SDateTime((ULONGLONG)(m_time + mks * _FT_MICROSECOND)); }
	SDateTime AddMilliseconds(double ms) { return SDateTime((ULONGLONG)(m_time + ms * _FT_MILLISECOND)); }
	SDateTime AddSeconds(double seconds) { return SDateTime((ULONGLONG)(m_time + seconds * _FT_SECOND)); }
	SDateTime AddMinutes(double minutes) { return SDateTime((ULONGLONG)(m_time + minutes * _FT_MINUTE)); }
	SDateTime AddHours(double hours) { return SDateTime((ULONGLONG)(m_time + hours * _FT_HOUR)); }
	SDateTime AddDays(double days) { return SDateTime(m_time + (ULONGLONG)(days * _FT_DAY)); }

	int GetHour()        const { return (m_time / _FT_HOUR) % 24; }
	int GetMinute()      const { return (m_time / _FT_MINUTE) % 60; }
	int GetSecond()      const { return (m_time / _FT_SECOND) % 60; }
	int GetMillisecond() const { return (m_time / _FT_MILLISECOND) % 1000; }
	int GetMks() const { return (m_time / _FT_MICROSECOND) % 1000000; }


	STimeSpan Subtract(const SDateTime &value) const;
	SDateTime Subtract(const STimeSpan &value) const;
	SDateTime Add(const STimeSpan &value) const;

	// Returns "Date" value of current timestamp - returned value has the same date value
	// as the current instance, but the time value is set to midnight
	SDateTime GetDate() const { return SDateTime(m_time - (m_time % _FT_DAY)); }
	STimeSpan GetTime() const;
	int GetDayOfWeek() const;

	// For log purposes
	string ToString();
	string ToDateString();
	string ToFileName();
	string ToShortFileName();
	string ToShortTime() const;
	string ToSecondsTime();
	string ToShortTimeMks() const;

	__forceinline ULONGLONG GetUlonglong() { return m_time; }
	bool IsEmpty() { return m_time == 0; }

private:
	ULONGLONG m_time;

	//	SDateTime(ULONGLONG time) : m_time(time) {}

	__forceinline static ULONGLONG AsUlonglong(const FILETIME &time) { return (((ULONGLONG)time.dwHighDateTime) << 32) + time.dwLowDateTime; }
	__forceinline static FILETIME  AsFileTime(ULONGLONG time) { FILETIME ft = { (DWORD)(time & 0xFFFFFFFF), (DWORD)(time >> 32) }; return ft; }
};

struct STimeSpan
{
private:
	LONGLONG m_span;

public:
	STimeSpan() : m_span(0) {}
	STimeSpan(unsigned hour, unsigned minute, unsigned second, unsigned millisecond = 0, unsigned microsecond = 0);

	double GetTotalMicroseconds() const
	{
		return (double)((double)m_span / _FT_MICROSECOND);
	}

	double GetTotalMilliseconds() const
	{
		return (double)((double)m_span / _FT_MILLISECOND);
	}

	double GetTotalSeconds() const
	{
		return (double)((double)m_span / _FT_SECOND);
	}

	double GetTotalMinutes() const
	{
		return (double)((double)m_span / _FT_MINUTE);
	}

	double GetTotalHours() const
	{
		return (double)((double)m_span / _FT_HOUR);
	}

	static STimeSpan FromMilliseconds(double value)
	{
		return STimeSpan((LONGLONG)(value * _FT_MILLISECOND));
	}

	static STimeSpan FromMicroseconds(double value)
	{
		return STimeSpan((LONGLONG)(value * _FT_MICROSECOND));
	}

	bool IsZero() const { return m_span == 0; }
	bool IsNegative() const { return m_span < 0; }
	bool operator == (const STimeSpan& x) const { return m_span == x.m_span; }
	bool operator != (const STimeSpan& x) const { return m_span != x.m_span; }
	bool operator <  (const STimeSpan& x) const { return m_span < x.m_span; }
	bool operator >(const STimeSpan& x) const { return m_span > x.m_span; }
	bool operator <= (const STimeSpan& x) const { return m_span <= x.m_span; }
	bool operator >= (const STimeSpan& x) const { return m_span >= x.m_span; }

	std::string ToString();
	//	static STimeSpan FromString(__in LPCSTR szInput, __out int &nDay, __out int &nMonth, __out int &nYear, __out int &nHour, __out int &nMin, __out int &nSec);
	static STimeSpan FromString(LPCSTR szInput);
	static STimeSpan FromMksString(LPCSTR szInput);
private:
	STimeSpan(LONGLONG span) : m_span(span) {}

	friend struct SDateTime;
};

inline STimeSpan operator-(const SDateTime &a, const SDateTime &b)
{
	return a.Subtract(b);
}

inline SDateTime operator-(const SDateTime &a, const STimeSpan &b)
{
	return a.Subtract(b);
}

inline SDateTime operator+(const SDateTime &a, const STimeSpan &b)
{
	return a.Add(b);
}

