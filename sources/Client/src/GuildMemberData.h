#pragma once

class CGuildMemberData {
public:
	CGuildMemberData(void);
	~CGuildMemberData(void);

	void SetID(DWORD dwID) {
		m_dwID = dwID;
	}

	DWORD GetID() {
		return m_dwID;
	}

	void SetName(std::string strName) {
		m_strName = strName;
	}

	std::string GetName() {
		return m_strName;
	}

	void SetMottoName(std::string strMottoName) {
		m_strMottoName = strMottoName;
	}

	std::string GetMottoName() {
		return m_strMottoName;
	}

	void SetIcon(DWORD dwIcon) {
		m_dwIcon = dwIcon;
	}

	DWORD GetIcon() {
		return m_dwIcon;
	}

	void SetJob(std::string strJob) {
		m_strJob = strJob;
	}

	std::string GetJob() {
		return m_strJob;
	}

	void SetLevel(DWORD dwLv) {
		m_dwLv = dwLv;
	}

	DWORD GetLevel() {
		return m_dwLv;
	}

	void SetOnline(bool bOnline) {
		m_bOnline = bOnline;
	}

	bool IsOnline() {
		return m_bOnline;
	}

	void SetManager(bool bMgr) {
		m_bMgr = bMgr;
	}

	bool IsManager() {
		return m_bMgr;
	}

	void SetPointer(void* pointer) {
		m_pPointer = pointer;
	}

	void* GetPointer() {
		return m_pPointer;
	}

	void SetPerm(int perm) {
		m_cPerm = perm;
	}

	unsigned int GetPerm() {
		return m_cPerm;
	}

private:
	DWORD m_dwID;
	std::string m_strName;
	std::string m_strMottoName;
	std::string m_strJob;
	DWORD m_dwLv;
	bool m_bOnline;
	bool m_bMgr;
	DWORD m_dwIcon;
	void* m_pPointer;
	unsigned int m_cPerm;
};
