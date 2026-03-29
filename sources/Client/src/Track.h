
#include "publicfun.h"

class CPointTrack
{
public:
    
    CPointTrack()
    :_fSpeed(0.0f),
     _fProgressRate(0.0f)
    {
		_fheight = 0;
    }

    void    Reset(float fX, float fY, float fZ);
    void    AddPoint(float fX, float fY, float fZ,DWORD dwTimeParam);
    void    FrameMove(DWORD dwTimeParam);
    void    SetSpeed(float fSpeed)      { _fSpeed = fSpeed;           }
    
    float   GetCurX() { return _vecCur.x; }
    float   GetCurY() { return _vecCur.y; }
    float   GetCurZ() { return _vecCur.z; }

    int     GetPointCnt()       { return (int)(_PointList.size());        }

protected:
	struct STrackPoint
	{
		float x;
		float y;
		float z;
		STrackPoint(): x(-1),y(-1),z(0)
		{
		}
	};

    STrackPoint  _vecLast;
    STrackPoint  _vecCur;
    STrackPoint  _vecNext;
    float        _fProgressRate;
    float        _fSpeed;

	float		 _fheight;//
	bool		 _bUpdateHei;

    std::list<STrackPoint> _PointList;
};

inline void CPointTrack::Reset(float fX, float fY, float fZ)
{
    _vecLast.x = fX;
    _vecLast.y = fY;
    _vecLast.z = fZ;
    _vecNext = _vecLast;
    _vecCur  = _vecLast;
    _fProgressRate = 0.0f;
    _fSpeed   = 0.3f;
}

inline void CPointTrack::AddPoint(float fX, float fY, float fZ,DWORD dwTimeParam)
{
	static DWORD	dwTime = dwTimeParam;

	_bUpdateHei = false;
	if(dwTimeParam - dwTime > 1000)
	{
		_bUpdateHei = true;
		dwTime = dwTimeParam;
	}

    if(_PointList.size() > 0)
    {
        STrackPoint last = _PointList.back();
        float fDis = GetDistance(fX, fY, fZ, last.x, last.y, last.z);
        if(fDis < 0.04f)
		{
			return; 
		}
    }else
		if((fabs(fX-GetCurX())<1.0f) &&(fabs(fY-GetCurY())<1.0f)&&(fabs(fZ-GetCurZ())<0.2f))
		{
			return;
		}

    STrackPoint p;
	if(_PointList.size() == 0)
	{
	/*	if(GetCurX()<=0 ||GetCurY()<=0)
		{
			p.x = fX; 
			p.y = fY;
			p.z = fZ;
			_PointList.push_back(p);
			return;
		}*/
		D3DXVECTOR3 vp(fX,  fY,  fZ);
		D3DXVECTOR3 vsrc(GetCurX(), GetCurY(), GetCurZ());
		vp = vp - vsrc;
		float fDis = D3DXVec3Length(&vp);

		float d = fDis/8;

		D3DXVec3Normalize(&vp,&vp);
		for (int n = 0; n < 8; n++)
		{
			const auto v = vp * d;
			D3DXVec3Add(&vsrc,&vsrc,&(v));
			p.x = vsrc.x; 
			p.y = vsrc.y;
			p.z = vsrc.z;
			_PointList.push_back(p);
		}
		_fheight =  GetCurZ();

		return;
	}


	//STrackPoint last = _PointList.back();
    //if (fabs(fZ - last.z) > 0.1f)
	//D3DXVECTOR3 vp(fX,  fY,  fZ);
	//D3DXVECTOR3 vsrc(last.x, last.y, last.z);
	//vp = vp - vsrc;
	//float fDis = D3DXVec3Length(&vp);

	//float d = fDis/2;

	//D3DXVec3Normalize(&vp,&vp);
	//for (int n = 0; n < 3; n++)
	//{
	//	D3DXVec3Add(&vsrc,&vsrc,&(vp*d));
	//	p.x = vsrc.x; 
	//	p.y = vsrc.y;
	//	p.z = vsrc.z;
	//	_PointList.push_back(p);
	//}

	
	////STrackPoint front = _PointList.front();
	if(_bUpdateHei)
	{
		if (fabs(fZ - _fheight) > 0.5f)
		{
	/*		_PointList.clear();
			D3DXVECTOR3 vp(fX,  fY,  fZ);
			D3DXVECTOR3 vsrc(front.x, front.y, front.z);
			vp = vp - vsrc;
			float fDis = D3DXVec3Length(&vp);
			fDis = fDis / (float)_PointList.size();
			D3DXVec3Normalize(&vp,&vp);
			for (int o = 0; o < (int)_PointList.size(); o++)
			{
				D3DXVec3Add(&vsrc,&vsrc,&(vp*fDis));
				p.x = vsrc.x; 
				p.y = vsrc.y;
				p.z = vsrc.z;
				_PointList.push_back(p);
			}*/

			float te = fZ - _fheight;
			int w = (int)( fabs(te) /0.5f );
			for (int o = 0; o < w; o++)
			{
				p.x = fX; 
				p.y = fY;
				p.z = _fheight + (te >0 ? 0.5f : -0.5f) * o;
				_PointList.push_back(p);
			}

			_fheight =  fZ;
			return;
		}/*else
			if(fabs(fZ - _fheight) <=0.1f)
			{
				_fheight =  fZ;
			}*/
	}
	if(fabs(fZ - _fheight) <=0.2f)
	{
		_fheight =  fZ;
	}
	p.x = fX; 
	p.y = fY;
	p.z = _fheight;

    _PointList.push_back(p);
}

inline void CPointTrack::FrameMove(DWORD dwTimeParam)
{

 	if(_fProgressRate > 1.0f)
    {
        _vecCur  = _vecNext;
        _vecLast = _vecNext;
        
        if(_PointList.size() > 0)  
        {
            _vecNext = _PointList.front();
            _PointList.pop_front();
            _fProgressRate = _fProgressRate - 1.0f;
        }
        else
        {
            _vecNext = _vecCur;
            _vecLast = _vecCur;
            _fProgressRate = 0.0f;
        }
    }
    
    _vecCur.x = _vecLast.x * (1.0f - _fProgressRate) + _vecNext.x * _fProgressRate;
	_vecCur.y = _vecLast.y * (1.0f - _fProgressRate) + _vecNext.y * _fProgressRate;
    _vecCur.z = _vecLast.z * (1.0f - _fProgressRate) + _vecNext.z * _fProgressRate;

    int nPointSize = (int)(_PointList.size());
    if(nPointSize >= 3) 
    {
        _fSpeed +=0.3f;//dwTime   * 0.0001f;
        if(_fSpeed > 1.0f) _fSpeed = 1.0f;
    }
    if(nPointSize < 3)
    {
        _fSpeed-=0.1f;//dwTime   * 0.0001f;
        if(_fSpeed < 0.002f) _fSpeed = 0.002f;
    }
    _fProgressRate+=_fSpeed; 
    

}


