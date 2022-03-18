#include "stdafx.h"
#include "DomVectorImplementation.h"

void CDomVectorImplementation::ProcessUpdate(const vector<CryptoBand> &updateBids, const vector<CryptoBand> &updateAsks)
{
	UpdateBids(updateBids);
	UpdateAsks(updateAsks);
}

void CDomVectorImplementation::UpdateBids(const vector<CryptoBand> &updateBids)
{
	CryptoVector::iterator ex = m_bids.begin();						// existing bids
	CryptoVector::const_iterator up = updateBids.begin();			// update bids
	m_tempBids.clear();
	for (;;)
	{
		if (ex == m_bids.end())
		{
			// processed all existing bids
			// copy all data from temp vector
			m_bids = m_tempBids;
			// copy rest data from update
			m_bids.insert(m_bids.end(), up, updateBids.end());
			break;
		}

		if (up == updateBids.end())
		{
			// processed all updated bids
			// copy all data from temp vector to the begin of bids
			int i = 0;
			for (CryptoVector::iterator t = m_tempBids.begin(); t != m_tempBids.end(); ++t)
				m_bids[i++] = *t;
			// rest of data should be valid in m_bids
			break;
		}

		if (ex->m_nPrice == up->m_nPrice)
		{
			if (up->m_fVolume == 0)
			{
				// new volume is 0, band was deleted, no update into temp bids vector
			}
			else
			{
				// save updated band volume into temp bids vector
				m_tempBids.push_back(*up);
			}

			++ex;
			++up;
			continue;
		}

		if (ex->m_nPrice > up->m_nPrice)
		{
			m_tempBids.push_back(*ex);
			++ex;
		}
		else
		{
			m_tempBids.push_back(*up);
			++up;
		}
	}
}

void CDomVectorImplementation::UpdateAsks(const vector<CryptoBand> &updateAsks)
{
	CryptoVector::iterator ex = m_asks.begin();						// existing asks
	CryptoVector::const_iterator up = updateAsks.begin();			// update asks
	m_tempAsks.clear();
	for (;;)
	{
		if (ex == m_asks.end())
		{
			// processed all existing asks
			// copy all data from temp vector
			m_asks = m_tempAsks;
			// copy rest data from update
			m_asks.insert(m_asks.end(), up, updateAsks.end());
			break;
		}

		if (up == updateAsks.end())
		{
			// processed all updated asks
			// copy all data from temp vector to the begin of asks
			int i = 0;
			for (CryptoVector::iterator t = m_tempAsks.begin(); t != m_tempAsks.end(); ++t)
				m_asks[i++] = *t;
			// rest of data should be valid in m_asks
			break;
		}

		if (ex->m_nPrice == up->m_nPrice)
		{
			if (up->m_fVolume == 0)
			{
				// new volume is 0, band was deleted, no update into temp asks vector
			}
			else
			{
				// save updated band volume into temp asks vector
				m_tempAsks.push_back(*up);
			}

			++ex;
			++up;
			continue;
		}

		if (ex->m_nPrice < up->m_nPrice)
		{
			m_tempAsks.push_back(*ex);
			++ex;
		}
		else
		{
			m_tempAsks.push_back(*up);
			++up;
		}
	}
}

void CDomVectorImplementation::ClearDOM()
{
	m_bids.clear();
	m_asks.clear();
}

void CDomVectorImplementation::PrintDOM()
{
	for (CryptoVector::iterator it = m_bids.begin(); it != m_bids.end(); ++it)
	{
		printf("%d\t%.6lf\n", it->m_nPrice, it->m_fVolume);
	}
	printf("\n");

	for (CryptoVector::iterator it = m_asks.begin(); it != m_asks.end(); ++it)
	{
		printf("%d\t%.6lf\n", it->m_nPrice, it->m_fVolume);
	}
	printf("\n");
}

void CDomVectorImplementation::PrintTopOfTheBook(const string &szDateTime)
{
	if (m_bids.empty() || m_asks.empty())
		return;
	CryptoVector::iterator bit = m_bids.begin();
	CryptoVector::iterator ait = m_asks.begin();
	printf("%s\t\t%d, %.6lf\t %d, %.6lf\n", szDateTime.c_str(), bit->m_nPrice, bit->m_fVolume, ait->m_nPrice, ait->m_fVolume);
}

