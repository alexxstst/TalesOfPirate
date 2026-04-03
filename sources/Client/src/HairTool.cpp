#include "StdAfx.h"
#include "HairRecordStore.h"
#include "hairtool.h"
#include "HairRecord.h"

using namespace std;
//---------------------------------------------------------------------------
// class CHairName
//---------------------------------------------------------------------------
CHairName::CHairName( CHairRecord* pInfo )
: _szName( pInfo->szDataName )
{
	AddInfo( pInfo );
}

//---------------------------------------------------------------------------
// class CHairName
//---------------------------------------------------------------------------
CHairTools::~CHairTools()
{
	_Clear();
}

void CHairTools::_Clear()
{
	for( hairs::iterator it=_hairs.begin(); it!=_hairs.end(); it++ )
		delete *it;

	_hairs.clear();
}

bool CHairTools::RefreshCha( DWORD dwChaID )
{
	_Clear();
	if( dwChaID==0 || dwChaID>4 ) return false;
	dwChaID--;

	HairRecordStore::Instance()->ForEach([&](CHairRecord& hair) {
		if( hair.IsChaUse[dwChaID] )
			_AddInfo( &hair );
	});
	return true;
}

void CHairTools::_AddInfo( CHairRecord* pInfo )
{
	for( hairs::iterator it=_hairs.begin(); it!=_hairs.end(); it++ )
	{
		if( strcmp( (*it)->GetName(), pInfo->szDataName )==0 )
		{
			(*it)->AddInfo( pInfo );
			return;
		}
	}

	_hairs.push_back( new CHairName( pInfo ) );
}
