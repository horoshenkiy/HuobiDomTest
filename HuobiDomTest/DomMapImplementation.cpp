#include "stdafx.h"
#include "DomMapImplementation.h"

void CDomMapImplementation::ProcessSnapshot(int nBids, CryptoBand *pBids, int nAsks, CryptoBand *pAsks)
{
	for (int b = 0; b < nBids; b++)
	{
		m_bids[pBids[b].m_nPrice] = pBids[b].m_fVolume;
	}

	for (int a = 0; a < nAsks; a++)
	{
		m_asks[pAsks[a].m_nPrice] = pAsks[a].m_fVolume;
	}
}

void CDomMapImplementation::ProcessUpdate(int nBids, CryptoBand *pBids, int nAsks, CryptoBand *pAsks)
{
	for (int b = 0; b < nBids; b++)
	{
		CryptoBand &cp = pBids[b];
		CryptoMap::iterator findIt = m_bids.find(cp.m_nPrice);
		if (findIt != m_bids.end())
		{
			if (cp.m_fVolume == 0)
				m_bids.erase(findIt);					// delete band
			else
				findIt->second = cp.m_fVolume;			// update volume
		}
		else
			m_bids[cp.m_nPrice] = cp.m_fVolume;			// insert new item
	}

	for (int a = 0; a < nAsks; a++)
	{
		CryptoBand &cp = pAsks[a];
		CryptoMap::iterator findIt = m_asks.find(cp.m_nPrice);
		if (findIt != m_asks.end())
		{
			if (cp.m_fVolume == 0)
				m_asks.erase(findIt);					// delete band
			else
				findIt->second = cp.m_fVolume;			// update volume
		}
		else
			m_asks[cp.m_nPrice] = cp.m_fVolume;			// insert new item
	}
}

void CDomMapImplementation::ProcessUpdate(const vector<CryptoBand> &bids, const vector<CryptoBand> &asks)
{
	for (const CryptoBand &cp : bids)
	{
		CryptoMap::iterator findIt = m_bids.find(cp.m_nPrice);
		if (findIt != m_bids.end())
		{
			if (cp.m_fVolume == 0)
				m_bids.erase(findIt);					// delete band
			else
				findIt->second = cp.m_fVolume;			// update volume
		}
		else
			m_bids[cp.m_nPrice] = cp.m_fVolume;			// insert new item
	}

	for (const CryptoBand &cp : asks)
	{
		CryptoMap::iterator findIt = m_asks.find(cp.m_nPrice);
		if (findIt != m_asks.end())
		{
			if (cp.m_fVolume == 0)
				m_asks.erase(findIt);					// delete band
			else
				findIt->second = cp.m_fVolume;			// update volume
		}
		else
			m_asks[cp.m_nPrice] = cp.m_fVolume;			// insert new item
	}
}

void CDomMapImplementation::ClearDOM()
{
	m_bids.clear();
	m_asks.clear();
}

void CDomMapImplementation::PrintDOM()
{
	for (CryptoMap::iterator it = m_bids.begin(); it != m_bids.end(); ++it)
	{
		printf("%d\t%.6lf\n", it->first, it->second);
	}
	printf("\n");

	for (CryptoMap::iterator it = m_asks.begin(); it != m_asks.end(); ++it)
	{
		printf("%d\t%.6lf\n", it->first, it->second);
	}
	printf("\n");
}

void CDomMapImplementation::PrintTopOfTheBook(const string &szDateTime)
{
	if (m_bids.empty() || m_asks.empty())
		return;
	CryptoMap::reverse_iterator bit = m_bids.rbegin();
	CryptoMap::iterator ait = m_asks.begin();
	printf("%s\t\t%d, %.6lf\t %d, %.6lf\n", szDateTime.c_str(), bit->first, bit->second, ait->first, ait->second);
}

