
#include "terrainattrib.h"
#include "util.h"
#include <iostream>
#include <memory>

using namespace std;

// 地图文件文件头定义
struct MPMapFileHeader
	{
	int nMapFlag;
	int nWidth;
	int nHeight;
	int nSectionWidth;
	int nSectionHeight;};

//// 设置对齐方式为1字节，因为要写入文件中
//#pragma pack(push)
//#pragma pack(1)
//
//// 区域属性文件文件头定义
//struct SAttribFileHeader
//	{
//	unsigned int width;
//	unsigned int height;
//	};
//
//// 区域属性文件单位数据定义
//typedef struct _Tile_Attrib
//	{
//	unsigned short attrib; // tile属性
//	unsigned char island; // 海岛编号
//
//	} STILE_ATTRIB;
//#pragma pack(pop)


class CTerrainAttrib
	{
public:
	CTerrainAttrib();
	~CTerrainAttrib();

	bool createFile(char const* filename, int width, int height, int option);
	bool openFile(char const* filename);

	void Init(char const* pszTerrainName);

	int getWidth() const {return _width;}
	int getHeight() const {return _height;}


	unsigned short getTileAttrib(int nX, int nY);
	void setTileAttrib(int nX, int nY, unsigned char btAttrib, bool bAdd = true);
	bool hasTileAttrib(int nX, int nY, unsigned char btAttrib);
    bool delTileAttrib(int x, int y, unsigned char attrib);

	unsigned char getTileIsland(int nX, int nY);
	void setTileIsland(int nX, int nY, unsigned char btIsland);

	// 初始化标志
	bool m_bInitFlag;

protected:
	bool _validateFilePointer();
	bool _validateTile(int nX, int nY);
	bool _validateTileAttrib(unsigned char btAttrib);
	bool _validateTileIsland(unsigned char btIsland);

	int _seekTile(int nX, int nY);

	bool _getMapInfo(char const* filename, int& width, int& height);

private:
	FILE* _fp;
	int _width;
	int _height;

	SAttribFileHeader _header;
	STILE_ATTRIB _tile_attrib;
	};


CTerrainAttrib::CTerrainAttrib(void)
	{
	_fp = NULL;
	_width = 0;
	_height = 0;

	memset(&_header, 0, sizeof _header);
	memset(&_tile_attrib, 0, sizeof _tile_attrib);

	m_bInitFlag = false;
	}

CTerrainAttrib::~CTerrainAttrib(void)
	{
	if (_fp)
		fclose(_fp);
	}


inline bool CTerrainAttrib::_validateFilePointer()
	{
	return (_fp != NULL) ? true : false;
	}

inline bool CTerrainAttrib::_validateTile(int nX, int nY)
	{
	if ((nX >= 0) && (nX < (_width - 1)) && (nY >= 0) && (nY < (_height - 1)))
		return true;
	else
		return false;
	}

inline bool CTerrainAttrib::_validateTileAttrib(unsigned char btAttrib)
	{
	if (btAttrib < TILE_ATTRIB_MIN_BITS || btAttrib > TILE_ATTRIB_MAX_BITS)
		return false;
	else return true;
	}

inline bool CTerrainAttrib::_validateTileIsland(unsigned char btIsland)
	{
	return (btIsland > TILE_ISLAND_MAX_VALUE) ? false : true;
	}

inline int CTerrainAttrib::_seekTile(int nX, int nY)
	{
	int offset = sizeof SAttribFileHeader + 
		(sizeof STILE_ATTRIB) * (nY * _width + nX);
    fseek(_fp, offset, SEEK_SET);
	return offset;
	}

bool CTerrainAttrib::_getMapInfo(char const* filename, int& width, int& height)
	{
	char mapfilename[128];
	MPMapFileHeader header;

	// 打开同名地图文件，取得宽度和高度
	sprintf(mapfilename, "map/%s.map", filename);
	FILE* fp = fopen(mapfilename, "rb");
	if (fp == NULL)
		{
		//LG("error", "msg地图[%s]不存在, 不能编辑其区域属性!", pszTerrainName);
		return false;
		}

	fread(&header, sizeof MPMapFileHeader, 1, fp);
	fclose(fp);

	width = header.nHeight;
	height = header.nWidth;
	return true;
	}

//
// 外部接口
//

