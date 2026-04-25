//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwMath.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"
#include "lwITypes.h"

LW_BEGIN

template< DWORD set_size, DWORD seq_size >
struct lwRenderStateSetTemplate
{
    enum { 
        SET_SIZE = set_size,
        SEQUENCE_SIZE = seq_size,
    };

    lwRenderStateValue rsv_seq[SET_SIZE][SEQUENCE_SIZE];
};

template< DWORD set_size, DWORD seq_size >
inline void lwRenderStateSetTemplate_Construct(lwRenderStateSetTemplate< set_size, seq_size >* obj)
{
    typedef lwRenderStateSetTemplate<set_size, seq_size> type_value;

    for(DWORD i = 0; i < type_value::SET_SIZE; i++)
    {
        for(DWORD j = 0; j < type_value::SEQUENCE_SIZE; j++)
        {
            obj->rsv_seq[i][j].state = LW_INVALID_INDEX;
            obj->rsv_seq[i][j].state = LW_INVALID_INDEX;
        }
    }
}
template< DWORD set_size, DWORD seq_size >
LW_RESULT lwSetRenderStateSet(lwRenderStateSetTemplate< set_size, seq_size >* obj, DWORD set_id, DWORD state, DWORD value)
{
    typedef lwRenderStateSetTemplate<set_size, seq_size> type_value;

    DWORD i;
    lwRenderStateValue* rsv_seq = obj->rsv_seq[set_id];
    lwRenderStateValue* rsv;
    
    for(i = 0; i < type_value::SEQUENCE_SIZE; i++)
    {
        rsv = &rsv_seq[i];

        if(rsv->state == state)
        {
            rsv->value = value;
            goto __ret;
        }
    }

    for(i = 0; i < type_value::SEQUENCE_SIZE; i++)
    {
        rsv = &rsv_seq[i];

        if(rsv->state == LW_INVALID_INDEX)
        {
            rsv->state = state;
            rsv->value = value;
            goto __ret;
        }
    }

__ret:
    return i == type_value::SEQUENCE_SIZE ? LW_RET_FAILED : LW_RET_OK;

}

template< DWORD set_size, DWORD seq_size >
LW_RESULT lwClearRenderStateSet(lwRenderStateSetTemplate< set_size, seq_size >* obj, DWORD set_id, DWORD state)
{
    typedef lwRenderStateSetTemplate<set_size, seq_size> type_value;

    DWORD i;
    lwRenderStateValue* rsv;
    lwRenderStateValue* rsv_seq = obj->rsv_seq[set_id];

    for(i = 0; i < type_value::SEQUENCE_SIZE; i++)
    {
        rsv = &rsv_seq[i];

        if(rsv->state == state)
        {
            for(DWORD j = i; j < type_value::SEQUENCE_SIZE - 1; j++)
            {
                if(rsv_seq[j + 1].state == LW_INVALID_INDEX)
                    goto __ret;

                rsv_seq[j] = rsv_seq[j + 1];
            }

            goto __ret;
        }
    }

__ret:

    return i == type_value::SEQUENCE_SIZE ? LW_RET_FAILED : LW_RET_OK;


}

typedef lwRenderStateSetTemplate< 2, LW_MESH_RS_NUM > lwRenderStateSetMesh2;
typedef lwRenderStateSetTemplate< 2, LW_MTL_RS_NUM > lwRenderStateSetMtl2;
typedef lwRenderStateSetTemplate< 2, LW_TEX_TSS_NUM > lwTextureStageStateSetTex2;
typedef lwRenderStateSetTemplate< 2, LW_TEX_RS_NUM > lwRenderStateSetTex2;

typedef lwRenderStateSetMtl2 lwMtlDRS;
typedef lwRenderStateSetTemplate< 2, LW_TEX_DTSS_NUM > lwTexDTSS;
typedef lwRenderStateSetTemplate< 2, LW_TEX_DRS_NUM > lwTexDRS;


struct lwRenderStateAtom
{
    DWORD state;
    DWORD value0;
    DWORD value1;
};
void inline lwRenderStateAtom_Construct(lwRenderStateAtom* obj)
{
    obj->state = LW_INVALID_INDEX;
    obj->value0 = 0;
    obj->value1 = 0;
}
void inline lwRenderStateAtom_Construct_A(lwRenderStateAtom* obj_seq, DWORD num)
{
    for(DWORD i = 0; i < num; i++)
    {
        lwRenderStateAtom_Construct(&obj_seq[i]);
    }
}


inline void RSA_VALUE(lwRenderStateAtom* obj, DWORD state, DWORD value)
{
    obj->state = state;
    obj->value0 = value;
    obj->value1 = value;
}

