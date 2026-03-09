#pragma once


#ifndef DB3_H_
#define DB3_H_

#include <windows.h>
#include <sql.h>
#include <string>
// Display Format

class TDt
{
public:
    TDt();
    TDt(long jul);
    TDt(int m, int d, int y);
    TDt(const char* lpszDate);
    TDt(TDt& dt);

    static const int FULL;
    static const int EUROPEAN;
    static const int MMDDYYYY;
    static const int MMMDDD;
    static const int DDDW;
    static const int YYYYMMDD;
    static const int CRWFORMULA;
    static const int LETTER;
    static const int ODBC;
    static const int MMMYYYY;
    static const int DAY;
    static const int MONTH;
    static const int ORACLE;
    static const int DDMMMYY;
    static const int DD;
    static const int MM;
    static const int YYYY;
    static const int DTH;
    static const int D;
    static const int M;

    TDt& operator = ( const TDt& dt );
    TDt& operator = ( const char* lpsz );  // dt = "03-01-1993"
    TDt& operator = ( long lJulian );      // dt = 36525 is 12-31-1999

    std::string Format( const char* fs );
    std::string Format( int a_nDisplayFormat = MMDDYYYY );
    void Format( char* ach, int size, int a_nDisplayFormat = MMDDYYYY );

    TDt& operator += ( int i );
    TDt& operator -= ( int i );
    TDt& operator ++();
    TDt  operator ++( int );
    TDt& operator --();
    TDt  operator --( int );

    bool operator <  ( TDt& dt2 ) { return Julian() <  dt2.Julian(); }
    bool operator <= ( TDt& dt2 ) { return Julian() <= dt2.Julian(); }
    bool operator >  ( TDt& dt2 ) { return Julian() >  dt2.Julian(); }
    bool operator >= ( TDt& dt2 ) { return Julian() >= dt2.Julian(); }
    bool operator == ( TDt& dt2 ) { return Julian() == dt2.Julian(); }
    bool operator != ( TDt& dt2 ) { return Julian() != dt2.Julian(); }

    friend bool operator < ( long dt1, TDt& dt2 );
    friend bool operator <= ( long dt1, TDt& dt2 );
    friend bool operator > ( long dt1, TDt& dt2 );
    friend bool operator >= ( long dt1, TDt& dt2 );
    friend bool operator == ( long dt1, TDt& dt2 );
    friend bool operator != ( long dt1, TDt& dt2 );

    bool operator < ( long l ) { return Julian() <  l; }
    bool operator <= ( long l ) { return Julian() <= l; }
    bool operator > ( long l ) { return Julian() >  l; }
    bool operator >= ( long l ) { return Julian() >= l; }
    bool operator == ( long l ) { return Julian() == l; }
    bool operator != ( long l ) { return Julian() != l; }

    //friend ostream& operator <<( ostream &os, TDt& dt );
    //friend ostream& operator <<( ostream &os, const TDt& dt );

    void  SetFormat( int format = MMDDYYYY );
    bool  IsNull();
    long  Julian();           // returns julian date
    int   DOY();              // returns relative day since Jan 1
    int   IsLeap();           // returns 1 if leap year, 0 if not

    TDt& Set();               // Sets to current system date
    TDt& Set( const char* p );
    TDt& Set( long lJulian );
    TDt& Set( int nMonth, int nDay, int nYear );
    TDt& Set( TDt& dt );
    TDt& Set( const TDt& dt );

    TDt& AddWeeks( int nCount = 1 );  //
    TDt& AddMonths( int nCount = 1 ); // May also pass neg# to decrement
    TDt& AddYears( int nCount = 1 );  //

    std::string DayName();   // const string pointer to "Monday" etc...    // potential error
    std::string DOWName();   // Day Of Week( 'Sunday'..'Saturday' )         // potential error
    std::string MonthName(); // Character month name                       // potential error

    int Day();          // Day number( day of month 1..[28]..[31] )
    int Month();        // [1..12]
    int Year();         // e.g. 1992

    int DOW();          //( Sunday=0..Saturday=6 )
    int DOM();          // 2nd tuesday, 3rd wed, etc...
    int WOM();          // Numeric week Of month ( 1..6 )
    int WOY();          // Numeric week Of year  ( 1..52 )
    int DaysInMonth();  // [1..31]
    int FirstDOM();     // First Day Of Month ( 0..6 )

    int Age();          // The age of person in years
    int YYMM();         // integer yymm ( 9201 )

