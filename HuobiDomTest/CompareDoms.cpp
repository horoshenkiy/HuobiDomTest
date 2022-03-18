#include "stdafx.h"
#include "DomMapImplementation.h"
#include "DomVectorImplementation.h"

bool AreDomsEquals(const CDomMapImplementation &domMap, const CDomVectorImplementation &domVector)
{
	if (domMap.m_bids.size() != domVector.m_bids.size())
		return false;

	if (domMap.m_asks.size() != domVector.m_asks.size())
		return false;

	int b = domVector.m_bids.size()-1;
	for (CDomMapImplementation::CryptoMap::const_iterator it = domMap.m_bids.begin(); it != domMap.m_bids.end(); ++it)
	{
		const CryptoBand &cp = domVector.m_bids[b--];
		if (it->first != cp.m_nPrice || it->second != cp.m_fVolume)
			return false;
	}

	int a = 0;
	for (CDomMapImplementation::CryptoMap::const_iterator it = domMap.m_asks.begin(); it != domMap.m_asks.end(); ++it)
	{
		const CryptoBand &cp = domVector.m_asks[a++];
		if (it->first != cp.m_nPrice || it->second != cp.m_fVolume)
			return false;
	}
	return true;
}