enum lwTexInfoTypeEnum
{
    TEX_TYPE_FILE =         0,
    TEX_TYPE_SIZE,
    TEX_TYPE_DATA,
    TEX_TYPE_INVALID =      LW_INVALID_INDEX,
};

//  Стратегия загрузки текстуры в видеопамять (см. lwTex::LoadVideoMemory).
//  Поле lwTex::_loadMode хранит выбранный режим. Сейчас переключатель
//  не активирован ни одним колсайтом (SetLoadMode не вызывается),
//  но сам выбор путей в LoadVideoMemory сохранён ради читаемости.
enum class TextureLoadMode : std::uint32_t
{
    //  Дефолт. DDS-ориентированный путь: ищет .dds-вариант файла рядом с
    //  .bmp/.tga (если есть, использует его) и идёт через D3DX — DXT-сжатие
    //  и mip-цепочка из DDS доезжают до VRAM как есть. Обычные форматы
    //  тоже подхватываются D3DX, но смысл этой ветки — именно DDS
    //  (компрессированные текстуры).
    LOAD_TEXTURE_DDS = 0,

    //  Пользовательские raster-изображения. Декодит BMP/TGA/PNG через
    //  ImageLoader (stb_image), приводит к нужному D3DFORMAT
    //  (R5G6B5/A1R5G5B5/A4R4G4B4/X8R8G8B8/A8R8G8B8) и заливает в текстуру
    //  через dev->CreateTexture. DDS этот путь не поддерживает.
    LOAD_TEXTURE_USER_IMAGE = 1,
};

struct lwTexFileInfo
{
};
struct lwTexMemInfo
{
};

struct lwTexInfo
{
    DWORD stage;
    DWORD level;
    DWORD usage;
    D3DFORMAT format;
    D3DPOOL pool;
    DWORD byte_alignment_flag;
    DWORD type; // file texture or user-defined texture
    // user-defined texture
    DWORD width; // TEX_TYPE_DATAwidthdata size
    DWORD height;
    // file texture
    DWORD colorkey_type;
    lwColorValue4b colorkey;
    char file_name[LW_MAX_NAME];
    // Было `void* data;` — размер указателя отличается на x86/x64 и ломал
    // разбор бинарных файлов моделей (.lgo/.lmo), сохранённых с x86-раскладкой.
    // Поле не использовалось как реальный указатель (ставилось только в 0),
    // поэтому заменено на 4-байтный плейсхолдер, фиксирующий бинарный формат.
    DWORD _reserved_data;

    lwRenderStateAtom tss_set[LW_TEX_TSS_NUM];
};

inline void lwTexInfo_Construct(lwTexInfo* obj)
{
    obj->stage = LW_INVALID_INDEX;
    obj->level = 0;
    obj->usage = 0;
    obj->format = D3DFMT_UNKNOWN;
    obj->pool = D3DPOOL_FORCE_DWORD;
    obj->byte_alignment_flag = 0;
    obj->type = TEX_TYPE_INVALID;

    obj->width = 0;
    obj->height = 0;

    obj->colorkey_type = COLORKEY_TYPE_NONE;
    obj->colorkey.color = 0;
    obj->file_name[0] = '\0';
    obj->_reserved_data = 0;

    //lwRenderStateSetTemplate_Construct(&obj->tss_set);
    lwRenderStateAtom_Construct_A(obj->tss_set, LW_TEX_TSS_NUM);
}

struct lwMtlInfo
{
    lwMaterial mtl;

    lwRenderStateSetMtl2 rs_set;
};

struct lwBlendInfo
{
    union {
        BYTE index[4];
        DWORD indexd;
    };
    float weight[4];
};

struct lwSubsetInfo
{
    DWORD primitive_num;
    DWORD start_index;
    DWORD vertex_num;
    DWORD min_index;
};

inline void lwSubsetInfo_Construct(lwSubsetInfo* obj, DWORD pri_num, DWORD start_index, DWORD vertex_num, DWORD min_index)
{
    obj->primitive_num = pri_num;
    obj->start_index = start_index;
    obj->vertex_num = vertex_num;
    obj->min_index = min_index;
}
struct lwBoundingBoxInfo
{
    DWORD id;
    lwBox box;
    lwMatrix44 mat;
};

struct lwBoundingSphereInfo
{
    DWORD id;
    lwSphere sphere;
    lwMatrix44 mat;
};

struct lwIndexMatrix44
{
    DWORD id;
    lwMatrix44 mat;
};

struct lwHelperDummyInfo
{
    DWORD id;
    lwMatrix44 mat;
    lwMatrix44 mat_local;
    DWORD parent_type;  // 0: default, 1: bone parent, 2: bone dummy parent
    DWORD parent_id;
};

struct lwHelperMeshFaceInfo
{
    DWORD vertex[3];
    DWORD adj_face[3];

