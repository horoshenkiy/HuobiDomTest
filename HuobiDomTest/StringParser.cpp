#include "stdafx.h"
#include "StringParser.h"

bool CStringParser::ParseString(char *psz)
{
	m_nBids = 0;
	m_nAsks = 0;
	int nLen = strlen(psz);
	if (nLen < 200)
	{
		printf("ParseString() : string size less than 200, error\n");
		return false;
	}

	const int nEndDateTime = 24;
	if (psz[0] != '[' || psz[nEndDateTime] != ']')
	{
		printf("ParseString() : invalid string format (does not start with \'[\' or ending DateTime symbol does not equal \']\')\n");
		return false;
	}

	char *p = psz + nEndDateTime;
	// parse DateTime, size 25
	//[2020 - 08 - 23 19:19 : 29.137]
	*p++ = '\0';
	m_szDateTime = psz + 1;

/*
we do not use update type, snapshot is the same as usual update
	const char *pszType = "\"type\": \"";
	const int nTypeLen = strlen(pszType);
	p = strstr(p, pszType);
	if (p == 0)
	{
		printf("ParseString() : cannot find update \"type\"\n");
		return false;
	}

	p += nTypeLen;
	bool bSnapshot = (p[0] == 's');		// snapshot update type
*/


	ParseAsks(p);
	ParseBids(p);
	if (m_nAsks == 3)
	{
		int aa = 0;
		aa++;
	}

	if (m_nAsks > nMaxAsks)
		nMaxAsks = m_nAsks;
	if (m_nBids > nMaxBids)
		nMaxBids = m_nBids;

	return true;
}

void CStringParser::ParseAsks(char *&p)
{
	const char *pszAsks = "asks\": [";
	const int nAsksLen = strlen(pszAsks);
	p = strstr(p, pszAsks);
	if (p == 0)
	{
		printf("ParseAsks() : cannot find asks\n");
		return;
	}
	p += nAsksLen;
	ParsePrices(p, m_nAsks, m_asks);
}

void CStringParser::ParseBids(char *&p)
{
	const char *pszBids = "bids\": [";
	const int nBidsLen = strlen(pszBids);
	p = strstr(p, pszBids);
	if (p == 0)
	{
		printf("ParseBids() : cannot find bids\n");
		return;
	}
	p += nBidsLen;
	ParsePrices(p, m_nBids, m_bids);
}

void CStringParser::ParsePrices(char *&p, int &nCounter, CryptoBand *bands)
{
	while (*p != ']')
	{
		ParsePriceAndVolume(p, nCounter, bands);
		if (*p == ',')
			p += 2;		//more [price, volume] to parse
	}
}

void CStringParser::ParsePriceAndVolume(char *&p, int &nCounter, CryptoBand *bands)
{
	char *pSave = p;
	// parse [price, volume]
	if (*p++ != '[')
		return;		// no prices
	double fPrice = atof(p);
	p = strchr(p, ',');
	if (p == 0)
	{
		printf("ParsePriceAndVolume() cannot parse price: %s\n", p);
		exit(1);
	}
	p += 2;

	double fVolume = atof(p);
	p = strchr(p, ']');
	if (p == 0)
	{
		printf("ParsePriceAndVolume() cannot parse volume: %s\n", p);
		exit(1);
	}
	++p;

	if (nCounter >= DOM_SIZE * 2)
	{
		printf("ParsePriceAndVolume() number of updates exceeds DOM_SIZE * 2: %s\n", p);
		exit(1);
	}

	bands[nCounter].m_nPrice = (CryptoPrice)(fPrice * PRICE_MUL + 0.5);
	bands[nCounter].m_fVolume = fVolume;
	nCounter++;
}

void CStringParser::CopyParsedData(SingleLineUpdate &slu)
{
	slu.m_szDateTime = m_szDateTime;
	for (int b = 0; b < m_nBids; b++)
	{
		slu.m_bids.push_back(m_bids[b]);
	}

	for (int a = 0; a < m_nAsks; a++)
	{
		slu.m_asks.push_back(m_asks[a]);
	}
}
