#pragma once
#include "common.h"
#include "StringParser.h"

class CDomVectorImplementation;
class CDomMapImplementation
{
	friend bool AreDomsEquals(const CDomMapImplementation &domMap, const CDomVectorImplementation &domVector);

private:
	using CryptoMap = map<CryptoPrice, CryptoVolume>;
	CryptoMap m_bids;
	CryptoMap m_asks;

public:
	void ProcessSnapshot(int nBids, CryptoBand *pBids, int nAsks, CryptoBand *pAsks);
	void ProcessUpdate(int nBids, CryptoBand *pBids, int nAsks, CryptoBand *pAsks);
	void ProcessUpdate(const vector<CryptoBand> &bids, const vector<CryptoBand> &asks);

	void ClearDOM();
	void PrintDOM();
	void PrintTopOfTheBook(const string &szDateTime);
};