    lwPlane plane;
    lwVector3 center;
};

struct lwHelperMeshInfo
{
    lwHelperMeshInfo()
        : id(LW_INVALID_INDEX), type(0), sub_type(0), state(1),
        vertex_seq(0), face_seq(0), vertex_num(0), face_num(0)
    {
        name[0] = '\0';
    }

    ~lwHelperMeshInfo()
    {
        LW_SAFE_DELETE_A(vertex_seq);
        LW_SAFE_DELETE_A(face_seq);
    }

    LW_RESULT Copy(const lwHelperMeshInfo* src);

    DWORD id;
    DWORD type; // helper mesh type
    char name[LW_CHAR_32];
    DWORD state;
    DWORD sub_type;
    lwMatrix44 mat;
    lwBox box;
    lwVector3* vertex_seq;
    lwHelperMeshFaceInfo* face_seq;

    DWORD vertex_num;
    DWORD face_num;

};

struct lwHelperBoxInfo
{
    lwHelperBoxInfo()
        : id(0), type(0), state(1)
    {
        name[0] = '\0';
    }

    DWORD id;
    DWORD type;
    DWORD state;
    lwBox box;
    lwMatrix44 mat;
    char name[LW_CHAR_32];

    LW_RESULT Copy(const lwHelperBoxInfo* src) { memcpy(this, src, sizeof(lwHelperBoxInfo)); return LW_RET_OK; }
};

struct lwMeshInfo
{
    struct lwMeshInfoHeader
    {
        DWORD fvf;
        D3DPRIMITIVETYPE pt_type;

        DWORD vertex_num;
        DWORD index_num;
        DWORD subset_num;
        DWORD bone_index_num;
        DWORD bone_infl_factor;
        DWORD vertex_element_num;

        lwRenderStateAtom rs_set[LW_MESH_RS_NUM];
    };

    union
    {
        lwMeshInfoHeader header;

        struct
        {
            DWORD fvf;
            D3DPRIMITIVETYPE pt_type;

            DWORD vertex_num;
            DWORD index_num;
            DWORD subset_num;
            DWORD bone_index_num;
            DWORD bone_infl_factor;
            DWORD vertex_element_num;

            lwRenderStateAtom rs_set[LW_MESH_RS_NUM];
        };
    };

    lwVector3* vertex_seq;
    lwVector3* normal_seq;
    union 
    {
        lwVector2* texcoord_seq[LW_MAX_TEXTURESTAGE_NUM];
        struct 
        {
            lwVector2* texcoord0_seq;
            lwVector2* texcoord1_seq;
            lwVector2* texcoord2_seq;
            lwVector2* texcoord3_seq;
        };
    };
    DWORD* vercol_seq;
    DWORD* index_seq;
    DWORD* bone_index_seq;
    lwBlendInfo* blend_seq;
    lwSubsetInfo* subset_seq;
    D3DVERTEXELEMENTX* vertex_element_seq;


    lwMeshInfo()
        : vertex_seq(0), normal_seq(0), vercol_seq(0), index_seq(0), 
        subset_seq(0), blend_seq(0), bone_index_seq(0),
        texcoord0_seq(0), texcoord1_seq(0), texcoord2_seq(0), texcoord3_seq(0),
        vertex_num(0), index_num(0), subset_num(0), bone_index_num(0), 
        bone_infl_factor(0), vertex_element_seq(0), vertex_element_num(0)
    {
        lwRenderStateAtom_Construct_A(rs_set, LW_MESH_RS_NUM);
    }

    ~lwMeshInfo()
    {
        LW_IF_DELETE_A(vertex_seq);
        LW_IF_DELETE_A(normal_seq);
        LW_IF_DELETE_A(vercol_seq);
        LW_IF_DELETE_A(texcoord0_seq);
        LW_IF_DELETE_A(texcoord1_seq);
        LW_IF_DELETE_A(texcoord2_seq);
        LW_IF_DELETE_A(texcoord3_seq);
        LW_IF_DELETE_A(index_seq);
        LW_IF_DELETE_A(blend_seq);        
        LW_IF_DELETE_A(bone_index_seq);        
        LW_IF_DELETE_A(subset_seq);        
        LW_IF_DELETE_A(vertex_element_seq); 
    }

    void ResetIndexBuffer(const DWORD* buf)
    {
        memcpy(index_seq, buf, sizeof(DWORD) * index_num);
    }

};
typedef lwMeshInfo::lwMeshInfoHeader lwMeshInfoHeader;

