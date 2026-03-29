
#include "db3.h"
#include <sstream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sqlext.h>

using namespace std;

const int TDt::FULL       =  1;
const int TDt::EUROPEAN   =  2;
const int TDt::MMDDYYYY   =  3;
const int TDt::MMMDDD     =  4;
const int TDt::DDDW       =  5;
const int TDt::YYYYMMDD   =  6;
const int TDt::CRWFORMULA =  7;
const int TDt::LETTER     =  8;
const int TDt::ODBC       =  9;
const int TDt::MMMYYYY    = 10;
const int TDt::DAY        = 11;
const int TDt::MONTH      = 12;
const int TDt::ORACLE     = 13;
const int TDt::DDMMMYY    = 14;
const int TDt::DD         = 15;
const int TDt::MM         = 16;
const int TDt::YYYY       = 17;
const int TDt::DTH        = 18;
const int TDt::D          = 19;
const int TDt::M          = 20;


int TDt::m_nDisplayFormat = TDt::MMDDYYYY;

// Constructors
TDt::TDt() 
{ // Set to today
    Set();
    SetFormat( TDt::MMDDYYYY );
}

TDt::TDt( long j ) 
{
    Set( j );
    SetFormat( TDt::MMDDYYYY );
}

TDt::TDt( int m, int d, int y ) 
{
    Set( m, d, y );
    SetFormat( TDt::MMDDYYYY );
}

TDt::TDt( const char *lpszDate ) 
{
    Set( lpszDate );
    SetFormat( TDt::MMDDYYYY );
}

TDt::TDt( TDt& dt ) 
{
    Set( dt );
    SetFormat( TDt::MMDDYYYY );
}

//
// Magic conversion routines
//
long TDt::Julian()
{
    return GetJulian( m_month, m_day, m_year );
}

//
// Assignment operators
//
TDt& TDt::operator = ( const TDt& dt ) 
{
    return Set( dt );
}

TDt& TDt::operator = ( const char* p ) 
{
    return Set( p );
}

TDt& TDt::operator = ( long l ) 
{
    return Set( l );
}