// 创建一个新的区域属性文件或覆盖原有文件 (*.atr)
bool CTerrainAttrib::createFile(char const* filename, int width, int height,
								int option)
	{
	if (_fp != NULL)
		{
		fclose(_fp);
		_fp = NULL;
		}

	// 得到宽度_width和高度_height
	if (width <= 0 && height <= 0)
		{
		if (!_getMapInfo(filename, _width, _height))
			return false;
		}
	else
		{
		_width = width;
		_height = height;
		}

	// 检查参数标志及文件是否存在!
	char attribfilename[128];
	sprintf(attribfilename, "map/%s.atr", filename);

	if ((option == 0) && (_access(attribfilename, 0) == 0))
		{
		// 文件已存在，并且调用者并不强制创建
		return true;
		}

	// 创建
	_fp = fopen(attribfilename, "wb");
	if (_fp == NULL) return false;

	// 组织数据
	_header.width = _width;
	_header.height = _height;

	int cnt = _height * _width;
	_tile_attrib.attrib = TILE_ATTRIB_DEFAULT_VALUE;
	_tile_attrib.island = TILE_ISLAND_DEFAULT_VALUE;

	// 写入文件头和tile属性数据
	fwrite(&_header, sizeof _header, 1, _fp);
	for (int i = 0; i < cnt; ++ i)
		fwrite(&_tile_attrib, sizeof _tile_attrib, 1, _fp);

	// fwrite不能一次写超过4K的字节
	//fwrite(&_tile_attrib, sizeof _tile_attrib, cnt, _fp);

	fclose(_fp);
	_fp = NULL;
	return true;
	}

bool CTerrainAttrib::openFile(char const* fname)
	{
	char atr_fname[MAX_PATH];
	sprintf(atr_fname, "map/%s.atr", fname);

	if (_fp != NULL) {fclose(_fp); _fp = NULL;}

    // 去掉只读属性
    DWORD file_atr = ::GetFileAttributes(atr_fname);
    if (file_atr & FILE_ATTRIBUTE_READONLY)
        SetFileAttributes(atr_fname,
                FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL);
	_fp = fopen(atr_fname, "r+b");
	if (_fp == NULL)
		{
		_width = 0;
		_height = 0;
		return false;
		}

	// 读取文件头
	fread(&_header, sizeof _header, 1, _fp);
	_width = _header.width;
	_height = _header.height;
	return true;
	}

void CTerrainAttrib::Init(char const* pszTerrainName)
	{
	if (_fp)
		{
		fclose(_fp);
		_fp = NULL;
		}

	// 打开同名地图文件，取得宽度和高度
	char szMapFileName[128];
	sprintf(szMapFileName, "map/%s.map", pszTerrainName);

	FILE* fp = fopen(szMapFileName, "rb");
	if (fp == NULL)
		{
		//LG("error", "msg地图[%s]不存在, 不能编辑其区域属性!", pszTerrainName);
		return;
		}

	MPMapFileHeader header;
	fread(&header, sizeof MPMapFileHeader, 1, fp);
	fclose(fp);

	_height = header.nWidth;
	_width  = header.nHeight;

	// 检查区域属性文件是否存在，不存在则创建
	char szAttrFileName[128];
	sprintf(szAttrFileName, "map/%s.atr", pszTerrainName);
	if (_access(szAttrFileName, 0) == -1) // 文件不存在, 创建新文件
		{
		_header.width = _width;
		_header.height = _height;

		int nTileCnt = _height * _width;
		_tile_attrib.attrib = TILE_ATTRIB_DEFAULT_VALUE;
		_tile_attrib.island = TILE_ISLAND_DEFAULT_VALUE;

		_fp = fopen(szAttrFileName, "wb");
#if 0
		for(int i = 0; i < nTileCnt; ++ i)
			{
			fwrite(&_tile_attrib, sizeof _tile_attrib, 1, _fp);
			}
#endif
		// 写入文件头
		fwrite(&_header, sizeof _header, 1, _fp);

		// 写入数据
		fwrite(&_tile_attrib, sizeof _tile_attrib, nTileCnt, _fp);

		fclose(_fp);
		}

	// 打开区域属性文件
	_fp = fopen(szAttrFileName, "r+b");
	m_bInitFlag = true;
	}

