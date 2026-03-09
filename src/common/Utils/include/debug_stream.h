/*
 Using an output stream for debugging (http://www.codetools.com/useritems/debugout.asp)
 By Sven Axelsson (svenax@bredband.net)

 modified: 
 Konstantin Boukreev (konstantin@mail.primorye.ru)
 
 Created: 12.04.2001 15:51:26
 Version: 1.0.0

*/

#ifndef _debug_stream_8c834f97_cc85_440a_a62d_a744ff43227c
#define _debug_stream_8c834f97_cc85_440a_a62d_a744ff43227c

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#include <ostream>
#include <streambuf>
#include <string>

template <typename tChar, typename tTraits = std::char_traits<tTraits> >
class basic_debugbuf : 
    public std::basic_streambuf<tChar, tTraits>
{
public:

    virtual ~basic_debugbuf()
    {
    //    sync();
    }

protected:

    int_type overflow(int_type ch = traits_type::eof())
    {
        if (!traits_type::eq_int_type(traits_type::eof(), ch))
        {
//           m_buf.append(1, traits_type::to_char_type(ch));         // 1
			m_buf += traits_type::to_char_type(ch);
        }
        else
        {
            return (int_type)sync();
        }

        return traits_type::not_eof(ch);
    }

    int sync()
    {
        output_debug_string(m_buf.c_str());
        m_buf.erase();

        return 0;
    }

	void output_debug_string(const tChar *text) {}
	
	
private:

    std::basic_string<char_type, traits_type> m_buf;

};

template<>
inline void basic_debugbuf<char, std::char_traits<char> >::output_debug_string(const char *text)
{
    ::OutputDebugStringA(text);
}

template<>
inline void basic_debugbuf<wchar_t, std::char_traits<wchar_t> >::output_debug_string(const wchar_t *text)
{
    ::OutputDebugStringW(text);
}


template <class tChar, class tTraits = std::char_traits<tChar> >
class basic_dostream : 
    public std::basic_ostream<tChar, tTraits>
{

    basic_debugbuf<tChar, tTraits> m_debugbuf;

public:

    basic_dostream() : std::basic_ostream<tChar, tTraits>(&m_debugbuf) {};
    ~basic_dostream() 
	{ 
		m_debugbuf.pubsync();
	}
};

typedef basic_dostream<char>    dostream;
typedef basic_dostream<wchar_t> wdostream;


#endif //_debug_stream_8c834f97_cc85_440a_a62d_a744ff43227c