//
// Incremental operators
//
TDt& TDt::operator += ( int i ) 
{
    long j = GetJulian( m_month, m_day, m_year );
    j += i;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt& TDt::operator -= ( int i ) 
{
    long j = GetJulian( m_month, m_day, m_year );
    j -= i;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt& TDt::operator ++() 
{
    long j = GetJulian( m_month, m_day, m_year );
    j++;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt TDt::operator ++( int ) 
{
    long j = GetJulian( m_month, m_day, m_year );
    j++;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt& TDt::operator --() 
{
    long j = GetJulian( m_month, m_day, m_year );
    j--;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt TDt::operator --( int ) 
{
    long j = GetJulian( m_month, m_day, m_year );
    j--;
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

//
// comparison operators
//
bool operator <( long l, TDt& dt2 ) 
{
    return l < dt2.Julian();
}

bool operator <= ( long l, TDt& dt2 ) 
{
    return( l == dt2.Julian() ) ||( l < dt2.Julian() );
}

bool operator >( long l, TDt& dt2 ) 
{
    return( l > dt2.Julian() );
}

bool operator >= ( long l, TDt& dt2 ) 
{
    return( l == dt2.Julian() ) ||( l > dt2.Julian() );
}

bool operator == ( long l, TDt& dt2 ) 
{
    return l == dt2.Julian();
}

bool operator != ( long l, TDt& dt2 ) 
{
    return l != dt2.Julian();
}

//
// Ostream operations
//
/*
ostream& operator <<( ostream &os, TDt& dt ) 
{
dt.Format();
os << dt.m_dsDate;
return os;
}

ostream& operator <<( ostream &os, const TDt& dt ) 
{
TDt& ndt = const_cast<TDt&>( dt );
ndt.Format();
os << ndt.m_dsDate;
return os;
}
*/

//
// Format routine
//
std::string TDt::Format( const char* fs )
{
    string s;
    char ach[128];
    tm time;
    memset( &time, 0, sizeof( tm ) );
    time.tm_year = m_year - 1900;
    time.tm_mon  = m_month - 1; // jan==0
    time.tm_mday = m_day;

    //#pragma MESSAGE( Add day of week week of year etc to this )

    strftime( ach, sizeof( ach ), fs, &time );
    ach[sizeof( ach )-1] = 0; // insurance
    s = ach;

    // add st nd rd th where appropriate
    // change case also?

    return s.c_str();
}

void TDt::Format(  char* ach, int size, int a_nDisplayFormat ) 
{
    string s = Format( a_nDisplayFormat );
    strncpy( ach, s.c_str(), size-1 );
    ach[size-1] = 0; // buffer safety
}

std::string TDt::Format(  int a_nDisplayFormat ) 
{
    string s;
    //
    // this stuff needs to be moved into a resource area( bug )
    //
    static const char *DayName[] = {"Sunday", "Monday",
        "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
    const char *MonthName[] = {"January","February","March","April","May",
        "June","July","August","September","October","November","December"};

    int nDisplayFormat; 
    if ( a_nDisplayFormat == -1 )
        nDisplayFormat = m_nDisplayFormat;
    else
        nDisplayFormat = a_nDisplayFormat;

    switch ( nDisplayFormat ) {
   case TDt::DD:
       {
           ostringstream os;
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_day;
           s = os.str();
       }
       break;

   case TDt::D:
       {
           ostringstream os;
           os << dec << m_day;
           s = os.str();
       }
       break;

   case TDt::MM:
       {
           ostringstream os;
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_month;
           s = os.str();
       }
       break;

   case TDt::M:
       {
           ostringstream os;
           os << dec << m_month;
           s = os.str();
       }
       break;

   case TDt::YYYY:
       {
           ostringstream os;
           os << dec << m_year;
           s = os.str();
       }
       break;

   case TDt::DTH:
       s = Format( TDt::D );
       s += GetNumberSuffix( m_day );
       break;

   case TDt::DAY:
       if ( ! IsNull() )
           s = DayName[DOW()];
       break;

   case TDt::MONTH:
       if ( ! IsNull() )
           s = MonthName[Month()-1];
       break;

   case TDt::FULL: // Monday, January 2nd, 1998
       if ( ! IsNull() )
       {
           string csSuffix = GetNumberSuffix( Day() );
           string csDayName = DayName[DOW()];
           string csMonthName = MonthName[Month()-1];
           s = csDayName;
           s += ", ";
           s += csMonthName;
           s += " ";
           s += Format( TDt::D );
           s += csSuffix;
           s += ", ";
           s += Format( TDt::YYYY );
       }
       break;

   case TDt::LETTER: // March 23rd, 2001
       if ( ! IsNull() )
       {
           string csSuffix = GetNumberSuffix( Day() );
           string csMonthName = MonthName[Month()-1];
           s += csMonthName;
           s += " ";
           s += Format( TDt::D );
           s += csSuffix;
           s += ", ";
           s += Format( TDt::YYYY );
       }
       break;

   case TDt::EUROPEAN: // 23 March, 2001
       if ( ! IsNull() )
       {   
           string csMonthName = MonthName[Month()-1];
           s += Format( TDt::D );
           s += " ";
           s += csMonthName;
           s += " ";
           s += Format( TDt::YYYY );
       }
       break;

   case TDt::MMMYYYY: // March 1999
       if ( ! IsNull() )
       {
           string csMonthName = MonthName[Month()-1];
           s += csMonthName;
           s += " ";
           s += Format( TDt::YYYY );
       }
       break;
   case TDt::ORACLE: // 02-JUL-2001
       if ( ! IsNull() )
       {
           string csMonthName = MonthName[Month()-1];
           csMonthName.erase( 3, 50 );
           s += Format( TDt::DD );
           s += "-";
           s += csMonthName;
           s += "-";
           s += Format( TDt::YYYY );
       }
       break;

   case TDt::ODBC:
       if ( ! IsNull() )
       {
           ostringstream os;
           os << "{d'";
           os << dec << setfill( '0' ) << setw( 4 ) << setiosflags( ios::right ) << m_year;
           os << "-";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_month;
           os << "-";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_day;
           os << "'}";
           s = os.str();
       }
       break;

   case TDt::CRWFORMULA:
       if ( ! IsNull() )
       {
           ostringstream os;
           os << "date( ";
           os << dec << setfill( '0' ) << setw( 4 ) << setiosflags( ios::right ) << m_year;
           os << ",";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_month;
           os << ",";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_day;
           os << " )";
           s = os.str();
       }
       break;

   case TDt::YYYYMMDD:
       if ( ! IsNull() )
       {
           ostringstream os;
           os << dec << setfill( '0' ) << setw( 4 ) << setiosflags( ios::right ) << m_year;
           os << "-";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_month;
           os << "-";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_day;
           s = os.str();
       }
       break;

   case TDt::MMDDYYYY:
       if ( ! IsNull() )
       {
           ostringstream os;
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_month;
           os << "-";
           os << dec << setfill( '0' ) << setw( 2 ) << setiosflags( ios::right ) << m_day;
           os << "-";
           os << dec << setfill( '0' ) << setw( 4 ) << setiosflags( ios::right ) << m_year;
           s = os.str();
       }
       break;
   default:
       s = "??-??-????";
    }

    return s.c_str(); // alive as long as the object is
}

void TDt::SetFormat( int format ) 
{
    m_nDisplayFormat = format;
}

//
// Assignment methods
//
TDt& TDt::Set()  // Get today's date
{
#if _MSC_VER < 1100
    /*
    time_t osBinaryTime;     // C run-time time( defined in <time.h> )
    time( &osBinaryTime ) ;  // Get the current time from the
    // operating system.
    CTime time3( osBinaryTime );  // CTime from C run-time time
    return Set( osBinaryTime.Format( "%m/%d/%y" ) );
    */
    dosdate_t ddt;
    _dos_getdate( &ddt );
    return Set( ddt.month, ddt.day, ddt.year );

#else
    SYSTEMTIME st;
    GetLocalTime( &st );
    return Set( st.wMonth, st.wDay, st.wYear );
#endif
}

// Possible formats--
//   mm/dd/yy
//   mm/dd/yyyy
//   mm/d/yy
//   m/d/yy
//   mddyy
//   mmddyy
//   mmddyyyy
//     *also with dashes "-"
TDt& TDt::Set( const char* a_p ) 
{
    if ( a_p == NULL || a_p[0] == 0 ) // null or zero string
        return Set( 0L ); // default to null

    int len = (int)strlen( a_p );

    // check for ODBC timestamp 'yyyy-mm-dd' or 'yyyy-mm-dd 00:00:00'
    //if ( len == 10 )
    if ( a_p[4] == '-' && a_p[7] == '-' )
        return Set( atoi( &( a_p[5] ) ), atoi( &( a_p[8] ) ), atoi( a_p ) );

    // test if the user entered mmddyy
    bool bIsNumeric = TRUE;
    for ( int n=0; n < len; n++ )
        if ( a_p[n] < '0' || a_p[n] > '9' )
        {
            bIsNumeric = false;
            break;
        }

        char p[16];
        strncpy( p, a_p, sizeof( p ) );

        if ( bIsNumeric &&( len == 6 || len == 8 ) ) // 'mm-dd-yy' or 'mm-dd-yyyy'
        {
            int Year = atoi( &( p[6] ) );
            p[5] = 0;
            int Day = atoi( &( p[3] ) );
            p[2] = 0;
            int Month = atoi( p );
            return Set( Month, Day, Get4DigitYear( Year ) );
        }

		for (int n = 0; n < len; n++) {
			if (p[n] == '-') {
				p[n] = '/';
			}
		}

        int month = atoi( p );
        if ( month == 0 || month < 1 || month > 12 ) // somethings wrong
            return Set(); // default to today

        char* i = strchr( p, '/' );
        if ( i == NULL )
            return Set();

        int day = atoi( ++i );
        if ( day < 1 || day > 31 ) 
        {
            Set();
            return Set( month, Day(), Year() ); // use the month arg
        }

        i = strchr( i, '/' );
        if ( i == NULL )
            return Set();

        int year = atoi( ++i );
        //if ( year == 0 ) 
        //{
        //   Set();
        //   return Set( month, day, Year() );
        //}

        return Set( month, day, Get4DigitYear( year ) );
}

/*
#ifndef WIN32
TDt& TDt::Set( const dosdate_t& ddt ) 
{
return Set( ddt.month, ddt.day, ddt.year );
}
#endif
*/

// Convert mdy into a number that is the days since 1/1/0001 a.d.
TDt& TDt::Set( int am, int ad, int ay ) 
{
    m_month = am;
    m_day = ad;
    m_year = ay;
    if ( m_month && m_day ) // non zero
        Get4DigitYear( ay );

    return *this;
}

TDt& TDt::Set( long j ) 
{
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt& TDt::Set( TDt& dt ) 
{
    m_month = dt.Month();
    m_day = dt.Day();
    m_year = dt.Year();
    return *this;
}

TDt& TDt::Set( const TDt& dt ) 
{
#if _MSC_VER < 1100
    TDt& ndt = ( TDt& )( dt );
    m_month = ndt.Month();
    m_day = ndt.Day();
    m_year = ndt.Year();
    return *this;
#else
    TDt& ndt = const_cast<TDt&>( dt );
    m_month = ndt.Month();
    m_day = ndt.Day();
    m_year = ndt.Year();
    return *this;
#endif
}

//
//  Miscellaneous Routines
//
int TDt::IsLeap() 
{
    return( ( m_year >= 1582 ) ?
        ( m_year % 4 == 0  &&  m_year % 100 != 0  ||  m_year % 400 == 0 ):
    ( m_year % 4 == 0 ) );
}

int TDt::Day()  
{
    return m_day;
}

int TDt::Month()  
{
    return m_month;
}

int TDt::Year()  
{
    return m_year;
}

TDt& TDt::SetBOM() 
{
    Set( Month(), 1, Year() );
    return *this;
}

TDt& TDt::SetEOM() 
{
    AddMonths( 1 );
    SetBOM();
    ( *this )--;
    return *this;
}

TDt& TDt::SetBOY() 
{
    Set( 1, 1, Year() );
    return *this;
}

TDt& TDt::SetEOY() 
{
    Set( 12, 31, Year() );
    return *this;
}

// This is not an accurate idea( ????? )
int TDt::YYMM()  
{      // integer yymm ( 9201 )
    int nY = Year();
    if ( nY > 1999 ) {
        nY -= 2000;
    } else if ( nY > 1900 ) {
        nY -= 1900;
    }
    return( nY * 100 ) + Month();
}

int TDt::DOY() 
{
    TDt temp( 1, 1, Year() );

    return( int )( Julian() - temp.Julian() + 1 );
}

int TDt::DaysInMonth() 
{
    static int GauDays[] = { 31, 28, 31, 30,
        31, 30, 31, 31, 30, 31, 30, 31 };
    return GauDays[Month()-1] +( Month()==2 && IsLeap() );
}

int TDt::FirstDOM() 
{
    TDt dt( Month(), 1, Year() );
    int i = dt.DOW() + 1;
    return i;
}

int TDt::DOW() 
{
    return( int )( ( Julian() % 7 ) ); // Sun == 0 .. Sat == 6
}

// 3rd Wednesday etc...
int TDt::DOM()  
{
    return( ( Day() - 1 ) / 7 ) + 1;
}

int TDt::WOM() 
{
    // Abs day includes the days from previous month that fills up
    // the begin. of the week.
    int nAbsDay = Day() + FirstDOM()-1;
    return( nAbsDay-( DOW() ) )/7 + 1;
}

int TDt::WOY() 
{
    TDt dt( 1, 1, Year() );
    return( int )( (( Julian() - dt.Julian() + 1 ) / 7 ) + 1 );
}

TDt& TDt::AddWeeks( int nCount ) 
{
    long j = GetJulian( m_month, m_day, m_year );
    j += ( nCount * 7 );
    GetMDY( m_month, m_day, m_year, j );
    return *this;
}

TDt& TDt::AddMonths( int nCount ) 
{
    m_month += nCount;

    while ( m_month < 1 ) {
        m_month += 12;
        m_year--;
    }

    while ( m_month > 12 ) {
        m_month -= 12;
        m_year++;
    }
    return *this;
}

// Possible bug on leap year 2-29-??
TDt& TDt::AddYears( int nCount ) 
{
    int m = Month();
    int d = Day();
    int y = Year() + nCount;
    Set( m, d, y );
    return *this;
}

std::string TDt::DayName() 
{
    return Format( TDt::DAY );
}

std::string TDt::MonthName() 
{
    return Format( TDt::MONTH );
}

// d1 - d2 = result
int TDt::DifferenceInYears( long d1, long d2 ) 
{
    TDt dt1 = d1;
    TDt dt2 = d2;
    return dt1.Year() - dt2.Year();
}

// d1 - d2 = result
int TDt::DifferenceInMonths( long d1, long d2 ) 
{
    TDt dt1 = d1;
    TDt dt2 = d2;
    double dbl1 = ( ( dt1.Year() * 12 ) + dt1.Month() );
    double dbl2 = ( ( dt2.Year() * 12 ) + dt2.Month() );
    double dbl = dbl1 - dbl2;
    int n = ( int )dbl;
    return n;
}

int TDt::Age() 
{

    if ( Julian() == 0 )
        return 0;

    TDt dtToday;
    int nAge = ( int )( dtToday.Julian() - Julian() );
    nAge = nAge / 365;
    return nAge;
}

void TDt::GetMDY( int& nm, int &nd, int& ny, long jul )
{
    if ( jul == 0 )
    {
        nm = 0;
        nd = 0;
        ny = 0;
        return;
    }
    double m, d, y, dayno;
    dayno = jul;
    dayno += 443;
    y = floor( ( dayno - 122.1 ) / 365.25 );
    m = floor( ( dayno - floor( 365.25 * y ) ) / 30.6001 );
    d = dayno - floor( 365.25 * y ) - floor( 30.6001 * m );
    m = ( m < 14 ) ? m - 1 : m - 13;
    y = ( m < 3 ) ? y + 1 : y;

    nm = ( int )m; // verify this; use new casting methods
    nd = ( int )d;
    ny = ( int )y;
}

long TDt::GetJulian( int am, int ad, int ay )
{
    double m, y;
    long jul;

    if ( am == 0 && ad == 0 && ay == 0 )
        return 0L;

    y = Get4DigitYear( ay );

    if ( am > 2 ) {
        m = am + 1.0;
        y = ay;
    } else {
        m = am + 13.0;
        y = ay - 1;
    }
    jul = ( long )( floor( 365.25 *( y ) ) + floor( 30.6001 * m ) + ad );
    jul -= 443;

    return jul;
}

int TDt::Get4DigitYear( int yy )
{
    // Use the Oracle convention breaking on 2049/1950
    if ( yy >= 0 && yy < 50 ) {
        yy += 2000;
    } else if ( yy >= 50 && yy <= 99 ) {
        yy += 1900;
    }
    return yy;
}

bool TDt::IsNull()
{
    return m_month == 0 && m_day == 0 && m_year == 0;
}

// 1st 2nd 3rd 4th 20th 21st 22nd 23rd 24th 30th 31st
string TDt::GetNumberSuffix( int day )
{
    static string s;

    if ( day == 1 || day == 21 || day == 31 )
        s = "st";
    else if ( day == 2 || day == 22 )
        s = "nd";
    else if ( day == 3 || day == 23 )
        s = "rd";
    else
        s = "th";
    return s.c_str();
}


//////////////////////////////////////////////////////////////////////
// CSQLBase Class
//////////////////////////////////////////////////////////////////////

const char CSQLBase::STRING         = 1;
const char CSQLBase::JULIAN_DATE    = 2;
const char CSQLBase::ODBC_DATE      = 3;
const char CSQLBase::ORACLE_DATE    = 4;
const char CSQLBase::DATE_YYYYMMDD  = 5;
const char CSQLBase::DATE_MMDDYYYY  = 6;
const char CSQLBase::DOUBLE         = 7;
const char CSQLBase::INTEGER        = 8;
const char CSQLBase::LONG           = 9;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLBase::CSQLBase()
{
    m_bConvertQuotes = true;
}

CSQLBase::~CSQLBase()
{
}

//////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////

void CSQLBase::SetTable( const char* tableName )
{
    _table = tableName;
}

CSQLBase& CSQLBase::operator= ( const char* tableName )
{
    _table = tableName;
    return *this;
}


void CSQLBase::ResetContent()
{
    _table = "";
    _stmt  = "";
}


bool CSQLBase::ConvertQuotes( string & s )
{
    if ( ! m_bConvertQuotes )
        return false;

    if ( s.find( '\'' ) == s.npos )
        return false;

    // [20011007]
    string substr1 = "'";
    string substr2 = "''";  // odbc accepts two single quotes as one
    for (int p = (int)s.find( "'" ); p != s.npos; p = (int)s.find( "'", p ) )
    {
        s.replace( p, 1, "''" );
        p += 2;
    }
    return true;
}

std::string CSQLBase::GetStatement()
{
    return _stmt;
}

void CSQLBase::SetStmt( const char *a_szStmt )
{
    _stmt = a_szStmt;
}

CSQLBaseWhere::CSQLBaseWhere() : CSQLBase()
{
}

CSQLBaseWhere::~CSQLBaseWhere()
{
}

std::string CSQLBaseWhere::GetStatement( bool includeWhereLiteral )
{
    std::string temp = _stmt;
    if ( _where.length() > 0 )
    {
        if ( includeWhereLiteral )
            temp += " where ";
        temp += _where;
    }
    if ( _orderby.length() > 0 )
    {
        temp += " order by ";
        temp += _orderby;
    }
    return temp;
}

void CSQLBaseWhere::ResetContent()
{
    _where = "";
    CSQLBase::ResetContent();
}

void CSQLBaseWhere::SetWhereParenthesis()
{
    if ( _where == "" )
        return;

    string s = _where;
    _where = "(";
    _where += s;
    _where += ")";
}

void CSQLBaseWhere::SetOrderBy( const char* a_szOrderBy )
{
    _orderby = a_szOrderBy;
}

void CSQLBaseWhere::SetWhere(const char * a_szWhere)
{
    if ( _where != "" )
        _where += " and ";
    _where += a_szWhere;
}

void CSQLBaseWhere::SetWhereOr(const char * a_szWhere)
{
    if ( _where != "" )
        _where += " or ";
    _where += a_szWhere;
}

void CSQLBaseWhere::SetWhere( const char* columnName, int value )
{
    if ( _where != "" )
        _where += " and ";
    _where += columnName;
    _where += "=";

    /* char ach[32]; wsprintf( ach, "%d", value ); _where += ach; */
    ostringstream ss;
    ss << fixed << setprecision(2) << value;
    _where += ss.str(); // unfinished: test this
}

void CSQLBaseWhere::SetWhere( const char* columnName, const std::string& value, char a_DataType, bool a_bUseLike, bool a_bUseOr )
{
    SetWhere( columnName, value.c_str(), a_DataType, a_bUseLike, a_bUseOr );
}

void CSQLBaseWhere::SetWhere( const char* columnName, long value, char a_DataType )
{
    //assert( a_DataType == CSQLBaseWhere::LONG || a_DataType == CSQLBaseWhere::ODBC_DATE || a_DataType == CSQLBaseWhere::JULIAN_DATE);
    if ( _where != "" )
        _where += " and ";
    _where += columnName;
    _where += "=";

    if ( a_DataType == CSQLBaseWhere::LONG )
    {
        /* char ach[32]; wsprintf( ach, "%ld", value ); _where += ach; */
        ostringstream ss;
        ss << dec << value;
        _where += ss.str(); // unfinished: test this
    }
    else if ( a_DataType == CSQLBaseWhere::ODBC_DATE 
        || a_DataType == CSQLBaseWhere::JULIAN_DATE )
    {
        TDt dt( value );
        string cs = dt.Format( TDt::ODBC );
        _where += cs;
    }
}

void CSQLBaseWhere::SetWhere( const char* columnName, double value )
{
    if ( _where != "" )
        _where += " and ";
    _where += columnName;
    _where += "=";

    /* char ach[32]; wsprintf( ach, "%.2f", value ); _where += ach; */
    ostringstream ss;
    ss << fixed << setprecision(2) << value;
    _where += ss.str(); // unfinished: test this
}

void CSQLBaseWhere::SetWhere( const char* columnName, bool isValidTime, int month, int day, int year )
{
    if ( _where != "" )
        _where += " and ";

    string cs;
    if ( ! isValidTime )
    {
        cs = "isnull(";
        cs += columnName;
        cs += ")";
    }
    else
    {
        TDt dt( month, day, year );
        cs = columnName;
        cs += "=";
        cs += dt.Format( TDt::ODBC );
    }

    _where += cs;
}

void CSQLBaseWhere::SetWhere( const char* columnName, const char* a_value, 
                             char a_DataType, bool a_bUseLike, bool a_bUseOr )
{
    if ( _where != "" )
        _where += (a_bUseOr)?" or ":" and ";

    // Test for NULL immediately because it has to have 
    // a different syntax
    string value = a_value;
    if ( value == "" || value == "NULL" || value == "null" )
    {
        _where += "isnull(";
        _where += columnName;
        _where += ")";
        return;
    }

    // add the column name to the conditional
    _where += columnName;

    // handle a wildcard match
    if ( a_bUseLike && a_DataType == CSQLBaseWhere::STRING )
        _where += " like ";
    else
        _where += "=";

    //
    // Add the remaining data to the right side of the 
    // comparison.  Check if it's an ODBC date string 
    // and process it as is.
    if ( value[0] == '{' && value[1] == 'd' && value[2] == '\'' ) 
    {  
        _where += value; // {d'1972-01-15'} etc.
        return;
    }

    if ( a_DataType == CSQLBaseWhere::STRING )
    {
        ConvertQuotes( value );
        _where += "'";
        _where += value;
        _where += "'";
        return;
    }

    if ( a_DataType == CSQLBaseWhere::ORACLE_DATE )
    {
        TDt dt( value.c_str() );
        string cs = dt.Format( TDt::ORACLE );
        _where += "'";
        _where += cs;
        _where += "'";
        return;
    }

    if ( a_DataType == CSQLBaseWhere::ODBC_DATE )
    {
        TDt dt( value.c_str() );
        string cs = dt.Format( TDt::ODBC );
        _where += cs;
        return;
    }

    _where += value;
}

CSQLDatabase::CSQLDatabase()
:
_isOpen( false ),
_rc( SQL_SUCCESS ),
_henv( 0 ),
_hdbc( 0 ),
_hstmt(0),
_autoCommit( true )
{
    //[2003-09-06] remove connection code and move below
}

CSQLDatabase::~CSQLDatabase()
{
    Close();
}

string& CSQLDatabase::dbmsName()
{
    if ( _dbmsName != "" )
        return _dbmsName;

    char         InfoValuePtr[64];
    SQLSMALLINT  BufferLength = sizeof( InfoValuePtr );
    SQLSMALLINT  StringLengthPtr;

    _rc = SQLGetInfo( _hdbc, SQL_DBMS_NAME, InfoValuePtr,
        BufferLength, &StringLengthPtr );

    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
        ThrowError( SQL_HANDLE_DBC );

    _dbmsName = (char*)InfoValuePtr;
    return _dbmsName;
}

bool CSQLDatabase::Open( const char* dsn, const char* user, const char* password ) 
{ 
    _dbmsName  = ""; // reset at each open

    return this->SQLConnect( dsn, user, password ); 
}

bool CSQLDatabase::Open(char const* conn_str)
{
    _dbmsName = "";

    return this->SQLConnect(conn_str);
}


// throws on error
bool CSQLDatabase::OpenEx( const char* dsn, const char* user, const char* password ) 
{ 
    _dbmsName  = ""; // reset at each open

    if ( ! Open( dsn, user, password ) )
        ThrowError( SQL_HANDLE_DBC );

    return true;
}

bool CSQLDatabase::Close()
{
    if ( ! _isOpen ) //[2002-04-20] courtesy of bt(2002/01/30); thanks
        return false;

    if ( _hdbc )
    {
        _rc = ::SQLDisconnect( _hdbc );
        if ( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) 
            _rc = ::SQLFreeHandle( SQL_HANDLE_DBC, _hdbc );
        _hdbc = NULL; //[2003-09-06]
    }
    if ( _henv ) {
        _rc = ::SQLFreeHandle( SQL_HANDLE_ENV, _henv );
        _henv = NULL; //[2003-09-06]
    }

    _isOpen = false;
    return true;
}

bool CSQLDatabase::SQLConnect( const char * a_szDSN, 
                              const char* a_szUserName, const char* a_szPassword )
{
    //[2003-09-06] This was in the constructor
    // see: 
    //     http://www.codeguru.com/mfc/comments/44221.shtml 
    // --for details
    ////////////////// start former constructor code ////////////////// 
    _isOpen     = false;
    _rc         = SQL_SUCCESS;
    _henv       = 0;
    _hdbc       = 0;
    _autoCommit = true;

    // Allocate environment handle 
    _rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    _rc = SQLSetEnvAttr( _henv, SQL_ATTR_ODBC_VERSION,( void* )SQL_OV_ODBC3, 0 ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    _rc = SQLAllocHandle( SQL_HANDLE_DBC, _henv, &_hdbc ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    _rc = SQLSetConnectAttr( _hdbc, SQL_LOGIN_TIMEOUT,( void* )7, 0 ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    //////////////////// end former constructor code ////////////////// 

    _rc = ::SQLConnect( _hdbc,( SQLCHAR* )a_szDSN, SQL_NTS, 
        ( SQLCHAR* )a_szUserName, SQL_NTS,( SQLCHAR* )a_szPassword, SQL_NTS );
    _isOpen = ( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO );
    return _isOpen;
}

bool CSQLDatabase::SQLConnect(char const* conn_instr)
{
    char conn_outstr[2048];
    SQLSMALLINT outstr_len;

    _isOpen     = false;
    _rc         = SQL_SUCCESS;
    _henv       = 0;
    _hdbc       = 0;
    _autoCommit = true;

    // Allocate environment handle 
    _rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    _rc = SQLSetEnvAttr( _henv, SQL_ATTR_ODBC_VERSION,( void* )SQL_OV_ODBC3, 0 ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 
    _rc = SQLAllocHandle( SQL_HANDLE_DBC, _henv, &_hdbc ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false; 

    _rc = SQLSetConnectAttr( _hdbc, SQL_LOGIN_TIMEOUT,( void* )7, 0 ); 
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) ) 
        return false;

    //////////////////// end former constructor code ////////////////// 
    _rc = ::SQLDriverConnect(_hdbc, NULL, (SQLCHAR *)conn_instr, SQL_NTS,
        (SQLCHAR *)conn_outstr, SQL_NTS, &outstr_len, SQL_DRIVER_NOPROMPT);
    
    _isOpen = ( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO );
    return _isOpen;
}

bool CSQLDatabase::ExecuteSQL( const char* a_szStmt )
{
    _rc = ::SQLAllocHandle( SQL_HANDLE_STMT, _hdbc, &_hstmt );
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
        ThrowError( SQL_HANDLE_DBC );

	_rc = SQLSetStmtOption(_hstmt, SQL_QUERY_TIMEOUT, 30);

    // returns: SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, 
    // SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, or SQL_INVALID_HANDLE   
    _rc = ::SQLExecDirect( _hstmt,( unsigned char* )a_szStmt, SQL_NTS );
    if ( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO )
    {
        // COMMIT the transaction
        if ( _autoCommit )
        {
            _rc = ::SQLEndTran( SQL_HANDLE_DBC, _hdbc, SQL_COMMIT );
            if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
                ThrowError( SQL_HANDLE_DBC );
        }
    }
    else
    {
        if ( _rc != SQL_NO_DATA )
            ThrowError();
    }

    if ( _hstmt != NULL )
    {
        SQLRETURN rc = ::SQLFreeHandle( SQL_HANDLE_STMT, _hstmt );
        _hstmt = NULL;
    }

    bool retVal = _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO;
    return retVal;
}

int CSQLDatabase::GetAffectedRows()
{
	SQLLEN nRows = 0;

	_rc = ::SQLRowCount(_hstmt, &nRows);
	return nRows;
}

int CSQLDatabase::GetSQLInt( const char * a_szStmt, int a_nDefault )
{
    try 
    {
        CSQLRecordset rs( _hdbc );
        if ( rs.SQLExecDirect( a_szStmt ) )
        {
            if ( rs.SQLFetch() ) 
            {
                _sResult = rs.SQLGetData( 1 );
                return atoi( _sResult.c_str() );
            }
        }
        return a_nDefault;
    }
    catch ( CSQLException* e )
    {
        e->Delete(); // must do if not using MFC catch () macro
        return a_nDefault;
    }
}

std::string CSQLDatabase::GetSQLString( const char * a_szStmt, const char* a_szDefault )
{
    static string sDefault;
    sDefault = a_szDefault;
    try 
    {
        CSQLRecordset rs( _hdbc );
        if ( rs.SQLExecDirect( a_szStmt ) )
        {
            if ( rs.SQLFetch() ) 
            {
                _sResult = rs.SQLGetData( 1 );
                return _sResult.c_str();
            }
        }
    }
    catch ( CSQLException* e )
    {
        e->Delete(); // must do if not using MFC catch () macro
    }
    return sDefault.c_str();
}

// see CSQLRecordset::ThrowError(); code is same.  Make a base class?
void CSQLDatabase::ThrowError( SQLSMALLINT handleType )
{
    SQLCHAR     szSQLState[6];
    SQLINTEGER  nNativeError = 0;
    SQLCHAR     szErrorMsg[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT cbErrorMsgMax = sizeof( szErrorMsg ) - 1;
    SQLSMALLINT cbErrorMsg = 0;

    SQLRETURN   rc;
    SQLHANDLE   theHandle = _hstmt; // the default
    switch (handleType)
    {
    case SQL_HANDLE_ENV:  theHandle = _henv;  break;
    case SQL_HANDLE_DBC:  theHandle = _hdbc;  break;
    case SQL_HANDLE_STMT: theHandle = _hstmt; break;
   
       //case SQL_HANDLE_DESC: theHandle =       break;
    default: return;
    }

    rc = SQLGetDiagRec( handleType, theHandle, 1 /*recnumber?*/, szSQLState, 
        &nNativeError, szErrorMsg, cbErrorMsgMax, &cbErrorMsg );
    szSQLState[sizeof( szSQLState )-1]=0; // insurance
    szErrorMsg[cbErrorMsg] = 0;         // more insurance

    CSQLException* pException = new CSQLException();
    pException->m_nRetCode = (SQLRETURN)nNativeError;
    pException->m_strError = ( const char* )szErrorMsg;
    pException->m_strStateNativeOrigin = ( const char* )szSQLState;

    if ( _hstmt != NULL )
    {
        SQLRETURN rc = ::SQLFreeHandle( SQL_HANDLE_STMT, _hstmt );
        _hstmt = NULL;
    }

    throw pException;
}

bool CSQLDatabase::SupportsTransactions()    // supports ROLLBACK and COMMIT
{
    SQLUINTEGER result;
    SQLRETURN rc = ::SQLGetInfo( _hdbc, SQL_TXN_CAPABLE,( SQLPOINTER )&result, 0, 0 );
    return result != SQL_TC_NONE;
}

void CSQLDatabase::SetAutoCommit( bool tf ) // makes all transactions COMMIT immediately
{
    _autoCommit = tf;

    if ( _autoCommit == true )
        return;

    if ( ! SupportsTransactions() )
        _autoCommit = true;
}

bool CSQLDatabase::GetAutoCommit()
{
    return _autoCommit;
}

bool CSQLDatabase::Commit()
{
    _rc = ::SQLEndTran( SQL_HANDLE_DBC, _hdbc, SQL_COMMIT );
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
        ThrowError( SQL_HANDLE_DBC );
    return true;
}

bool CSQLDatabase::Rollback()
{
    _rc = ::SQLEndTran( SQL_HANDLE_DBC, _hdbc, SQL_ROLLBACK );
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
        ThrowError( SQL_HANDLE_DBC );
    return true;
}

CSQLDelete::CSQLDelete()
:CSQLBaseWhere()
{
}

CSQLDelete::CSQLDelete( const char* table )
:CSQLBaseWhere()
{
    SetTable( table );
}

CSQLDelete::~CSQLDelete()
{
}

std::string CSQLDelete::GetStatement()
{
    _stmt = "delete from " + _table;
    if ( _where != "" )
    {
        _stmt += " where ";
        _stmt += _where;
    }
    return _stmt.c_str();
}


CSQLInsert::CSQLInsert()
:CSQLBase()
{
}

CSQLInsert::CSQLInsert( const char* tableName )
:CSQLBase()
{
    SetTable( tableName );
}

CSQLInsert::~CSQLInsert()
{
}

void CSQLInsert::SetColumn( const char* columnName, const char* a_chValue, char a_DataType )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    if ( _values != "" )
        _values += ", ";

    if ( a_chValue[0] == '\0')
    {
        _values += "''";
    }
    else if (stricmp( a_chValue, "NULL" ) == 0 )
    {
        _values += "NULL";
    }
    else if ( strlen( a_chValue ) >= 3 && a_chValue[0] == '{' && a_chValue[1] == 'd' 
        && a_chValue[2] == '\'' ) // {d'1972-01-15'} etc.
    {
        _values += a_chValue;
    }
    else if ( stricmp( a_chValue, "now()" ) == 0 ) // now()
    {
        _values += a_chValue;
    }
    else // otherwise single quote all strings
    {
        if ( a_DataType == STRING )
        {
            std::string value = a_chValue;
            ConvertQuotes( value );
            _values += "'";
            _values += value;
            _values += "'";
        }
        else if ( a_DataType == ORACLE_DATE )
        {
            TDt dt( a_chValue );
            std::string cs = dt.Format( TDt::ORACLE );
            _values += "'";
            _values += cs;
            _values += "'";
        }
        else if ( a_DataType == ODBC_DATE 
            || a_DataType == DATE_MMDDYYYY )
        {
            TDt dt( a_chValue );
            std::string cs = dt.Format( TDt::ODBC );
            _values += cs;
        }
        else if ( a_DataType == DOUBLE )
        {
            _values += a_chValue;
        }
        else if ( a_DataType == INTEGER )
        {
            _values += a_chValue;
        }
        else if ( a_DataType == LONG )
        {
            _values += a_chValue;
        }
        else
        {
            std::string value = a_chValue;
            ConvertQuotes( value );
            _values += "'";
            _values += value;
            _values += "'";
        }
    }
}

void CSQLInsert::SetColumn( const char* columnName, const std::string& a_sValue, char a_DataType )
{
    SetColumn( columnName, a_sValue.c_str(), a_DataType );
}

void CSQLInsert::SetColumn( const char* columnName, double value )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    if ( value > 10000000000000.0 || value < -10000000000000.0 )
        value = 0.0;

    char ach[32];
    sprintf( ach, "%.2f", value );
    if ( _values != "" )
        _values += ", ";
    _values += ach;
}

void CSQLInsert::SetColumn( const char* columnName, int value )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    char ach[32];
    sprintf( ach, "%d", value );
    if ( _values != "" )
        _values += ", ";
    _values += ach;
}

/*
void CSQLInsert::SetColumn( const char* columnName, COleDateTime& value )
{
if ( _columns != "" )
_columns += ", ";
_columns += columnName;

std::string cs;
if ( value.GetStatus() == COleDateTime::valid )
cs = value.Format( "{d'%Y-%m-%d'}" );
else
cs = "NULL";

if ( _values != "" )
_values += ", ";
_values += cs;
}
*/

void CSQLInsert::SetColumn( const char* columnName, bool isValidTime, int month, int day, int year )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    std::string cs;
    if ( isValidTime )
    {
        TDt dt( month, day, year );
        cs = dt.Format( TDt::ODBC );
    }
    else
        cs = "NULL";

    if ( _values != "" )
        _values += ", ";
    _values += cs;
}


void CSQLInsert::SetColumn( const char* columnName, long value, char a_DataType )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    if ( a_DataType == LONG )
    {
        char ach[32];
        sprintf( ach, "%d", value );
        if ( _values != "" )
            _values += ", ";
        _values += ach;
    }
    else if ( a_DataType == ODBC_DATE
        || a_DataType == JULIAN_DATE )
    {
        TDt dt( value );
        std::string cs = dt.Format( TDt::ODBC );
        if ( _values != "" )
            _values += ", ";
        _values += cs;

    }
}

std::string CSQLInsert::GetStatement()
{
    _stmt = "insert into " + _table + " ( ";
    _stmt += _columns;
    _stmt += " ) values ( ";
    _stmt += _values;
    _stmt += " )";
    return _stmt.c_str();
}

void CSQLInsert::ResetContent()
{
    CSQLBase::ResetContent();

    _columns = "";
    _values = "";
}

CSQLRecordset::CSQLRecordset()
:CSQLBaseWhere(),
_rc( SQL_SUCCESS ),
_hdbc( NULL ),
_hstmt( NULL )
{
}

CSQLRecordset::CSQLRecordset( SQLHDBC a_hdbc )
:CSQLBaseWhere(),
_rc( SQL_SUCCESS ),
_hdbc( a_hdbc ),
_hstmt( NULL )
{
    assert( a_hdbc != NULL );
}

CSQLRecordset::~CSQLRecordset()
{
    SQLFreeStmt(SQL_DROP, false); // false==don't throw on error
}

CSQLRecordset::CSQLRecordset( CSQLDatabase& db )
:CSQLBaseWhere(),
_rc( SQL_SUCCESS ),
_hdbc( db._hdbc ),
_hstmt( NULL )
{
}

void CSQLRecordset::Connect( CSQLDatabase& db )
{
    SQLFreeStmt();
    _hdbc = db._hdbc; 
}

void CSQLRecordset::operator << ( const char* statement )
{
    _stmt = statement;
}

void CSQLRecordset::operator << ( const std::string& statement )
{
    _stmt = statement;
}

bool CSQLRecordset::SQLAllocStmt()
{
    if ( _hdbc == NULL )
        return false;

    SQLFreeStmt();

    _rc = ::SQLAllocStmt( _hdbc, &_hstmt );
    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        ThrowError( true ); // use db handle      // [2001-12-03]

    return true;
}

bool CSQLRecordset::SQLExecDirect( const char * a_szStmt )
{
    _stmt = a_szStmt;
    return SQLExecDirect();
}

void CSQLRecordset::ResetContent()
{
    CSQLBaseWhere::ResetContent();
    CSQLBase::ResetContent();
    _groupBy = "";
    _orderBy = "";
    _result  = "";
}

bool CSQLRecordset::SQLExecDirect()
{
    if ( _where != "" )
    {
        _stmt += " where ";
        _stmt += _where;
    }

    if ( _orderBy != "" )
    {
        _stmt += " order by ";
        _stmt += _orderBy;
    }

    if ( _groupBy != "" )
    {
        _stmt += " group by ";
        _stmt += _groupBy;
    }

    if ( ! SQLAllocStmt() )
        return false;

	_rc = SQLSetStmtOption(_hstmt, SQL_QUERY_TIMEOUT, 30);

    _rc = ::SQLExecDirect( _hstmt,( unsigned char* )( const char* )_stmt.c_str(), SQL_NTS );
    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        ThrowError();

    return true;
}

bool CSQLRecordset::SQLFetch()
{
    _rc = ::SQLFetch( _hstmt );

    if ( _rc == SQL_NO_DATA_FOUND )
        return false;

    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        ThrowError();

    return true;
}

// [2003-09-06]
// Note: since this method gets called from a destructor, we've got to 
// be able to turn of the throw on error logic or else serious program
// errors will result.
void CSQLRecordset::SQLFreeStmt( SQLUSMALLINT a_uType, bool throwOnError )
{
    _rc = SQL_SUCCESS_WITH_INFO; // in case it's null
    if ( _hstmt == NULL )
        return;

    _rc = ::SQLFreeStmt( _hstmt, a_uType );
    _hstmt = NULL;
    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        if (throwOnError) // you don't want to throw from a destructor
            ThrowError();
}

void CSQLRecordset::SetHDBC( HDBC* a_hdbc )
{
    SQLFreeStmt();
    _hdbc = a_hdbc;
}

std::string CSQLRecordset::SQLGetData( int a_uRow, int a_eDataType )
{
    _result = "";

    SDWORD cbData;
    // unfinished: rewrite this for pointer to buffer and dynamically allocate it )
    char ach[10240];
    ach[0] = 0;
    _rc = ::SQLGetData( _hstmt,( UWORD )a_uRow, SQL_C_CHAR, ach, sizeof( ach ), &cbData );
    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        ThrowError();

    _result = ach;

    if ( a_eDataType == DOUBLE )
    {
        char ach1[32];
        sprintf( ach1, "%.2f", atof( ach ) );
        _result = ach1;
        return _result;
    }
    else if ( a_eDataType == ODBC_DATE )
    {
        TDt dt = ach;
        _result = dt.Format( TDt::ODBC );
        return _result;
    }
    else if ( a_eDataType == DATE_YYYYMMDD )
    {
        TDt dt = ach;
        _result = dt.Format( TDt::YYYYMMDD );
        return _result;
    }
    //    else if ( a_eDataType == DATE_YYYYMMDDHHMMSS || a_eDataType == DATETIME )
    //    {
    //       return ach;
    //    }
    else if ( a_eDataType == DATE_MMDDYYYY )
    {
        TDt dt = ach;
        _result = dt.Format( TDt::MMDDYYYY );
        return _result;
    }
    else
        return _result;
}

int CSQLRecordset::nSQLGetData( int column )
{
    return atoi( SQLGetData( column ).c_str() );
}

bool CSQLRecordset::bSQLGetData( int column )
{
    return( atoi( SQLGetData( column ).c_str() ) != 0 );
}

long CSQLRecordset::lSQLGetData( int column, int a_eDataType )
{
    if ( a_eDataType == JULIAN_DATE || a_eDataType == ODBC_DATE )
    {
        TDt dt = SQLGetData( column ).c_str();
        return dt.Julian();
    }

    return atol( SQLGetData( column ).c_str() );
}

double CSQLRecordset::dblSQLGetData( int column )
{
    return atof( SQLGetData( column ).c_str() );
}

void CSQLRecordset::SetOrderBy( const char * a_szOrderBy )
{
    _orderBy = a_szOrderBy;
}

void CSQLRecordset::SetGroupBy( const char * a_szGroupBy )
{
    _groupBy = a_szGroupBy;
}

int CSQLRecordset::GetColumns()
{
    char ach[32];
    SDWORD dwDesc;
    SWORD cbDesc;
    _rc = ::SQLColAttributes( _hstmt, 1, SQL_COLUMN_COUNT, ach, sizeof( ach ), &cbDesc, &dwDesc );
    if ( _rc != SQL_SUCCESS && _rc != SQL_SUCCESS_WITH_INFO )
        ThrowError();

    return dwDesc;
}

void CSQLRecordset::ThrowError( bool useDbHandle )
{
    SQLCHAR     szSQLState[6];
    SQLINTEGER  nNativeError = 0;
    SQLCHAR     szErrorMsg[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT cbErrorMsgMax = sizeof( szErrorMsg ) - 1;
    SQLSMALLINT cbErrorMsg = 0;
    SQLRETURN   rc;

    szSQLState[0] = 0; // [2001-12-03]
    szErrorMsg[0] = 0; // [2001-12-03]

    if ( useDbHandle )
    {
        rc = SQLGetDiagRec( SQL_HANDLE_DBC, _hdbc, 1, szSQLState, 
            &nNativeError, szErrorMsg, cbErrorMsgMax, &cbErrorMsg );
    }
    else
    {
        rc = SQLGetDiagRec( SQL_HANDLE_STMT, _hstmt, 1, szSQLState, 
            &nNativeError, szErrorMsg, cbErrorMsgMax, &cbErrorMsg );
    }
    szSQLState[sizeof( szSQLState )-1]=0; // insurance

    CSQLException* pException = new CSQLException();
    pException->m_nRetCode = (SQLRETURN)nNativeError;
    pException->m_strError = ( const char* )szErrorMsg;
    pException->m_strStateNativeOrigin = ( const char* )szSQLState;
    pException->_statement = _stmt;
    throw pException;
}


// the recordset must be active for this to work in
// other words a select must have been done and waiting
// for fetch statements.  Here's a case block--
#if ( 0 )
switch ( n ) {
   case SQL_UNKNOWN_TYPE: break;
   case SQL_CHAR:         break;
   case SQL_NUMERIC:      break;
   case SQL_DECIMAL:      break;
   case SQL_INTEGER:      break;
   case SQL_SMALLINT:     break;
   case SQL_FLOAT:        break;
   case SQL_REAL:         break;
   case SQL_DOUBLE:       break;
   case SQL_DATETIME:     break;
   case SQL_VARCHAR:      break;
   case SQL_DATE:         break;
   case SQL_TIME:         break;
   case SQL_TIMESTAMP:    break;
   default:               break;
}
#endif

int CSQLRecordset::GetColumnType( int a_nColumn )
{
    unsigned char achColName[SQL_MAX_COLUMN_NAME_LEN+1];
    SWORD cbColName;
    SWORD fSQLType;
    UDWORD cbPrecision;
    SWORD cbScale;
    SWORD fNullable;
    _rc = ::SQLDescribeCol( _hstmt, a_nColumn, achColName,
        SQL_MAX_COLUMN_NAME_LEN, &cbColName, &fSQLType, &cbPrecision, 
        &cbScale, &fNullable );
    if ( !( _rc == SQL_SUCCESS || _rc == SQL_SUCCESS_WITH_INFO ) )
    {
        return 0;
    }
    return fSQLType;
}

#include <sstream>
#include <iomanip>
using namespace std;

CSQLUpdate::CSQLUpdate()
:CSQLBaseWhere()
{
}

CSQLUpdate::CSQLUpdate( const char* table )
:CSQLBaseWhere()
{
    SetTable( table );
}

CSQLUpdate::~CSQLUpdate()
{
}

void CSQLUpdate::SetColumn( const char* columnName, const std::string& a_sValue, char a_DataType )
{
    SetColumn( columnName, a_sValue.c_str(), a_DataType );
}

void CSQLUpdate::SetColumn( const char* columnName, const char* a_chValue, char a_DataType )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    // if there's already an equal sign then don't add one.  This makes
    // it possible for the user to enter "column=column+"
    std::string cs = columnName;
    if ( cs.find( '=' ) == cs.npos )
        _columns += "=";

    if ( a_chValue[0] == '\0')
    {
        _columns += "''";
    }
    else if (stricmp( a_chValue, "NULL" ) == 0)
    {
        _columns += "NULL";
    }
    else if ( strlen( a_chValue ) >= 3 && a_chValue[0] == '{' && a_chValue[1] == 'd' 
        && a_chValue[2] == '\'' ) // {d'1972-01-15'} etc.
    {
        _columns += a_chValue;
    }
    else if ( stricmp( a_chValue, "now()" ) == 0)
    {
        _columns += a_chValue; //[09-18-2001]
    }
    else
    {
        if ( a_DataType == CSQLBase::STRING )
        {
            std::string value = a_chValue;
            ConvertQuotes( value );
            _columns += "'";
            _columns += value;
            _columns += "'";
        }
        else if ( a_DataType == CSQLBase::ORACLE_DATE )
        {
            TDt dt( a_chValue );
            std::string cs = dt.Format( TDt::ORACLE );
            _columns += "'";
            _columns += cs;
            _columns += "'";
        }
        else if ( a_DataType == CSQLBase::ODBC_DATE )
        {
            TDt dt( a_chValue );
            std::string cs = dt.Format( TDt::ODBC );
            _columns += cs;
        }
        else if ( a_DataType == CSQLBase::DOUBLE )
        {
            _columns += a_chValue;
        }
        else if ( a_DataType == CSQLBase::INTEGER )
        {
            _columns += a_chValue;
        }
        else if ( a_DataType == CSQLBase::LONG )
        {
            _columns += a_chValue;
        }
    }
}

void CSQLUpdate::SetColumn( const char* columnName, double value )
{
    if ( _columns != "" )
        _columns += ", ";

    _columns += columnName;

    // if there's already an equal sign then don't add one.  This makes
    // it possible for the user to enter "column=column+"
    std::string cs = columnName;
    if ( cs.find( '=' ) == cs.npos )
        _columns += "=";

    ostringstream os;
    os << fixed << setprecision( 2 ) << value;
    _columns += os.str();
}

void CSQLUpdate::SetColumn( const char* columnName, bool isValidTime, int month, int day, int year )
{
    if ( _columns != "" )
        _columns += ", ";

    _columns += columnName;

    // if there's already an equal sign then don't add one.  This makes
    // it possible for the user to enter "column=column+"
    std::string cs = columnName;
    if ( cs.find( '=' ) == cs.npos )
        _columns += "=";

    if ( isValidTime )
    {
        TDt dt( month, day, year );
        cs = dt.Format( TDt::ODBC );
    }
    else
        cs = "NULL";

    _columns += cs;
}

void CSQLUpdate::SetColumn( const char* columnName, int value )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    // if there's already an equal sign then don't add one.  This makes
    // it possible for the user to enter "column=column+"
    std::string cs = columnName;
    if ( cs.find( '=' ) == cs.npos )
        _columns += "=";

    ostringstream os;
    os << dec << value;
    _columns += os.str();
}

void CSQLUpdate::SetColumn( const char* columnName, long a_lValue, char a_DataType )
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += columnName;

    // if there's already an equal sign then don't add one.  This makes
    // it possible for the user to enter "column=column+"
    std::string cs = columnName;
    if ( cs.find( '=' ) == cs.npos )
        _columns += "=";

    if ( a_DataType == CSQLBase::ODBC_DATE )
    {
        if ( a_lValue != 0L )
        {
            TDt dt( a_lValue );
            cs = dt.Format( TDt::ODBC );
            _columns += cs;
        }
        else
        {
            _columns += "NULL";
        }
    }
    else
    {
        char ach[32];
        sprintf( ach, "%ld", a_lValue );
        _columns += ach;
    }
}

void CSQLUpdate::SetColumn(const char *a_csAssignment)
{
    if ( _columns != "" )
        _columns += ", ";
    _columns += a_csAssignment;
}

std::string CSQLUpdate::GetStatement()
{
    _stmt = "update " + _table + " set ";
    _stmt += _columns;
    if ( _where != "" )
    {
        _stmt += " where ";
        _stmt += _where;
    }
    return _stmt.c_str();
}

void CSQLUpdate::ResetContent()
{
    CSQLBase::ResetContent();
    CSQLBaseWhere::ResetContent();

    _columns = "";
}

