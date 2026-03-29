// Task.h      
#pragma once

#include "DBCCommon.h"
#include <atomic>

_DBC_BEGIN

class Task
{
public:
	Task() : m_exitFlag(false) {}
	virtual ~Task() = default;

	bool GetExitFlag() const { return m_exitFlag.load(); }
	void SetExitFlag() { m_exitFlag = true; }

	virtual long Process() = 0;
	virtual void Free() { delete this; }

private:
	std::atomic<bool> m_exitFlag;
};

_DBC_END
