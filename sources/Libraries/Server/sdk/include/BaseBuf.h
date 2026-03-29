//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#include "PreAlloc.h"
#include "robject.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//
class BaseBuf :public PreAllocStru {
public:
	BaseBuf(uLong size) :PreAllocStru(size), m_size(size), m_buf(std::make_unique<char[]>(size)) {}
	uLong Size() override { return m_buf ? m_size : 0; }
	char* getbuf() { return m_buf.get(); }

private:
	std::unique_ptr<char[]> m_buf;
	uLong					m_size;
};
//
class rbuf :public BaseBuf, public robject<true> {
public:
	rbuf(uLong size) :BaseBuf(size) {}
protected:
	void Initially() { InitRCount(0); }
	void Finally() { InitRCount(1); }
	void Free() { BaseBuf::Free(); }
private:
};

#pragma pack(pop)
_DBC_END
