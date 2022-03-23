#include "stdafx.h"
#include "DateTime.h"

SDateTime::SDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond, int nMS)
{
	FILETIME ft;
	SYSTEMTIME st = { (WORD)nYear, (WORD)nMonth, 0, (WORD)nDay, (WORD)nHour, (WORD)nMinute, (WORD)nSecond, (WORD)nMS };
	SystemTimeToFileTime(&st, &ft);
	m_time = AsUlonglong(ft);
}

SDateTime SDateTime::Now()
{
	FILETIME res;
	GetSystemTimeAsFileTime(&res);

	static LONGLONG nShift = 0;
	if (!nShift)
	{
		FILETIME localTime = { 0 };
		FileTimeToLocalFileTime(&res, &localTime);
		ULONGLONG nLocal = AsUlonglong(localTime);
		ULONGLONG nUTC = AsUlonglong(res);
		nShift = nLocal - nUTC;
	}

	ULONGLONG nRes = AsUlonglong(res) + nShift;
	return SDateTime(nRes);
}

STimeSpan SDateTime::Subtract(const SDateTime &value) const
{
	return STimeSpan(m_time - value.m_time);
}

SDateTime SDateTime::Subtract(const STimeSpan &value) const
{
	return SDateTime(m_time - value.m_span);
}

SDateTime SDateTime::Add(const STimeSpan &value) const
{
	return SDateTime(m_time + value.m_span);
}

STimeSpan SDateTime::GetTime() const
{
	return STimeSpan(m_time % _FT_DAY);
}

/*
int SDateTime::GetDayOfWeek() const
{
FILETIME ft = ToFileTime();
SYSTEMTIME st;
FileTimeToSystemTime(&ft, &st);

int mon;
if (st.wMonth > 2)
mon = st.wMonth;			//for march to december month code is same as month
else
st.wYear--;
//	mon = (12 + st.wMonth);		//for Jan and Feb, month code will be 13 and 14 year--; //decrease year for month Jan and Feb
int y = st.wYear % 100;			//last two digit
int c = st.wYear / 100;			//first two digit
int w = (st.wDay + floor((13 * (st.wMonth + 1)) / 5) + y + floor(y / 4) + floor(c / 4) + (5 * c));
return (w - 1) % 7;
}
*/

int SDateTime::GetDayOfWeek() const
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	/*
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	int y = st.wYear;
	int m = st.wMonth;			//for march to december month code is same as month
	if (st.wMonth <= 2)
	y--;			//for march to december month code is same as month
	int d = st.wDay;

	int w = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
	int aa = 0;
	return w;
	*/
	return st.wDayOfWeek;
}

string SDateTime::ToString()
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return szBuf;
}

string SDateTime::ToDateString()
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%d.%02d.%02d",
		st.wYear, st.wMonth, st.wDay);
	return szBuf;
}

string SDateTime::ToFileName()
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%d%02d%02d_%02d%02d%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return szBuf;
}

string SDateTime::ToShortFileName()
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%d%02d%02d",
		st.wYear, st.wMonth, st.wDay);
	return szBuf;
}

string SDateTime::ToShortTime() const
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%02d:%02d:%02d:%03d",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return szBuf;
}

string SDateTime::ToShortTimeMks() const
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%02d:%02d:%02d:%06d",
		st.wHour, st.wMinute, st.wSecond, GetMks());
	return szBuf;
}

string SDateTime::ToSecondsTime()
{
	FILETIME ft = ToFileTime();
	SYSTEMTIME st;
	char szBuf[255];

	FileTimeToSystemTime(&ft, &st);
	sprintf_s(szBuf, sizeof(szBuf), "%02d:%02d:%02d",
		st.wHour, st.wMinute, st.wSecond);
	return szBuf;
}


STimeSpan::STimeSpan(unsigned hour, unsigned minute, unsigned second, unsigned millisecond, unsigned microsecond)
{
	if (hour >= 24 || minute >= 60 || second >= 60 || millisecond >= 1000 || microsecond >= 1000)
		throw std::runtime_error("Invalid time span value");
	m_span = hour * _FT_HOUR + minute * _FT_MINUTE + second * _FT_SECOND + millisecond * _FT_MILLISECOND + microsecond * _FT_MICROSECOND;
}

std::string STimeSpan::ToString()
{
	LONGLONG span = _abs64(m_span);
	int days = (int)(span / _FT_DAY);
	int hours = (span / _FT_HOUR) % 24;
	int minutes = (span / _FT_MINUTE) % 60;
	int seconds = (span / _FT_SECOND) % 60;
	int ms = (span / _FT_MILLISECOND) % 1000;

	char szBuf[255];
	if (days > 0)
	{
		sprintf_s(szBuf, sizeof(szBuf), "%s%dd %0d:%02d:%02d.%03d",
			(m_span < 0 ? "-" : ""), days, hours, minutes, seconds, ms);
	}
	else
	{
		sprintf_s(szBuf, sizeof(szBuf), "%s%0d:%02d:%02d.%03d",
			(m_span < 0 ? "-" : ""), hours, minutes, seconds, ms);
	}
	return szBuf;
}

STimeSpan STimeSpan::FromString(LPCSTR szInput)
{
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;
	int nMS = 0;
	if ((4 != sscanf_s(szInput, "%lu:%lu:%lu:%lu", &nHour, &nMin, &nSec, &nMS))
		&&
		(3 != sscanf_s(szInput, "%lu:%lu:%lu", &nHour, &nMin, &nSec))
		)
	{
		throw std::runtime_error("Invalid STimeSpan::FromString() value");
		return 0;
	}
	return FromMilliseconds((nHour * 3600 + nMin * 60 + nSec) * 1000 + nMS);
}

STimeSpan STimeSpan::FromMksString(LPCSTR szInput)
{
	LONGLONG nHour = 0;
	LONGLONG nMin = 0;
	LONGLONG nSec = 0;
	LONGLONG nMS = 0;
	if (4 != sscanf_s(szInput, "%lu:%lu:%lu:%lu", &nHour, &nMin, &nSec, &nMS))
	{
		throw std::runtime_error("Invalid STimeSpan::FromMksString() value");
		return 0;
	}
	return FromMicroseconds((nHour * 3600 + nMin * 60 + nSec) * 1000000 + nMS);
}