    TDt& SetBOM();      // First day of Month
    TDt& SetEOM();      // Last day of Month
    TDt& SetBOY();      // First day Of Year
    TDt& SetEOY();      // Last day Of Year

    int DifferenceInYears( long d1, long d2 );  // result = d1 - d2
    int DifferenceInMonths( long d1, long d2 );

    static int m_nDisplayFormat;

private:
    void GetMDY( int& m, int &d, int& y, long jul );
    long GetJulian( int m, int d, int y );
    int  Get4DigitYear( int yy );
    std::string GetNumberSuffix( int day );
    int m_month;
    int m_day;
    int m_year;

    //string _s; // a place to put results from stuff
};

// use like this: .SetColumn( "postedon", SQLBASE_OLEDATETIME( dtObject ) );
#define SQLBASE_OLEDATETIME(o) ((o).GetStatus()==COleDateTime::valid), (o).GetMonth(), (o).GetDay(), (o).GetYear()
#define SQLBASE_SYSTEMDATETIME(o) ((o).GetYear() != 0), (o).GetMonth(), (o).GetDay(), (o).GetYear()

class CSQLBase 
{
public:
    CSQLBase();
    ~CSQLBase();

    void SetStmt( const char* a_szStmt );
    std::string GetStatement();

    static const char STRING;
    static const char JULIAN_DATE;
    static const char ODBC_DATE;
    static const char ORACLE_DATE;
    static const char DATE_YYYYMMDD;
    static const char DATETIME;
    static const char DATE_YYYYMMDDHHMMSS;
    static const char DATE_MMDDYYYY;
    static const char DOUBLE;
    static const char INTEGER;
    static const char LONG;

    void SetTable( const char* tableName );
    CSQLBase& operator=(const char* );
    void ResetContent();

    bool m_bConvertQuotes;

    std::string _table;
    std::string _stmt;

protected:
    bool ConvertQuotes( std::string& a_cs );
private:
    CSQLBase(CSQLBase& o) {;}
};

class CSQLBaseWhere : public CSQLBase
{
public:
    CSQLBaseWhere();
    virtual ~CSQLBaseWhere();

    void SetWhere( const char* a_szWhere );
    void SetWhere( const char* columnName, int value );
    void SetWhere( const char* columnName, long value, char a_DataType=LONG );
    void SetWhere( const char* columnName, double value );
    void SetWhere( const char* columnName, bool isValidTime, int month, int day, int year );
    void SetWhere( const char* columnName, const char* value, char a_DataType=STRING, bool a_bUseLike=false, bool a_bUseOr=false );
    void SetWhere( const char* columnName, const std::string& value, char a_DataType=STRING, bool a_bUseLike=false, bool a_bUseOr=false );

    void SetWhereOr( const char* a_szWhere );
    void SetWhereParenthesis();

    void SetOrderBy( const char* a_szOrderBy );

    std::string GetWhere() { return _where.c_str(); }
    std::string GetStatement( bool includeWhereLiteral=true );
    void ResetContent();
protected:
    std::string _where;
    std::string _orderby;
};

class CSQLException
{
public:
    CSQLException() :m_nRetCode( 0 ) {;}
    CSQLException( SQLRETURN r ) { m_nRetCode = r; }
    ~CSQLException() {;}

    SQLRETURN m_nRetCode;
    std::string m_strError;
    std::string m_strStateNativeOrigin;
    std::string _statement;

    void Delete() { delete this; }
private:
    CSQLException( const CSQLException& o ) {;} // don't allow copy
    //operator = ( CSQLException& ) {;}
};

class CSQLDatabase  {
public:
    CSQLDatabase();
    virtual ~CSQLDatabase();
    bool SQLConnect(char const* dsn, char const* user, char const* password);
    bool Open(const char* dsn, const char* user, const char* password);
    bool OpenEx(const char* dsn, const char* user, const char* password);

    bool SQLConnect(char const* conn_str);
    bool Open(char const* conn_str);

    bool Close();
    bool ExecuteSQL( const char* a_szStmt );
	int GetAffectedRows();
    bool ExecuteSQL( const std::string& s ) { return ExecuteSQL( s.c_str() ); }
    bool IsOpen() { return _isOpen; }
    void ThrowError( SQLSMALLINT handleType=SQL_HANDLE_STMT );

    std::string GetSQLString( const char* stmt, const char* sDefault="" );
    int GetSQLInt( const char* stmt, int nDefault=0 );
    SQLRETURN GetRetCode() { return _rc; }

