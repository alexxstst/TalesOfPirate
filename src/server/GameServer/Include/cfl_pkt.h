#pragma once


#ifndef _CFL_PKT_H_
#define _CFL_PKT_H_

#include "cfl_lock.h"
#include "cfl_mempool.h"


// packet container
template <class T>
class cfl_pktc : public cfl_mpelem
    {
    friend class cfl_mp<cfl_pktc>;

protected:
    cfl_pktc() : cfl_mpelem() {}

public:

    // 

    static short hdr_sz() {return T::hdr_sz();} // 
    static short pkt_len(void* dat) {return T::pkt_len(dat);} // 
    static short pkt_maxlen() {return T::pkt_maxlen();} // 

    void* pkt_buf() {return _pkt.pkt_buf();} // 
    short pkt_len() {return _pkt.pkt_len();} // 

    void clone_from(cfl_pktc<T>* pkt) {_pkt.clone(pkt->_pkt);} // deep-copy

public:

    // 

    void WriteCmd(unsigned short command) {_pkt.wcmd(command);}
    void WriteChar(char const c) {_pkt.wc(c);}
    void WriteShort(short s) {_pkt.ws(s);}
    void WriteInt(int i) {_pkt.wi(i);}
    void WriteLong(long l) {_pkt.wl(l);}
	void WriteLongLong(LONG64 l) {_pkt.wll(l);}
    void WriteSequence(char const* pBlock, int len) {_pkt.wb(pBlock, len);}
    void WriteString(char const* str) {_pkt.wstr(str);}

    unsigned short ReadCmd() {return _pkt.rcmd();}
    char	ReadChar() {return _pkt.rc();}
    short	ReadShort() {return _pkt.rs();}
    int		ReadInt() {return _pkt.ri();}
    long	ReadLong() {return _pkt.rl();}
	LONG64	ReadLongLong() {return _pkt.rll();}
    char const* ReadSequence(unsigned short& len) {return _pkt.rb(len);}
    char const* ReadString() {return _pkt.rstr();}

    long ReverseReadLong() {return _pkt.rrl();}
    short ReverseReadShort() {return _pkt.rrs();}
    char ReverseReadChar() {return _pkt.rrc();}

    void ResetOffset() {_pkt.reset_offset();}


protected:

    void on_get() {_pkt.reset();}
    void on_ret() {_pkt.reset();}

    void pkt_enc() {}; // 
    void pkt_dec() {}; // 

    // the core packet
    T _pkt;};


// struct cfl_pkt
#pragma pack(push)
#pragma pack(1)
struct cfl_pkt
    {
    enum {MYPKT_DATMAXLEN = 8 * 1024};
    struct HDR
        {
        short length;
        int ssid;};


    //  begin
    HDR hdr;
    unsigned short cmd;
    char buf[MYPKT_DATMAXLEN];
    unsigned short offset;
    unsigned short offset_r;
    //  end



    // 
    cfl_pkt() {reset();}
    void reset();

    // 
    static short hdr_sz();
    static short pkt_len(void* dat);
    static short pkt_maxlen();

    bool has_room(short len) const;
    bool has_data(short len) const;
    void clone(cfl_pkt const& pkt);

    // 
    short get_len() const;
    void set_len(short len);
    void add_len(short len);

    // 
    void* pkt_buf() const {return (void *)&hdr;}
    short pkt_len() const {return get_len();}

    // 
    void wcmd(unsigned short);
    void wc(char c);
    void ws(short s);
    void wi(int i);
    void wl(int l);
	void wll(LONG64 ll);
    void wb(char const* p, short len);
    void wstr(char const* str);

    // 
    unsigned short rcmd() const;
    char rc();
    short rs();
    int ri();
    long rl();
	LONG64 rll();
    char const* rb(unsigned short& len);
    char const* rstr();

    long rrl(); // Reverse Read Long
    short rrs(); // Reverse Read Short
    char rrc(); // Reverse Read Char

    void reset_offset() {offset = 0; offset_r = 0;}
};

