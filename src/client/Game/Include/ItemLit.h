//
#pragma once

struct lwLitInfo
{
    DWORD id;
    char file[LW_MAX_FILE];
    DWORD anim_type;
    DWORD transp_type;
    float opacity;
};

struct lwItemLitInfo
{
    typedef std::vector<lwLitInfo*> _LitBuf;
    typedef _LitBuf::iterator _LitBuf_It;

    lwItemLitInfo()
    {
        id = 0;
        descriptor[0] = '\0';
        file[0] = '\0';
    }
    ~lwItemLitInfo()
    {
        _LitBuf_It it = lit.begin();
        _LitBuf_It it_e = lit.end();
        for(; it != it_e; ++it)
        {
            LW_DELETE((*it));
        }
    }

    _LitBuf_It FindLitWithId(DWORD id)
    {
        _LitBuf_It it = lit.begin();
        _LitBuf_It it_e = lit.end();
        for(; it != it_e; ++it)
        {
            if((*it)->id == id)
                break;
        }
        return it;
    }


    DWORD id;
    char descriptor[LW_MAX_NAME];
    char file[LW_MAX_FILE];
    _LitBuf lit;
};

struct lwItemLitFileHead
{
    DWORD version;
    DWORD type;
    DWORD mask[4];
};

class lwItemLit
{
    typedef std::vector<lwItemLitInfo*> _ItemBuf;
    typedef _ItemBuf::iterator _ItemBuf_It;
    _ItemBuf _item;

_ItemBuf_It _FindItemWithId(DWORD id);
public:
    lwItemLit();
    ~lwItemLit();

    LW_RESULT Clear();
    LW_RESULT Load(const char* file);
    LW_RESULT Save(const char* file);

    LW_RESULT GetItemNum(DWORD* num);
    LW_RESULT GetItemWithId(DWORD id, lwItemLitInfo** info);
    LW_RESULT FindItem(DWORD id, lwItemLitInfo** info);
    LW_RESULT AddItem(DWORD id, const char* desc, lwItemLitInfo** ret_info);
    LW_RESULT RemoveItem(DWORD item_id);
    LW_RESULT RemoveItemLit(DWORD item_id, DWORD lit_id);
    
};
