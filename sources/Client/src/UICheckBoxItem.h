#pragma once
#include "uigraph.h"

class CCheckBoxItem :
	public CGraph {
public:
	CCheckBoxItem(void);
	~CCheckBoxItem(void);

	virtual void Render(int x, int y);
	bool GetIsChecked() {
		return m_bChecked;
	}

	void SetIsChecked(bool isCheck) {
		m_bChecked = isCheck;
	}

	void SetTextColor(DWORD color) {
		m_dwColor = color;
	}

	DWORD GetTextColor() {
		return m_dwColor;
	}

	const char* GetCaption() {
		return m_strCaption.c_str();
	}

	void SetCaption(const char* str) {
		m_strCaption = str;
	}

private:
	bool m_bChecked;
	DWORD m_dwColor;
	std::string m_strCaption;
};