inline void lwMeshInfo_Construct(lwMeshInfo* obj)
{
    obj->fvf = 0;
    obj->pt_type = D3DPT_FORCE_DWORD;
    obj->vertex_num = 0;
    obj->subset_num = 0;
    obj->index_num = 0;
    obj->bone_index_num = 0;
    obj->bone_infl_factor = 0;
    obj->vertex_seq = 0;
    obj->normal_seq = 0;
    obj->vercol_seq = 0;
    obj->blend_seq = 0;
    obj->index_seq = 0;
    obj->texcoord0_seq = 0;
    obj->texcoord1_seq = 0;
    obj->texcoord2_seq = 0;
    obj->texcoord3_seq = 0;
    obj->subset_seq = 0;
    obj->bone_index_seq = 0;
    obj->vertex_element_seq = 0;
    obj->vertex_element_num = 0;
    lwRenderStateAtom_Construct_A(obj->rs_set, LW_MESH_RS_NUM);
}

inline void lwMeshInfo_Destruct(lwMeshInfo* obj)
{
    LW_IF_DELETE_A(obj->vertex_seq);
    LW_IF_DELETE_A(obj->normal_seq);
    LW_IF_DELETE_A(obj->vercol_seq);
    LW_IF_DELETE_A(obj->texcoord0_seq);
    LW_IF_DELETE_A(obj->texcoord1_seq);
    LW_IF_DELETE_A(obj->texcoord2_seq);
    LW_IF_DELETE_A(obj->texcoord3_seq);
    LW_IF_DELETE_A(obj->index_seq);
    LW_IF_DELETE_A(obj->blend_seq);        
}

//enum lwMtlTexInfoTransparencyTypeEnum
//{
//    MTLTEX_TRANSP_FILTER =          0,
//    MTLTEX_TRANSP_ADDITIVE =        1,
//    MTLTEX_TRANSP_SUBTRACTIVE =     2,
//};

enum lwMtlTexInfoTransparencyTypeEnum
{
    MTLTEX_TRANSP_FILTER =          0,
    MTLTEX_TRANSP_ADDITIVE =        1,
    MTLTEX_TRANSP_ADDITIVE1 =       2,
    MTLTEX_TRANSP_ADDITIVE2 =       3,
    MTLTEX_TRANSP_ADDITIVE3 =       4,
    MTLTEX_TRANSP_SUBTRACTIVE =     5,
    MTLTEX_TRANSP_SUBTRACTIVE1 =    6,
    MTLTEX_TRANSP_SUBTRACTIVE2 =    7,
    MTLTEX_TRANSP_SUBTRACTIVE3 =    8,
};

struct lwMtlTexInfo
{
    float opacity;
    DWORD transp_type;
    lwMaterial mtl;
    //lwRenderStateSetMtl2 rs_set;
    lwRenderStateAtom rs_set[LW_MTL_RS_NUM];
    lwTexInfo tex_seq[LW_MAX_TEXTURESTAGE_NUM];

    lwMtlTexInfo()
    {
        opacity = 1.0f;
        transp_type = MTLTEX_TRANSP_FILTER;
        lwMaterial_Construct(&mtl);
        lwRenderStateAtom_Construct_A(rs_set, LW_MTL_RS_NUM);

        lwTexInfo_Construct(&tex_seq[0]);
        lwTexInfo_Construct(&tex_seq[1]);
        lwTexInfo_Construct(&tex_seq[2]);
        lwTexInfo_Construct(&tex_seq[3]);
    }     
};

enum lwObjectStateEnum
{
    STATE_VISIBLE =              0,
    STATE_ENABLE =               1,
    STATE_UPDATETRANSPSTATE =    3,
    STATE_TRANSPARENT =          4,
    STATE_FRAMECULLING =         5,

    STATE_INVALID =              LW_INVALID_INDEX,
    OBJECT_STATE_NUM =                  8
};

class lwStateCtrl
{
public:
    BYTE _state_seq[OBJECT_STATE_NUM];

public:
    lwStateCtrl()
    {
        SetDefaultState();
    }
    void SetDefaultState()
    {
        memset(_state_seq, 0, sizeof(_state_seq));
        _state_seq[STATE_VISIBLE] = 1;
        _state_seq[STATE_ENABLE] = 1;
        _state_seq[STATE_UPDATETRANSPSTATE] = 1;
        _state_seq[STATE_TRANSPARENT] = 0;
        _state_seq[STATE_FRAMECULLING] = 0;
    }
    inline void SetState(DWORD state, BYTE value){ _state_seq[state] = value; }
    inline BYTE GetState(DWORD state) const { return _state_seq[state]; }
};

class lwIBuffer;
struct lwSysMemTexInfo
{
    DWORD level;
    DWORD usage;
    D3DFORMAT format;
    DWORD width;
    DWORD height;
    DWORD filter;
    DWORD mip_filter;
    DWORD colorkey;
    char file_name[LW_MAX_PATH];
    lwIBuffer* buf;
};


LW_END