unsigned short CTerrainAttrib::getTileAttrib(int nX, int nY)
	{
	unsigned short attrib = 0;

	if (_validateFilePointer() &&_validateTile(nX, nY))
		{
		_seekTile(nX, nY);
		fread(&_tile_attrib.attrib, sizeof _tile_attrib.attrib, 1, _fp);
		attrib = _tile_attrib.attrib;
		}

	return attrib;
	}

void CTerrainAttrib::setTileAttrib(int nX, int nY, unsigned char btAttrib, bool bAdd /* = true */)
	{
	if (!_validateTileAttrib(btAttrib))
		return;

	// btAttrib = {1, ..., 16}
	if (_validateFilePointer() && _validateTile(nX, nY))
		{
		unsigned short sAttrib = 0;
		unsigned short i = 1;

		_seekTile(nX, nY);
		long offset = sizeof _tile_attrib.attrib;
		fread(&_tile_attrib.attrib, offset, 1, _fp);
		sAttrib = _tile_attrib.attrib;

		if (bAdd)
			{
			// 增加tile属性
			sAttrib |= i << (btAttrib - 1);
			}
		else
			{
			// 赋予tile属性
			sAttrib = i << (btAttrib - 1);
			}

		//_seekTile(nX, nY);
        _tile_attrib.attrib = sAttrib;
		fseek(_fp, - offset, SEEK_CUR);
		fwrite(&_tile_attrib.attrib, sizeof _tile_attrib.attrib, 1, _fp);
		}
	}

bool CTerrainAttrib::hasTileAttrib(int nX, int nY, unsigned char btAttrib)
	{
	if (!_validateTileAttrib(btAttrib))
		return false;

	if (_validateFilePointer() && _validateTile(nX, nY))
		{
		unsigned short i = 1;
		unsigned short j = 0;

		_seekTile(nX, nY);
		fread(&_tile_attrib.attrib, sizeof _tile_attrib.attrib, 1, _fp);

		j = _tile_attrib.attrib & (i << (btAttrib - 1));
		return (j == 0) ? false : true;
		}
	else return false;
	}

bool CTerrainAttrib::delTileAttrib(int nX, int nY, unsigned char btAttrib)
    {
    if (!_validateTileAttrib(btAttrib)) return false;

    // btAttrib = {1, ..., 16}
    if (_validateFilePointer() && _validateTile(nX, nY))
        {
        unsigned short sAttrib = 0;
        unsigned short i = 1;

        _seekTile(nX, nY);
        long offset = sizeof _tile_attrib.attrib;
        fread(&_tile_attrib.attrib, offset, 1, _fp);
        sAttrib = _tile_attrib.attrib;

        i <<= (btAttrib - 1);
        if (sAttrib & i) sAttrib ^= i;

        _tile_attrib.attrib = sAttrib;
        fseek(_fp, - offset, SEEK_CUR);
        fwrite(&_tile_attrib.attrib, sizeof _tile_attrib.attrib, 1, _fp);}
    return false;}


void CTerrainAttrib::setTileIsland(int nX, int nY, unsigned char btIsland)
	{
	if (!_validateTileIsland(btIsland))
		return;

	if (_validateFilePointer() && _validateTile(nX, nY))
		{
		// 定位到attrib
		_seekTile(nX, nY);

		// 定位到island
		_tile_attrib.island = btIsland;
		fseek(_fp, sizeof _tile_attrib.attrib, SEEK_CUR);
		fwrite(&_tile_attrib.island, sizeof _tile_attrib.island, 1, _fp);
		}
	}

unsigned char CTerrainAttrib::getTileIsland(int nX, int nY)
	{
	unsigned char btIsland = TILE_ISLAND_DEFAULT_VALUE;

	if (_validateFilePointer() && _validateTile(nX, nY))
		{
		// 定位到attrib
		_seekTile(nX, nY);

		// 定位到island
		fseek(_fp, sizeof _tile_attrib.attrib, SEEK_CUR);
		fread(&_tile_attrib.island, sizeof _tile_attrib.island, 1, _fp);
		btIsland = _tile_attrib.island;
		}

	return btIsland;
	}

