//=============================================================================
// FileName: ItemAttr.h
// Creater: ZhangXuedong
// Date: 2004.12.28
// Comment: CItemAttr class
//=============================================================================

#ifndef ITEMATTR_H
#define ITEMATTR_H

#include "ItemAttrType.h"

class CItemRecord;

class CItemAttr {
public:
	CItemAttr() {
		m_bInitFlag = false;
	}

	bool Init(const CItemRecord& rec);

	bool HasInit(void) {
		return m_bInitFlag;
	}

	short GetAttr(short sAttrType);
	short SetAttr(short sAttrType, short sVal);
	short AddAttr(short sAttrType, short sVal);

protected:

private:
	short m_sAttr[ITEMATTR_MAX_NUM]{};
	bool m_bInitFlag{};
};

inline short CItemAttr::GetAttr(short sAttrType) {
	if (sAttrType < 1 || sAttrType >= ITEMATTR_MAX_NUM)
		return 0;

	return m_sAttr[sAttrType];
}

// 01
inline short CItemAttr::SetAttr(short sAttrType, short sVal) {
	if (sAttrType < 1 || sAttrType >= ITEMATTR_MAX_NUM)
		return 0;

	m_sAttr[sAttrType] = sVal;
	return 1;
}

// 01
inline short CItemAttr::AddAttr(short sAttrType, short sVal) {
	if (sAttrType < 1 || sAttrType >= ITEMATTR_MAX_NUM)
		return 0;

	m_sAttr[sAttrType] += sVal;
	return 1;
}

//=============================================================================
class CItemRecord;

class CItemRecordAttr {
public:
	bool Init(const CItemRecord& itemRec);

	short GetAttr(short sAttrType, bool bMax);

protected:

private:
	short m_sAttr[ITEMATTR_MAX_NUM][2];
};

inline short CItemRecordAttr::GetAttr(short sAttrType, bool bMax) {
	if (sAttrType < 1 || sAttrType >= ITEMATTR_MAX_NUM)
		return 0;

	if (bMax)
		return m_sAttr[sAttrType][1];
	else
		return m_sAttr[sAttrType][0];
}

#endif // ITEMATTR_H
