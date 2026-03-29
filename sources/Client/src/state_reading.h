#pragma once
#include "STStateObj.h"

namespace GUI
{
	struct stSelectBox;
}

class CReadingState : public CActionState
{
public:
	CReadingState(CActor* p);

    virtual const char* GetExplain()
	{
		return "CReadingState";
	}

	virtual void FrameMove();
	virtual void BeforeNewState()
	{
		PopState();
	}

protected:
	virtual bool _Start();
	virtual void _End();
    virtual bool _IsAllowCancel()
	{
		return false;
	}

};