#pragma pack(pop)


inline short cfl_pkt::hdr_sz() {return sizeof(HDR);}
inline short cfl_pkt::pkt_len(void* dat)
    {
    short len;
    memcpy((void *)&len, dat, sizeof len);

#ifdef USE_NBO
    return ntohs(len);
#else
    return len;
#endif
    } 
inline short cfl_pkt::pkt_maxlen() {return MYPKT_DATMAXLEN;}

inline short cfl_pkt::get_len() const
    {
#ifdef USE_NBO
    short len;
    memcpy((void *)&len, (void *)&(hdr.length), sizeof len);
    return ntohs(len);
#else
    return hdr.length;
#endif
    }

inline void cfl_pkt::set_len(short len)
    {
#ifdef USE_NBO
    hdr.length = htons(len);
#else
    hdr.length = len;
#endif            
    }

inline void cfl_pkt::add_len(short len) {set_len(get_len() + len);}

inline void cfl_pkt::reset()
    {
#ifdef USE_NBO
    hdr.length = htons(hdr_sz() + sizeof(unsigned short));
    hdr.ssid = htonl(0x80000000); // Asynchronized Packet Flag
#else
    hdr.length = hdr_sz() + sizeof(unsigned short);
    hdr.ssid = 0x80000000;
#endif

    offset = 0;
    offset_r = 0;}

inline bool cfl_pkt::has_room(short len) const
    {
    return ((offset + len ) <= MYPKT_DATMAXLEN) ? true : false;}

inline bool cfl_pkt::has_data(short len) const
    {    
    return ((offset + len) <= get_len()) ? true : false;}

inline void cfl_pkt::clone(cfl_pkt const& pkt)
    {
    if (this == &pkt) return;

    // clone...
    memcpy(pkt_buf(), pkt.pkt_buf(), pkt.pkt_len());
    offset = pkt.offset;
    offset_r = pkt.offset_r;}

inline void cfl_pkt::wcmd(unsigned short command)
    {
#ifdef USE_NBO
    cmd = htons(command);
#else
    cmd = command;
#endif    
    }

inline void cfl_pkt::wc(char c)
    {
    if (has_room(sizeof(char)))
        {
        *(char *)(buf + offset) = c;
        offset += sizeof(char);
        add_len(sizeof(char));}
    return;}

inline void cfl_pkt::ws(short s)
    {
    if (has_room(sizeof(s)))
        {
#ifdef USE_NBO
        *(short *)(buf + offset) = htons(s);
#else
        *(short *)(buf + offset) = s;
#endif
        offset += sizeof(short);
        add_len(sizeof(s));}
    
    return;}

inline void cfl_pkt::wi(int i)
    {
    if (has_room(sizeof(int)))
        {
#ifdef USE_NBO
        *(int *)(buf + offset) = htonl(i);
#else
        *(int *)(buf + offset) = i;
#endif
        offset += sizeof(int);
        add_len(sizeof(i));}

    return;}

inline void cfl_pkt::wl(int l)
    {
    if (has_room(sizeof l))
        {
#ifdef USE_NBO
        *(long *)(buf + offset) = htonl(l);
#else
        *(long *)(buf + offset) = l;
#endif
        offset += sizeof(long);
        add_len(sizeof(l));}

    return;}
inline void cfl_pkt::wll(LONG64 ll)
	{
		if (has_room(sizeof ll))
		{
#ifdef USE_NBO
			*(LONG64 *)(buf + offset) = htonl(ll);
			throw("error !don't support long64!");
#else
			*(LONG64 *)(buf + offset) = ll;
#endif
			offset += sizeof(LONG64);
			add_len(sizeof(ll));}

		return;}

inline void cfl_pkt::wb(char const* p, short len)
    {
    if (has_room(len + sizeof(short)))
        {        
        ws((short)len); // write length        
        memcpy(buf + offset, p, len); // write data

        offset += len;
        add_len(len);}

    return;}