// 单件CTAMgr
#include <iostream>
#include <memory>
class CTAMgr
	{
private:
	CTAMgr(CTAMgr const&) {}
	CTAMgr& operator =(CTAMgr const&) {}

public:
	CTAMgr() {}
	~CTAMgr() {}
	static CTAMgr* Instance();

public:
	// 核心对象
	std::unique_ptr<CTerrainAttrib> ta = std::make_unique <CTerrainAttrib>();

protected:
private:
	inline static std::unique_ptr<CTAMgr> _instance = nullptr;
	};

CTAMgr* CTAMgr::Instance()
	{
	if (!_instance )
		{
		_instance = std::make_unique<CTAMgr>();
		}

	return _instance.get();
	}

//
// 简单接口
//
bool createAttribFile(char const* filename, int width, int height, int option)
	{
	return CTAMgr::Instance()->ta->createFile(filename, width, height, option);
	}

bool openAttribFile(char const* filename)
	{
	return CTAMgr::Instance()->ta->openFile(filename);
	}

bool getAttribFileInfo(int& width, int& height)
	{
	CTAMgr* pTAMgr = CTAMgr::Instance();

	width = pTAMgr->ta->getWidth();
	height = pTAMgr->ta->getHeight();
	return true;
	}

bool getTileAttrib(int x, int y, unsigned short& attrib)
	{
	attrib = CTAMgr::Instance()->ta->getTileAttrib(x, y);
	return true;
	}

bool hasTileAttrib(int x, int y, unsigned char attrib)
	{
	return CTAMgr::Instance()->ta->hasTileAttrib(x, y, attrib);
	}

bool getTileIsland(int x, int y, unsigned char& index)
	{
	index = CTAMgr::Instance()->ta->getTileIsland(x, y);
	return true;
	}

void setTileAttrib(int nX, int nY, unsigned char btAttrib, bool bAdd /* = true */)
	{
	CTAMgr::Instance()->ta->setTileAttrib(nX, nY, btAttrib, bAdd);
	}

void setTileIsland(int nX, int nY, unsigned char btIsland)
	{
	CTAMgr::Instance()->ta->setTileIsland(nX, nY, btIsland);
	}

bool delTileAttrib(int x, int y, unsigned char attrib)
    {
    return CTAMgr::Instance()->ta->delTileAttrib(x, y, attrib);
    }






//////////////////////////////////////////////////////////////////////////
//
// add by claude at 2004-10-18 for loading terrain attrib all in memory
//
//////////////////////////////////////////////////////////////////////////

// class disk_stuff
class disk_stuff
    {
public:
    disk_stuff(char const* fname, char const* mode);
    virtual ~disk_stuff();

    virtual bool load();
    virtual bool chfile(char const* fname, char const* mode);

    int SEEK(long offset, int origin) {return fseek(_fp, offset, origin);}
    size_t READ(void *buffer, size_t size, size_t count)
        {return fread(buffer, size, count, _fp);}
    long TELL() {return ftell(_fp);}

protected:
    FILE* _fp;
    };

inline disk_stuff::disk_stuff(char const* fname, char const* mode)
    {
    _fp = fopen(fname, mode);
    if (_fp == NULL) throw std::runtime_error("disk_stuff constructor");
    }

inline disk_stuff::~disk_stuff() {fclose(_fp);}

inline bool disk_stuff::load() {return true;}

inline bool disk_stuff::chfile(char const* fname, char const* mode)
    {
    fclose(_fp); _fp = fopen(fname, mode);
    if (_fp == NULL) throw std::runtime_error("disk_stuff chfile");
    return true;
    }

// class cache_stuff
class cache_stuff : public disk_stuff
    {
public:
    cache_stuff(char const* fname, char const* mode);
    virtual ~cache_stuff();

    virtual bool load();
    };

inline cache_stuff::cache_stuff(char const* fname, char const* mode)
try : disk_stuff(fname, mode) {}
catch (bad_exception& be) {throw be;}
catch (...) {throw std::runtime_error("other exception in cache_stuff");};

inline cache_stuff::~cache_stuff() {}

inline bool cache_stuff::load() {return disk_stuff::load();}

// class mem_stuff
class mem_stuff : public disk_stuff
    {
public:
    mem_stuff(char const* fname, char const* mode);
    virtual ~mem_stuff();

    virtual bool load();
    virtual bool chfile(char const* fname, char const* mode = "rb");

protected:
    unsigned char* _dat;
    unsigned int _len;
    };

