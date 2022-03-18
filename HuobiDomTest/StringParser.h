#pragma once
#include "common.h"


struct CryptoBand
{
	CryptoPrice m_nPrice;
	CryptoVolume m_fVolume;
};

struct SingleLineUpdate
{
	string m_szDateTime;
	vector<CryptoBand> m_bids;
	vector<CryptoBand> m_asks;
};

class CStringParser
{
public:
	string m_szDateTime;
	int m_nBids = 0;								// number of bids during current parse
	CryptoBand m_bids[DOM_SIZE * 2];

	int m_nAsks = 0;								// number of asks during current parse
	CryptoBand m_asks[DOM_SIZE * 2];

	int nMaxBids = 0;
	int nMaxAsks = 0;

public:
	bool ParseString(char *psz);
	void CopyParsedData(SingleLineUpdate &slu);

private:
	void ParseAsks(char *&p);
	void ParseBids(char *&p);
	void ParsePrices(char *&p, int &nCounter, CryptoBand *bands);
	void ParsePriceAndVolume(char *&p, int &nCounter, CryptoBand *bands);
};