inline void cfl_pkt::wstr(char const* str)
    { // add the '\0'
    wb(str, (short)strlen(str) + 1);}

inline unsigned short cfl_pkt::rcmd() const
{
#ifdef USE_NBO
    return ntohs(cmd);
#endif
    return cmd;
}

inline char cfl_pkt::rc()
    {
    char c;
    if (has_data(sizeof(char)))
        {
        c = *(char *)(buf + offset);
        offset += sizeof(char);
        return c;}
    else throw std::out_of_range("read char out of memory");}

inline short cfl_pkt::rs()
    {
    short s;
    if (has_data(sizeof(short)))
        {
#ifdef USE_NBO
        memcpy((void *)&s, (void *)(buf + offset), sizeof(short));
        offset += sizeof(short);
        return ntohs(s);
#else
        s = *(short *)(buf + offset);
        return s;
#endif
        }
    else throw std::out_of_range("read short out of memory");}

inline int cfl_pkt::ri()
    {
    int i;
    if (has_data(sizeof(int)))
        {
#ifdef USE_NBO
        memcpy((void *)&i, (void *)(buf + offset), sizeof(int));
        offset += sizeof(int);
        return ntohl(i);
#else
        i = *(int *)(buf + offset);
        offset += sizeof(int);
        return i;
#endif
        }
    else throw std::out_of_range("read int out of memory");}

inline long cfl_pkt::rl()
    {
    long l;
    if (has_data(sizeof(long)))
        {
#ifdef USE_NBO
        memcpy((void *)&l, (void *)(buf + offset), sizeof(long));
        offset += sizeof(long);
        return ntohl(l);
#else
        l = *(long *)(buf + offset);
        offset += sizeof(long);
        return l;
#endif
        }
    else throw std::out_of_range("read long out of memory");}

	inline LONG64 cfl_pkt::rll()
	{
		LONG64 ll;
		if (has_data(sizeof(long)))
		{
#ifdef USE_NBO
			memcpy((void *)&ll, (void *)(buf + offset), sizeof(LONG64));
			offset += sizeof(LONG64);
			throw std::out_of_range("read long long out of memory");
			return ntohl(ll);
#else
			ll = *(LONG64 *)(buf + offset);
			offset += sizeof(LONG64);
			return ll;
#endif
		}
		else throw std::out_of_range("read long long out of memory");}

inline long cfl_pkt::rrl()
    {
    if (offset_r == 0 || offset_r > get_len())
        offset_r = get_len() - hdr_sz() - sizeof(unsigned short);

    long l;
    offset_r -= sizeof(long);
#ifdef USE_NBO
    memcpy((void *)&l, (void *)(buf + offset_r), sizeof(long));
    return ntohl(l);
#else
    l = *(long *)(buf + offset_r);
    return l;
#endif
    }

inline short cfl_pkt::rrs()
    {
    if (offset_r == 0 || offset_r > get_len())
        offset_r = get_len() - hdr_sz() - sizeof(unsigned short);

    short s;
    offset_r -= sizeof(short);
#ifdef USE_NBO
    memcpy((void *)&s, (void *)(buf + offset_r), sizeof(short));
    return ntohs(s);
#else
    s = *(short *)(buf + offset_r);
    return s;
#endif
    }

inline char cfl_pkt::rrc()
    {
    if (offset_r == 0 || offset_r > get_len())
        offset_r = get_len() - hdr_sz() - sizeof(unsigned short);

    offset_r -= sizeof(char);
    char c = *(char *)(buf + offset_r);
    return c;
    }

inline char const* cfl_pkt::rb(unsigned short& len)
    {
    len = rs();
    if (has_data(len))
        {
        char const* p = buf + offset;
        offset += len;
        return p;}
    else throw std::out_of_range("read block out of memory");}

inline char const* cfl_pkt::rstr()
    {
    unsigned short len;
    return (char const *)rb(len);}



typedef cfl_pktc<cfl_pkt> Packet;

#endif // _CFL_PKT_H_
