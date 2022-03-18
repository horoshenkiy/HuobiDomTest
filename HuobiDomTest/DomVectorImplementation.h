#pragma once
#include "common.h"
#include "StringParser.h"

class CDomMapImplementation;
class CDomVectorImplementation
{
	friend bool AreDomsEquals(const CDomMapImplementation &domMap, const CDomVectorImplementation &domVector);

private:
	using CryptoVector = vector<CryptoBand>;
	CryptoVector m_bids;
	CryptoVector m_tempBids;
	CryptoVector m_asks;
	CryptoVector m_tempAsks;

public:
	void ProcessUpdate(const vector<CryptoBand> &bids, const vector<CryptoBand> &asks);

	void ClearDOM();
	void PrintDOM();
	void PrintTopOfTheBook(const string &szDateTime);

private:
	inline void UpdateBids(const vector<CryptoBand> &updateBids);
	__forceinline void UpdateAsks(const vector<CryptoBand> &updateAsks);
};