inline mem_stuff::mem_stuff(char const* fname, char const* mode)
try : disk_stuff(fname, mode) {_dat = NULL; _len = 0;}
catch (bad_exception& be) {throw be;}
catch (...) {throw std::runtime_error("other exception in mem_stuff");};

inline mem_stuff::~mem_stuff() {if (_dat) delete _dat;}

inline bool mem_stuff::load()
// read all data into heap
    {
    if (_dat != NULL) return true;

    // get file size
    SEEK(0, SEEK_END);
    long len = TELL();

    // allocate the heap
    unsigned char* p;
    try {p = new unsigned char[len];}
    catch (bad_alloc& ba) {(ba); throw std::runtime_error("no heap to new");}
    catch (...) {throw std::runtime_error("other exception to new in mem_stuff::load");}

    // allocate heap successfully, so read all data from disk
    SEEK(0, SEEK_SET);
    if (READ(p, 1, len) < size_t(len))
        {delete p; throw std::runtime_error("fread fault in mem_stuff::load");}

    // all data read into memory successfully
    _dat = p; _len = len; return true;}

inline bool mem_stuff::chfile(char const* fname, char const* mode /* = "rb" */)
    {
    disk_stuff::chfile(fname, mode);

    // load the new file
    SEEK(0, SEEK_END);
    if (_len != TELL()) return false;

    SEEK(0, SEEK_SET);
    if (READ(_dat, 1, _len) < size_t(_len))
        {delete _dat; _dat = NULL; 
        throw std::runtime_error("fread fault in mem_stuff::chfile");}

    return true;}

// 区域属性基本接口定义
class terrain_attr
    {
protected:
    terrain_attr() {_hdr = NULL;}
    virtual ~terrain_attr() {};

public:
    virtual bool get_attr(unsigned int x, unsigned int y, unsigned short& attrib) = 0;
    virtual bool has_attr(unsigned int x, unsigned int y, unsigned char attrib_mask) = 0;
    virtual bool get_island(unsigned int x, unsigned int y, unsigned char& island) = 0;
    virtual bool get_info(unsigned int& width, unsigned int& height) = 0;

protected:
    terrain_attr_hdr* _hdr;
    };

// 区域属性全部装入内存，直接从内存中读取
class terrain_attr_mem : public terrain_attr, public mem_stuff
    {
public:
    terrain_attr_mem(char const* fname);
    ~terrain_attr_mem();

    bool get_attr(unsigned int x, unsigned int y, unsigned short& attrib);
    bool has_attr(unsigned int x, unsigned int y, unsigned char attrib_mask);
    bool get_island(unsigned int x, unsigned int y, unsigned char& island);
    bool get_info(unsigned int& width, unsigned int& height);
    };

inline terrain_attr_mem::terrain_attr_mem(char const* fname)
try : terrain_attr(), mem_stuff(fname, "rb") {load(); _hdr = (terrain_attr_hdr *)_dat;}
catch (bad_exception& ba) {throw ba;}
catch (...) {throw std::runtime_error("exception in terrain_attr_mem constructor");};

inline terrain_attr_mem::~terrain_attr_mem() {}

inline bool terrain_attr_mem::get_attr(unsigned int x, unsigned int y, unsigned short& attrib)
    {
    if (_dat == NULL || x >= _hdr->width || y >= _hdr->height) return false;

    int offset = sizeof terrain_attr_hdr +
        (sizeof terrain_attr_dat) * (y * _hdr->width + x);
    attrib = ((terrain_attr_dat *)(_dat + offset))->attrib;
    return true;}

inline bool terrain_attr_mem::has_attr(unsigned int x, unsigned int y, unsigned char attrib_mask)
    {
    if (_dat == NULL || x >= _hdr->width || y >= _hdr->height
        || attrib_mask < 1 || attrib_mask > 16) return false;

    unsigned short i = 1;
    int offset = sizeof terrain_attr_hdr +
        (sizeof terrain_attr_dat) * (y * _hdr->width + x);
    unsigned short j = ((terrain_attr_dat *)(_dat + offset))->attrib &
        (i << (attrib_mask - 1));
    return (j == 0) ? false : true;}