    bool SupportsTransactions();   // supports ROLLBACK and COMMIT
    void SetAutoCommit( bool tf ); // makes all transactions COMMIT immediately
    bool GetAutoCommit();
    bool Commit();
    bool Rollback();
    std::string& dbmsName();

    SQLRETURN _rc;    // void*
    SQLHENV   _henv;  // void*
    SQLHDBC   _hdbc;  // void*
    HSTMT     _hstmt; // void*

private:
    bool     _isOpen;
    std::string   _dsn;
    std::string   _user;
    std::string   _pswd;
    std::string   _sResult; // for GetSQL... functions
    bool     _autoCommit;
    std::string   _dbmsName;  // reset at each open
};

class CSQLDelete : public CSQLBaseWhere 
{
public:
    CSQLDelete();
    CSQLDelete( const char* table );
    CSQLDelete( CSQLDelete& o );
    ~CSQLDelete();

    std::string GetStatement();
};

class CSQLInsert :public CSQLBase 
{
public:
    CSQLInsert();
    CSQLInsert( const char* table );
    CSQLInsert( CSQLInsert& o );
    ~CSQLInsert();

    std::string GetStatement();
    bool HasColumns() { return( _columns.length() > 0 ); }
    void SetColumn( const char* columnName, const char* a_chValue, char a_DataType=STRING );
    void SetColumn( const char* columnName, const std::string& a_sValue, char a_DataType=STRING );
    void SetColumn( const char* columnName, double value );
    void SetColumn( const char* columnName, long value, char a_DataType=CSQLBase::LONG );
    void SetColumn( const char* columnName, int value );
    void SetColumn( const char* columnName, bool isValidTime, int month, int day, int year ); // use CSQLBASE_OLEDATETIME( o ) as 2nd arg

    void ResetContent();
private:
    std::string _columns;
    std::string _values;
};

class CSQLRecordset : public CSQLBaseWhere
{
    // constructor/destructor
public:
    CSQLRecordset();
    CSQLRecordset( SQLHDBC a_hdbc );
    CSQLRecordset( CSQLDatabase& db );
    virtual ~CSQLRecordset();
    void Connect( CSQLDatabase& db );
    void ResetContent();

    void operator <<( const char* statement );
    void operator <<( const std::string& statement );

    bool SQLExecDirect();
    bool SQLExecDirect( const char* a_szStmt );
    bool SQLExecDirect( const std::string& stmt ) { return SQLExecDirect( stmt.c_str() ); }
    bool SQLFetch();
    std::string SQLGetData( int a_uRow, int a_eDataType=CSQLBase::STRING );

    void     SetOrderBy( const char * a_szOrderBy );
    void     SetGroupBy( const char* a_szGroupBy );

double   dblSQLGetData( int column );
    int      nSQLGetData( int column );
    bool     bSQLGetData( int column );
    long     lSQLGetData( int column, int a_eDataType );
    int      GetColumnType( int a_nColumn );
    int      GetColumns();
    void     SetHDBC( HDBC* a_hdbc );
    bool     SQLAllocStmt();
    void     SQLFreeStmt( SQLUSMALLINT a_uType=SQL_DROP, bool throwOnError=true );

protected:
    void ThrowError( bool useDbHandle=false ); // [2001-12-03]
private:

    SQLRETURN _rc;

    SQLHDBC   _hdbc;
    SQLHSTMT  _hstmt;

    std::string   _orderBy;
    std::string   _groupBy;

    std::string _result;
};

class CSQLUpdate : public CSQLBaseWhere {
public:
    CSQLUpdate();
    CSQLUpdate( const char* table );
    CSQLUpdate(CSQLUpdate& o);
    ~CSQLUpdate();

    std::string GetStatement();
    bool HasColumns() { return (_columns.length() > 0); }

    void SetColumn( const char* a_csAssignment );
    void SetColumn( const char* columnName, const char* a_chValue, char a_DataType=CSQLBase::STRING );
    void SetColumn( const char* columnName, const std::string& a_sValue, char a_DataType=CSQLBase::STRING );
    void SetColumn( const char* columnName, long a_lValue, char a_DataType=CSQLBase::LONG );
    void SetColumn( const char* columnName, double value );
    void SetColumn( const char* columnName, int value );
    //void SetColumn( const char* columnName, COleDateTime& value );
    void SetColumn( const char* columnName, bool isValidTime, int month=0, int day=0, int year=0 );

    void ResetContent();
private:
    std::string _columns;
};

#endif
