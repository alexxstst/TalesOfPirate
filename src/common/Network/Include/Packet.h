//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2004
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2004.6
// This program is modified recently by Zhang.Dabo in 2004.11
//=================================================================
#pragma once

#include "dstring.h"		//Packet.h
#include "BaseBuf.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

class TcpCommApp;
class RPacket;

enum{em_cmdsize	=sizeof(uShort)};
//==============================================================================================
class WPacket:private rptr<rbuf>
{
	friend class RPacket;
	friend class SNDPacket;
	friend class Sender;
	friend class TcpCommApp;
	friend class RPCMGR;
	friend class OnServeCall;
public:
	WPacket(const WPacket &wpkt)
		:rptr<rbuf>(wpkt),__tca(wpkt.__tca)
		,m_offset(wpkt.m_offset),m_head(wpkt.m_head),m_wpos(wpkt.m_wpos),m_cpos(wpkt.m_cpos)//,m_appendant(wpkt.m_appendant)
	{}
	WPacket(const RPacket &rpk);
	WPacket(const TcpCommApp *tca=0);
	WPacket &operator=(rbuf *bptr);
	WPacket &operator=(const WPacket &wpk);
	WPacket &operator=(const RPacket &rpk);

	WPacket		Duplicate()const;

	bool		WriteCmd(uShort	cmd);
	bool		WriteChar(uChar ch);
	bool		WriteShort(uShort sh);
	bool		WriteLong(uLong lo);
	bool		WriteLongLong(LONG64 ll);
	bool		WriteSequence(cChar *seq,uShort len);
	bool		WriteString(cChar *str);
	bool		WriteString(std::string_view str);
	bool		WriteFloat(float fVal);

	operator bool()const			{return rptr<rbuf>::operator bool();}
	uLong		HasData()const		{return (*this)?m_wpos:0;}//00
	uLong		Size()const			{return	(*this)?(*this)->Size():0;}
	uLong		Head()const			{return m_head;}
private:
	void		WritePktLen();
	uLong		BackupSESS()const;
	void		WriteSESS(uLong	ses);
	uLong GetDataLen() const 
	{
		return em_cmdsize + m_wpos;
	}
	char const* GetDataAddr() const 
	{
		return	(*this)->getbuf() + m_offset + m_head;
	}
	char* GetDataAddr()
	{
		return	(*this)->getbuf() + m_offset + m_head;
	}
	uLong GetPktLen() const 
	{
		return m_head + em_cmdsize + m_wpos;
	}
	char const*	GetPktAddr() const
	{
		return (*this)->getbuf() + m_offset;
	}
	char* GetPktAddr()
	{
		return (*this)->getbuf() + m_offset;
	}

	TcpCommApp const* __tca;
	uLong m_head;
	volatile uLong m_offset, m_wpos, m_cpos;
	//volatile bool					m_appendant;
};
//==============================================================================================
class RPacket:private rptr<rbuf>
{
	friend class WPacket;
	friend class Receiver;
	friend class RPCMGR;
public:
	RPacket(const RPacket &rpk)
		:rptr<rbuf>(rpk),__tca(rpk.__tca)
		,m_offset(rpk.m_offset),m_head(rpk.m_head),m_rpos(rpk.m_rpos),m_len(rpk.m_len)
		,m_tickcount(rpk.m_tickcount),m_revrpos(0)
	{}
	RPacket(const WPacket &wpk);

	RPacket(const TcpCommApp *tca=0);
	RPacket &operator=(rbuf	*bptr);
	RPacket	&operator=(const RPacket &rpk);
	RPacket &operator=(const WPacket &wpk);

	uShort		ReadCmd();
	uChar		ReadChar();
	uShort		ReadShort();
	uLong		ReadLong();
	LONG64		ReadLongLong();
	char*		ReadSequence(uShort &retlen);
	cChar *		ReadString(uShort *len =0);
	float		ReadFloat();
	uint64_t	ReadAddress();

	uChar		ReverseReadChar();
	uShort		ReverseReadShort();
	uLong		ReverseReadLong();
	LONG64 ReverseReadLongLong();
	uint64_t	ReverseReadAddress();
	bool		DiscardLast(uLong len);//true-false-()

	operator bool()const				{return rptr<rbuf>::operator bool();}
	uLong		HasData()const			{return (bool(*this)&&(m_len>(m_head +em_cmdsize)))?(m_len -m_head -em_cmdsize):0;}
	//RemainData()00
	uLong		RemainData()const		{return (bool(*this)&&(m_len>(m_head +em_cmdsize +m_rpos)))?(m_len -m_head -em_cmdsize -m_rpos):0;}
	uLong		Size()const				{return	(*this)?(*this)->Size():0;}
	uLong		GetDataLen()const;
	char const*	GetDataAddr() const	
	{
		return (*this)->getbuf() + m_offset + m_head;
	}
	char* GetDataAddr() 
	{
		return (*this)->getbuf() + m_offset + m_head;
	}
	uLong		GetTickCount()const		{return m_tickcount;}
	cChar* GetBuffer()const { return	(*this)->getbuf(); }
	uLong		GetHead()const { return m_head; }

public:
	uLong		ReadSESS()const;
	void		RestoreSESS(uLong ses);
	void		ReadPktLen();
	void		SetOffset(uLong offset)	{m_offset =offset;}
	uLong		GetOffset()const		{return	m_offset;}
	void		SetPktLen(uLong len)	{m_len	=len;}
 	uLong		GetPktLen()const		{return m_len;}
	cChar	*	GetPktAddr()const		{return (*this)->getbuf()+m_offset;}
	void		SetPktPos(uLong rpos) { m_rpos = rpos; }
	void		SetPktRevPos(uLong rpos) { m_revrpos = rpos; }

	TcpCommApp const* __tca{};
	uLong m_head{};
	volatile uLong					m_offset{}, m_rpos{}, m_len{}, m_revrpos{}, m_tickcount{};
};

#pragma pack(pop)
_DBC_END