inline bool terrain_attr_mem::get_island(unsigned int x, unsigned int y, unsigned char& island)
    {
    if (_dat == NULL || x >= _hdr->width || y >= _hdr->height) return false;

    int offset = sizeof terrain_attr_hdr +
        (sizeof terrain_attr_dat) * (y * _hdr->width + x);
    island = ((terrain_attr_dat *)(_dat + offset))->island;
    return true;}

inline bool terrain_attr_mem::get_info(unsigned int& width, unsigned int& height)
    {
    width = _hdr->width;
    height = _hdr->height;
    return true;}

// 处理terrain_attr_mem的单件
class tamem_mgr
    {
    friend class cfl_singleton_ap<tamem_mgr>;

private:
    tamem_mgr(tamem_mgr const&) {}
    tamem_mgr& operator =(tamem_mgr const&) {}

public:
	tamem_mgr() : _ta_map()
	{
		memset(_ta, NULL, sizeof _ta); _ta_cnt = 0;
	}
	~tamem_mgr()
	{
		for (int i = 0; i < _ta_cnt; ++i) delete _ta[i];
	}
    bool validate(int id)
        {
        if ((id >= 0) && (id < _ta_cnt)) return true;
        else {
            ToLogService("common", "Fault: [id={}], wrong map index", id);
            return false;}
        }

    // 装载区域属性文件进内存
    int load_map(char const* fname);

    // 得到区域属性文件信息
    bool get_info(int id, unsigned int& width, unsigned int& height)
        {
        return (validate(id)) ? _ta[id]->get_info(width, height) : false;}

    // 区域属性
    bool get_trrnattr(int id, unsigned int x, unsigned int y, unsigned short& attr)
        {
        return (validate(id)) ? _ta[id]->get_attr(x, y, attr) : false;}

    bool get_trrnattr(char const* fname, unsigned int x, unsigned int y, unsigned short& attr)
        {return true;}

    bool has_trrnattr(int id, unsigned int x, unsigned int y, unsigned char mask)
        {
        return (validate(id)) ? _ta[id]->has_attr(x, y, mask) : false;}

    // 海岛编号
    bool get_isldidx(int id, unsigned int x, unsigned int y, unsigned char& isldidx)
        {
        return (validate(id)) ? _ta[id]->get_island(x, y, isldidx) : false;}

protected:
#define INVALID_INDEX (-1)
    enum {MAX_MAP = 100};
    int _ta_cnt;
    terrain_attr_mem* _ta[MAX_MAP + 1];
    map<string, int> _ta_map;

    int _find(char const* fname)
        {
        map<string, int>::iterator it = _ta_map.find(fname);
        if (it != _ta_map.end()) return (*it).second;
        else return INVALID_INDEX;}

    }; typedef cfl_singleton_ap<tamem_mgr> TAM_MGR;

int tamem_mgr::load_map(char const* fname)
    {
    if (_ta_cnt >= MAX_MAP) return INVALID_INDEX;

    int id = _find(fname);
    if (id != INVALID_INDEX) return id;
    else { // 新装入内存
        terrain_attr_mem* tmp;
        try {tmp = new terrain_attr_mem(fname);}
        catch (...)
            {
            ToLogService("common", "Load attrib file: [{}] failed", fname);
            return INVALID_INDEX;}

        ToLogService("common", "Attrib file: [id={}] -> [{}]", _ta_cnt, fname);
        _ta[_ta_cnt] = tmp;
        _ta_map[fname] = _ta_cnt;
        id = _ta_cnt;
        ++ _ta_cnt;
        return id;}
    }

int s_openAttribFile(char const* filename)
    {
    string strfile = filename; strfile += ".atr";
    return TAM_MGR::instance()->load_map(strfile.c_str());}

bool s_getTileAttrib(int id, unsigned int x, unsigned int y, unsigned short& attrib)
    {return TAM_MGR::instance()->get_trrnattr(id, x, y, attrib);}

bool s_hasTileAttrib(int id, unsigned int x, unsigned int y, unsigned char attrib_mask)
    {return TAM_MGR::instance()->has_trrnattr(id, x, y, attrib_mask);}

bool s_getTileIsland(int id, unsigned int x, unsigned int y, unsigned char& island_index)
    {return TAM_MGR::instance()->get_isldidx(id, x, y, island_index);}

bool s_getAttribFileInfo(int id, unsigned int& width, unsigned int& height)
    {return TAM_MGR::instance()->get_info(id, width, height);}
