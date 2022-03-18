// HuobiTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StringParser.h"
#include "DomMapImplementation.h"
#include "DomVectorImplementation.h"
#include "HighResClock.h"
#include "DateTime.h"

int main()
{
	string szSourceLog = "huobi_global_depth.log";
	FILE* pFile = 0;
	fopen_s(&pFile, szSourceLog.c_str(), "r");
	if (pFile == 0)
	{
		printf("Cannot read huobi log file: %s\n", szSourceLog.c_str());
		return -1;
	}

	const int MAX_BUF = 4096;
	char buf[MAX_BUF];
	CStringParser sp;
	CDomMapImplementation domMap;
	CDomVectorImplementation domVector;
	HighResClock hrc;
	hrc.Calibrate();

	vector<SingleLineUpdate> parsedDataStorage;
	while (fgets(buf, MAX_BUF, pFile) != NULL)
	{
		if (!sp.ParseString(buf))
		{
			printf("main() ParseString() parsing error: %s\n", buf);
			return -1;
		}

		SingleLineUpdate slu;
		sp.CopyParsedData(slu);
		parsedDataStorage.push_back(slu);
	}
	fclose(pFile);
	pFile = 0;

	int nLine = 0;
	for (SingleLineUpdate& slu : parsedDataStorage)
	{
		domMap.ProcessUpdate(slu.m_bids, slu.m_asks);
		//		domMap.PrintTopOfTheBook(slu.m_szDateTime);

		domVector.ProcessUpdate(slu.m_bids, slu.m_asks);
		domVector.PrintTopOfTheBook(slu.m_szDateTime);
		nLine++;
		if (!AreDomsEquals(domMap, domVector))
		{
			printf("domMap != domVector at line %d\n", nLine);
		}
	}


	// measure map implementation DOM speed
	domMap.ClearDOM();
	ULONGLONG n1 = hrc.NowTicks();
	SDateTime dt1 = hrc.Now();
	for (SingleLineUpdate& slu : parsedDataStorage)
	{
		domMap.ProcessUpdate(slu.m_bids, slu.m_asks);
	}
	ULONGLONG n2 = hrc.NowTicks();
	SDateTime dt2 = hrc.Now();

	STimeSpan ts = dt2 - dt1;
	ULONGLONG ticks = n2 - n1;
	printf("\nDomMap time elapsed: %s, ticks=%I64d\n\n", ts.ToString().c_str(), ticks);


	// measure vector implementation DOM speed
	domVector.ClearDOM();
	n1 = hrc.NowTicks();
	dt1 = hrc.Now();
	for (SingleLineUpdate& slu : parsedDataStorage)
	{
		domVector.ProcessUpdate(slu.m_bids, slu.m_asks);
	}
	n2 = hrc.NowTicks();
	dt2 = hrc.Now();

	ts = dt2 - dt1;
	ticks = n2 - n1;
	printf("DomVector time elapsed: %s, ticks=%I64d\n\n", ts.ToString().c_str(), ticks);

	return 0;
}

