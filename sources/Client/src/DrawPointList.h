#pragma once

class CDrawPointList
{
public:
	CDrawPointList();
	~CDrawPointList();

public:
	void Clear()					{ points.clear();		}
	void Add( D3DXVECTOR3 p, DWORD c=0xff00ffff, float h=0.0 );
	void Add( int x, int y, DWORD c=0xff00ffff, float h=0.0 );
	void Reader();
	void SetIsEnabled( bool v )     { _bIsEnabled = v;		}
	bool GetIsEnabled()				{ return _bIsEnabled;	}

	void				OnLostDevice();
	void				OnResetDevice();

	void SetMaxCount( unsigned int v )	{ _nMaxCount = v;	}

private:
	struct stPoint
	{
		stPoint( D3DXVECTOR3 p, DWORD c ) : pos(p), color(c) {}
		DWORD			color;
		D3DXVECTOR3		pos;
	};

	typedef std::vector<stPoint> vpoint;
	vpoint  points;

	unsigned int		_nMaxCount;
	bool				_bIsEnabled;

};
