#pragma once

class CHairRecord;

class CHairName {
public:
	CHairName(CHairRecord* pInfo);

	const char* GetName() {
		return _szName;
	}

	DWORD GetMax() {
		return (DWORD)_hairs.size();
	}

	CHairRecord* GetInfo(DWORD v) {
		return _hairs[v];
	}

public:
	void AddInfo(CHairRecord* pInfo) {
		_hairs.push_back(pInfo);
	}

private:
	typedef std::vector<CHairRecord*> hairs;
	hairs _hairs;
	const char* _szName;
};

class CHairTools {
public:
	~CHairTools();

	bool RefreshCha(DWORD dwChaID);

	DWORD GetHairMax() {
		return (DWORD)_hairs.size();
	}

	CHairName* GetHair(DWORD v) {
		return _hairs[v];
	}

private:
	typedef std::vector<CHairName*> hairs;
	hairs _hairs;

private:
	void _Clear();
	void _AddInfo(CHairRecord* pInfo);
};
