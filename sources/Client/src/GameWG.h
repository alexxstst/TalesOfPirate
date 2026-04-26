#pragma once

#include <unordered_set>
#include <string>


//   add by Philip.Wu  2006-07-06
class CGameWG {
public:
	CGameWG(void);
	~CGameWG(void);

	// 
	bool RefreshModule(void);

	// 
	bool IsUseHdts(void);

	// 
	void BeginThread(void);

	// 
	void SafeTerminateThread();

private:
	// 
	std::unordered_set<std::string> m_lstModule;

	// 
	HANDLE m_hThread;

	// 
	static UINT CALLBACK Run(void* param);
};
