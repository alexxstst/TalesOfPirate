//
#include "stdafx.h"
#include "lwgraphicsutil.h"

#include "tgacore.h"
#include "lwbitmap.h"
#include "lwfileutil.h"
#include "lwErrorCode.h"
#include "lwD3D.h"



LW_BEGIN

#ifndef USE_MINDPOWER

void lwMessageBox(const char* fmt, ...)
{
    char buf[512];
    
    va_list args;
    va_start(args, fmt);
    _vsntprintf(buf, 512, fmt, args);
    va_end(args);

    MessageBox(NULL, buf, "error", MB_OK);
}
#endif
//
float lwGetFPS()
{
    static double __tick_frequency_reci = 0.0; // 1000ms
	static DWORD __last_tick_time = 0;
	static DWORD __frame = 0;
	static float __fps = 0.0f;

	DWORD cur_time;
	DWORD interval;
	
    LARGE_INTEGER timetick;
    QueryPerformanceCounter(&timetick);

    cur_time = (DWORD)(timetick.QuadPart * __tick_frequency_reci);

 	if(__last_tick_time == 0) 
    {
        LARGE_INTEGER timefreq;
        QueryPerformanceFrequency(&timefreq);
        __tick_frequency_reci = 1000.0 / (double)timefreq.QuadPart;

        __last_tick_time = cur_time;
	}

	
	__frame++;
	interval = cur_time - __last_tick_time;

	if(interval >= 500) 
    {
		__fps = (float)((float)__frame / interval) * 1000.0f;
		__frame = 0;
		__last_tick_time = cur_time;
	}

	return __fps;
}

void lwConvertTgaDataToColorValue(lwColorValue4b* dst_data, const CGraphicsFileData* tga, DWORD colorkey_type, lwColorValue4b* colorkey)
{
	int size = tga->m_FileHeader.Width * tga->m_FileHeader.Height;

    lwColorValue4b* dst;
    lwColorValue4b* dst_0;
    CGraphicsFileData::stFileData* src;

    dst_0 = &dst_data[0];

	for(int i=0; i<size; i++) 
    {
        dst = &dst_data[i];
        src = &tga->m_pstFileData[i];

        dst->r = src->R;
		dst->g = src->G;
		dst->b = src->B;
        dst->a = src->A;

        if(colorkey_type == COLORKEY_TYPE_COLOR)
        {
            if(dst->r == colorkey->r && dst->g == colorkey->g && dst->b == colorkey->b)
            {
                dst->color = 0x00000000;
            }
        }
        else if(colorkey_type == COLORKEY_TYPE_PIXEL)
        {
            if(dst->r == dst_0->r && dst->g == dst_0->g && dst->b == dst_0->b) 
            {
                dst->color = 0x00000000;
            }
        }
	}
}

int lwLoadColorValue(lwColorValue4b** buf, int* width, int* height, const char* file, DWORD colorkey_type, lwColorValue4b* colorkey)
{
	int img_height, img_width;

	lwColorValue4b* img_data;

	char ext[8];
	lwGetPathFileNameExt(ext, file);

	if(_tcsicmp(ext, "tga") == 0) {
		// tga file
		CGraphicsFileData tga;
		
        if(tga.LoadTGAFile2(file) == 0)
            return 0;

		img_height = tga.m_FileHeader.Height;
		img_width = tga.m_FileHeader.Width;

		img_data = LW_NEW(lwColorValue4b[img_height * img_width]);

		lwConvertTgaDataToColorValue(img_data, &tga, colorkey_type, colorkey);
	}
	else if(_tcsicmp(ext, "bmp") == 0) {
		// bmp file
		lwBitmap bm;
		
        if(bm.Load(file, colorkey_type, colorkey) == 0)
            return 0;

		img_height = bm._bmih.biHeight;
		img_width = bm._bmih.biWidth;

		img_data = bm.Detach();

	}
    else {
        return 0;
    }

    *buf = img_data;
    *width = img_width;
    *height = img_height;

    return 1;
}

void lwFreeColorValue(lwColorValue4b* buf)
{
    LW_DELETE_A(buf);
}
//

DWORD lwGetD3DFormatStride(DWORD format)
{
    DWORD stride;

    switch(format)
    {
    case D3DFMT_R5G6B5:
        stride = sizeof(WORD);
        break;
    case D3DFMT_A1R5G5B5:
        stride = sizeof(WORD);
        break;
    case D3DFMT_A4R4G4B4:
        stride = sizeof(WORD);
        break;
    case D3DFMT_X8R8G8B8:
        stride = sizeof(DWORD);
        break;
    case D3DFMT_A8R8G8B8:
        stride = sizeof(DWORD);
        break;
    default:
        LG_MSGBOX("undefined D3DFORMAT type, call jack");
        stride = 0;
    }

    return stride;
}

DWORD lwGetSurfaceSize(UINT width, UINT height, D3DFORMAT format)
{
    DWORD size = 0;

    switch(format)
    {
#if(defined LW_USE_DX9)
    case D3DFMT_A16B16G16R16:
        size = width * height * 8;
        break;
    case D3DFMT_A2R10G10B10:
        size = width * height * 4;
        break;
#endif
    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
    case D3DFMT_R8G8B8:
        size = width * height * 3;
        break;
    case D3DFMT_A1R5G5B5:
    case D3DFMT_X1R5G5B5:
    case D3DFMT_A4R4G4B4:
    case D3DFMT_X4R4G4B4:
    case D3DFMT_R5G6B5:
        size = width * height * 2;
        break;
    case D3DFMT_R3G3B2:
        size = width * height * 1;
        break;
    case D3DFMT_DXT1:
        size = width * height / 2;
        break;
    case D3DFMT_DXT2:
    case D3DFMT_DXT3:
    case D3DFMT_DXT4:
    case D3DFMT_DXT5:
        size = width * height * 1;
        break;
    default:
        size = 0;
    }

    return size;
}

DWORD lwGetTexFlexibleSize(DWORD size)
{
    DWORD ret;

    if(size > 1024)
        ret = 2048;
    else if(size > 512)
        ret = 1024;
    else if(size > 256)
        ret = 512;
    else if(size > 128)
        ret = 256;
    else if(size > 64)
        ret = 128;
    else if(size > 32)
        ret = 64;
    else if(size > 16)
        ret = 32;
    else if(size > 8)
        ret = 16;
    else if(size > 4)
        ret = 8;
    else if(size > 2)
        ret = 4;
    else
        ret = 2;

    return ret;
}

LW_RESULT lwLoadTexDataInfo(lwTexDataInfo* info, const char* file, DWORD format, DWORD colorkey_type, lwColorValue4b* colorkey, int use_power_size)
{
    DWORD img_width, img_height;
    lwColorValue4b* img_buf;

    if(lwLoadColorValue(&img_buf, (int*)&img_width, (int*)&img_height, file, colorkey_type, colorkey) == 0)
    {
        LG_MSGBOX("invalid texture file: %s when called lwLoadTexDataInfo", file);
        return LW_RET_FAILED;
    }

    int src_index;
    int dst_index;

    DWORD tex_width = img_width;
    DWORD tex_height = img_height;

    if(use_power_size)
    {
        tex_width = lwGetTexFlexibleSize(img_width);
        tex_height = lwGetTexFlexibleSize(img_height);
    }

    DWORD stride = lwGetD3DFormatStride(format);
    DWORD size = tex_width * tex_height * stride;

    BYTE* src_data = LW_NEW(BYTE[size]);
    DWORD src_pitch = tex_width * stride;

    WORD* data = (WORD*)src_data;
    DWORD* d_data = (DWORD*)src_data;

    lwColorValue4b* s;

    DWORD i, j;
    DWORD src_p, dst_p;

    switch(format) 
    {
    case D3DFMT_R5G6B5:
        for(i = 0; i < tex_height; i++) 
        {
            if(i >= img_height)
                continue;

            src_p = i * img_width;
            dst_p = i * src_pitch / 2;

            for(j = 0; j < tex_width; j++) 
            {
                if(j >= img_width)
                    break;

                src_index = src_p + j;
                dst_index = dst_p + j;

                s = &img_buf[src_index];
                data[dst_index] = ((s->r>>3)<<11) | ((s->g>>3)<<6) | (s->b>>3);
            }
        }
        break;
    case D3DFMT_A1R5G5B5:
        for(i = 0; i < tex_height; i++) 
        {
            if(i >= img_height)
                continue;

            src_p = i * img_width;
            dst_p = i * src_pitch / 2;

            for(j = 0; j < tex_width; j++) 
            {
                if(j >= img_width)
                    break;

                src_index = src_p + j;
                dst_index = dst_p + j;

                s = &img_buf[src_index];
                data[dst_index] = (s->a>>7)<<15 | (s->r>>3)<<10 | (s->g>>3)<<5 | (s->b>>3);
            }
        }
        break;
    case D3DFMT_A4R4G4B4:
        for(i = 0; i < tex_height; i++) 
        {
            if(i >= img_height)
                continue;

            src_p = i * img_width;
            dst_p = i * src_pitch / 2;

            for(j = 0; j < tex_width; j++) 
            {
                if(j >= img_width)
                    break;

                src_index = src_p + j;
                dst_index = dst_p + j;

                s = &img_buf[src_index];
                data[dst_index] = (s->a>>4)<<12 | (s->r>>4)<<8 | (s->g>>4)<<4 | (s->b>>4);
            }
        }
        break;
    case D3DFMT_X8R8G8B8:
    case D3DFMT_A8R8G8B8: // the same method
        for(i = 0; i < tex_height; i++) 
        {
            if(i >= img_height)
                continue;

            src_p = i * img_width;
            dst_p = i * src_pitch / 4;

            for(j = 0; j < tex_width; j++) 
            {
                if(j >= img_width)
                    break;

                src_index = src_p + j;
                dst_index = dst_p + j;

                s = &img_buf[src_index];
                d_data[dst_index] = s->color;
            }
        }
        break;
    }

    lwFreeColorValue(img_buf);

    info->data = src_data;
    info->size = size;
    info->pitch = src_pitch;
    info->width = tex_width;
    info->height = tex_height;

    return LW_RET_OK;

}

LW_RESULT _ConvertMeshDataVB(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
LW_RESULT _ConvertMeshDataIB(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);

void _ConvertMeshData_xn(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xwdt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xwdt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xd(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xnt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xnt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xdt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xdt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xndt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xndt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb1dt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb1dt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xunt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb2unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb3unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb4unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);

void _ConvertMeshData_xunt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb2unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb3unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
void _ConvertMeshData_xb4unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info);
//
LW_RESULT lwExtractMeshData(lwMeshInfo* info, void* vb_data, void* ib_data, DWORD vert_num, DWORD index_num, D3DFORMAT vb_fvf, D3DFORMAT ib_fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(vert_num > 0)
    {
        switch(vb_fvf)
        {
        case FVF_XYZRHWDIFFUSETEX1:
            {
                typedef lwFVFStruct_XyzwDiffuseTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);


                for(DWORD i = 0; i < vert_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = *(lwVector3*)&p->pos;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                }

            }
            break;
        case FVF_XYZRHWDIFFUSETEX2:
            {
                typedef lwFVFStruct_XyzwDiffuseTex2 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->texcoord1_seq = LW_NEW(lwVector2[vert_num]);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = *(lwVector3*)&p->pos;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                    info->texcoord1_seq[i] = p->texcoord0;
                }
            }
            break;
        case FVF_XYZDIFFUSE:
            {
                typedef lwFVFStruct_XyzDiffuse _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->vercol_seq[i] = p->dif;
                }
            }
            break;
        case FVF_XYZNORMALTEX1:
            {
                typedef lwFVFStruct_XyzNormalTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->normal_seq = LW_NEW(lwVector3[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->normal_seq[i] = p->normal;
                    info->texcoord0_seq[i] = p->texcoord0;
                }

            }
            break;
        case FVF_XYZDIFFUSETEX1:
            {
                typedef lwFVFStruct_XyzDiffuseTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);


                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                }
            }
            break;
        case FVF_XYZDIFFUSETEX2:
            {
                typedef lwFVFStruct_XyzDiffuseTex2 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->texcoord1_seq = LW_NEW(lwVector2[vert_num]);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    p->pos = info->vertex_seq[i];
                    p->dif = info->vercol_seq[i];
                    p->texcoord0 = info->texcoord0_seq[i];
                    p->texcoord1 = info->texcoord1_seq[i];
                }

            }
            break;
        case FVF_XYZNORMALDIFFUSETEX1:
            {
                typedef lwFVFStruct_XyzNormalDiffuseTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->normal_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->normal_seq[i] = p->normal;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                }

            }
            break;
        case FVF_XYZUBYTE4NORMALTEX1:
            {
                typedef lwFVFStruct_XyzUbyte4NormalTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->normal_seq = LW_NEW(lwVector3[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
                memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->normal_seq[i] = p->normal;
                    info->texcoord0_seq[i] = p->texcoord0;
                    info->blend_seq[i].indexd = p->index;
                }
            }
            break;
        case FVF_XYZB1DIFFUSETEX1:
            {
                typedef lwFVFStruct_Xyzb1DiffuseTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
                memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->blend_seq[i].weight[0] = p->weight;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                }
            }
            break;
        case FVF_XYZB1DIFFUSETEX2:
            {
                typedef lwFVFStruct_Xyzb1DiffuseTex2 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->vercol_seq = LW_NEW(DWORD[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->texcoord1_seq = LW_NEW(lwVector2[vert_num]);
                info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
                memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->blend_seq[i].weight[0] = p->weight;
                    info->vercol_seq[i] = p->dif;
                    info->texcoord0_seq[i] = p->texcoord0;
                    info->texcoord1_seq[i] = p->texcoord1;
                }
            }
            break;
        case FVF_XYZB2UBYTE4NORMALTEX1:
            {
                typedef lwFVFStruct_Xyzb2Ubyte4NormalTex1 _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
                _FVF_STRUCT* p;

                info->vertex_num = vert_num;
                info->vertex_seq = LW_NEW(lwVector3[vert_num]);
                info->normal_seq = LW_NEW(lwVector3[vert_num]);
                info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
                info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
                memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

                for(DWORD i = 0; i < info->vertex_num; i++)
                {
                    p = &buf[i];
                    info->vertex_seq[i] = p->pos;
                    info->normal_seq[i] = p->normal;
                    info->texcoord0_seq[i] = p->texcoord0;
                    info->blend_seq[i].indexd = p->index;
                    info->blend_seq[i].weight[0] = p->weight;
                }

			}
			break;
		case FVF_XYZB3UBYTE4NORMALTEX1:
			{
				typedef lwFVFStruct_Xyzb3Ubyte4NormalTex1 _FVF_STRUCT;
				_FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
				_FVF_STRUCT* p;

				info->vertex_num = vert_num;
				info->vertex_seq = LW_NEW(lwVector3[vert_num]);
				info->normal_seq = LW_NEW(lwVector3[vert_num]);
				info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
				info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
				memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

				for(DWORD i = 0; i < info->vertex_num; i++)
				{
					p = &buf[i];
					info->vertex_seq[i] = p->pos;
					info->normal_seq[i] = p->normal;
					info->texcoord0_seq[i] = p->texcoord0;
					info->blend_seq[i].indexd = p->index;
					info->blend_seq[i].weight[0] = p->weight[0];
					info->blend_seq[i].weight[1] = p->weight[1];
					info->blend_seq[i].weight[2] = p->weight[2];
				}
			}
			break;
		case FVF_XYZB4UBYTE4NORMALTEX1:
			{
				typedef lwFVFStruct_Xyzb4Ubyte4NormalTex1 _FVF_STRUCT;
				_FVF_STRUCT* buf = (_FVF_STRUCT*)vb_data;
				_FVF_STRUCT* p;

				info->vertex_num = vert_num;
				info->vertex_seq = LW_NEW(lwVector3[vert_num]);
				info->normal_seq = LW_NEW(lwVector3[vert_num]);
				info->texcoord0_seq = LW_NEW(lwVector2[vert_num]);
				info->blend_seq = LW_NEW(lwBlendInfo[vert_num]);
				memset(info->blend_seq, 0, sizeof(lwBlendInfo) * vert_num);

				for(DWORD i = 0; i < info->vertex_num; i++)
				{
					p = &buf[i];
					info->vertex_seq[i] = p->pos;
					info->normal_seq[i] = p->normal;
					info->texcoord0_seq[i] = p->texcoord0;
					info->blend_seq[i].indexd = p->index;
					info->blend_seq[i].weight[0] = p->weight[0];
					info->blend_seq[i].weight[1] = p->weight[1];
					info->blend_seq[i].weight[2] = p->weight[2];
					info->blend_seq[i].weight[3] = p->weight[3];
				}
			}
			break;
        default:
            goto __ret;
        }
    }


    if(index_num > 0)
    {
        info->index_seq = LW_NEW(DWORD[index_num]);
        info->index_num = index_num;

        switch(ib_fvf)
        {
        case D3DFMT_INDEX16:
            {
                typedef WORD _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)ib_data;

                for(DWORD i = 0; i < index_num; i++)
                {
                    info->index_seq[i] = buf[i];
                }
            }
            break;
        case D3DFMT_INDEX32:
            {
                typedef DWORD _FVF_STRUCT;
                _FVF_STRUCT* buf = (_FVF_STRUCT*)ib_data;

                for(DWORD i = 0; i < index_num; i++)
                {
                    info->index_seq[i] = buf[i];
                }
            }
            break;
        default:
            goto __ret;
        }
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwLoadMeshDataInfo(lwMeshDataInfo* info, const lwMeshInfo* mi)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(_ConvertMeshDataVB(&info->vb_data, &info->vb_size, &info->vb_stride, mi)))
        goto __ret;

    if(mi->index_num > 0)
    {
        if(LW_FAILED(_ConvertMeshDataIB(&info->ib_data, &info->ib_size, &info->ib_stride, mi)))
            goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT _ConvertMeshDataIB(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    // D3DFMT_INDEX16
    WORD* buf = LW_NEW(WORD[info->index_num]);

    for(DWORD i = 0; i < info->index_num; i++)
    {
        buf[i] = (WORD)info->index_seq[i];
    }

    *data = buf;
    *size = sizeof(WORD) * info->index_num;
    *stride = sizeof(WORD);

    return LW_RET_OK;

}

LW_RESULT _ConvertMeshDataVB(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    //
    switch(info->fvf)
    {
    case FVF_XYZNORMAL: // bin: 10010,  Dec: 18
        _ConvertMeshData_xn(data, size, stride, info);
        break;
    case FVF_XYZRHWDIFFUSETEX1: // bin: 101000100,  Dec: 324
        _ConvertMeshData_xwdt1(data, size, stride, info);
        break;
    case FVF_XYZRHWDIFFUSETEX2: // bin: 1001000100,  Dec: 580
        _ConvertMeshData_xwdt2(data, size, stride, info);
        break;
    case FVF_XYZDIFFUSE: // bin: 1000010,  Dec: 42
        _ConvertMeshData_xd(data, size, stride, info);
        break;
    case FVF_XYZDIFFUSETEX1: // bin: 101000010,  Dec: 322
        _ConvertMeshData_xdt1(data, size, stride, info);
        break;
    case FVF_XYZDIFFUSETEX2: // bin: 1001000010,  Dec: 578
        _ConvertMeshData_xdt2(data, size, stride, info);
        break;
    case FVF_XYZNORMALTEX1: // bin: 100010010,  Dec: 274
        _ConvertMeshData_xnt1(data, size, stride, info);
        break;
    case FVF_XYZNORMALTEX2: // bin: 1000010010,  Dec: 530
        _ConvertMeshData_xnt2(data, size, stride, info);
        break;
    case FVF_XYZNORMALDIFFUSETEX1: // bin: 101010010,  Dec: 338
        _ConvertMeshData_xndt1(data, size, stride, info);
        break;
    case FVF_XYZNORMALDIFFUSETEX2: // bin: 1001010010,  Dec: 594
        _ConvertMeshData_xndt2(data, size, stride, info);
        break;
    case FVF_XYZB1DIFFUSETEX1: // bin: 101000110,  Dec: 326
        _ConvertMeshData_xb1dt1(data, size, stride, info);
        break;
    case FVF_XYZB1DIFFUSETEX2: // bin: 1001000110,  Dec: 582
        _ConvertMeshData_xb1dt2(data, size, stride, info);
        break;
    case FVF_XYZUBYTE4NORMALTEX1: // bin: 1000100010010,  Dec: 4370
        _ConvertMeshData_xunt1(data, size, stride, info);
        break;
    case FVF_XYZB2UBYTE4NORMALTEX1: // bin: 1000100011000,  Dec: 4376
        _ConvertMeshData_xb2unt1(data, size, stride, info);
        break;
    case FVF_XYZB3UBYTE4NORMALTEX1: // bin: 1000100011010,  Dec: 4378
        _ConvertMeshData_xb3unt1(data, size, stride, info);
        break;
    case FVF_XYZB4UBYTE4NORMALTEX1: // bin: 1000100011100,  Dec: 4380
        _ConvertMeshData_xb4unt1(data, size, stride, info);
        break;

    case FVF_XYZUBYTE4NORMALTEX2: // bin: 1001000010010,  Dec: 4626
        _ConvertMeshData_xunt2(data, size, stride, info);
        break;
    case FVF_XYZB2UBYTE4NORMALTEX2: // bin: 1001000011000,  Dec: 4632
        _ConvertMeshData_xb2unt2(data, size, stride, info);
        break;
    case FVF_XYZB3UBYTE4NORMALTEX2: // bin: 1001000011010,  Dec: 4634
        _ConvertMeshData_xb3unt2(data, size, stride, info);
        break;
    case FVF_XYZB4UBYTE4NORMALTEX2: // bin: 1001000011100,  Dec: 4636
        _ConvertMeshData_xb4unt2(data, size, stride, info);
        break;
    default:
		assert(0 && "_ConvertMeshDataVB undefined fvf format");
        MessageBox(0, "_ConvertMeshDataVB Unknown FVF format", "error", MB_OK);
        return LW_RET_FAILED;
    }

    return LW_RET_OK;
}

void _ConvertMeshData_xn(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzNormal _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);

}

void _ConvertMeshData_xwdt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzwDiffuseTex1 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        *(lwVector3*)&p->pos = info->vertex_seq[i];
        p->pos.w = 1.0f;
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xwdt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzwDiffuseTex2 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        *(lwVector3*)&p->pos = info->vertex_seq[i];
        p->pos.w = 1.0f;
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
        p->texcoord1 = info->texcoord1_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xd(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    lwFVFStruct_XyzDiffuse* buf = LW_NEW(lwFVFStruct_XyzDiffuse[info->vertex_num]);
    lwFVFStruct_XyzDiffuse* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->dif = info->vercol_seq[i];
    }

    *data = buf;
    *size = sizeof(lwFVFStruct_XyzDiffuse) * info->vertex_num;
    *stride = sizeof(lwFVFStruct_XyzDiffuse);
}

void _ConvertMeshData_xnt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    lwFVFStruct_XyzNormalTex1* buf = LW_NEW(lwFVFStruct_XyzNormalTex1[info->vertex_num]);
    lwFVFStruct_XyzNormalTex1* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
    }

    *data = buf;
    *size = sizeof(lwFVFStruct_XyzNormalTex1) * info->vertex_num;
    *stride = sizeof(lwFVFStruct_XyzNormalTex1);
}

void _ConvertMeshData_xnt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    lwFVFStruct_XyzNormalTex2* buf = LW_NEW(lwFVFStruct_XyzNormalTex2[info->vertex_num]);
    lwFVFStruct_XyzNormalTex2* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
		p->texcoord1 = info->texcoord1_seq[i];
    }

    *data = buf;
    *size = sizeof(lwFVFStruct_XyzNormalTex2) * info->vertex_num;
    *stride = sizeof(lwFVFStruct_XyzNormalTex2);
}

void _ConvertMeshData_xdt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    lwFVFStruct_XyzDiffuseTex1* buf = LW_NEW(lwFVFStruct_XyzDiffuseTex1[info->vertex_num]);
    lwFVFStruct_XyzDiffuseTex1* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];        
    }

    *data = buf;
    *size = sizeof(lwFVFStruct_XyzDiffuseTex1) * info->vertex_num;
    *stride = sizeof(lwFVFStruct_XyzDiffuseTex1);
}
void _ConvertMeshData_xdt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzDiffuseTex2 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
        p->texcoord1 = info->texcoord1_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xndt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzNormalDiffuseTex1 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];        
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xndt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_XyzNormalDiffuseTex2 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
		p->texcoord1 = info->texcoord1_seq[i];  
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb1dt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_Xyzb1DiffuseTex1 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->weight = info->blend_seq[i].weight[0];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);

}
void _ConvertMeshData_xb1dt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{
    typedef lwFVFStruct_Xyzb1DiffuseTex2 _FVF_STRUCT;

    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->weight = info->blend_seq[i].weight[0];
        p->dif = info->vercol_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
        p->texcoord1 = info->texcoord1_seq[i];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xunt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_XyzUbyte4NormalTex1 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];     
        p->index = info->blend_seq[i].index[0];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb2unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb2Ubyte4NormalTex1 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];     
        p->index = info->blend_seq[i].indexd;
        p->weight = info->blend_seq[i].weight[0];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb3unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb3Ubyte4NormalTex1 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];     
        p->index = info->blend_seq[i].indexd;
        p->weight[0] = info->blend_seq[i].weight[0];
		p->weight[1] = info->blend_seq[i].weight[1];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb4unt1(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb4Ubyte4NormalTex1 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];     
        p->index = info->blend_seq[i].indexd;
        p->weight[0] = info->blend_seq[i].weight[0];
        p->weight[1] = info->blend_seq[i].weight[1];
        p->weight[2] = info->blend_seq[i].weight[2];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xunt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_XyzUbyte4NormalTex2 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];
		p->texcoord1 = info->texcoord1_seq[i];
        p->index = info->blend_seq[i].index[0];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb2unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb2Ubyte4NormalTex2 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];  
        p->texcoord1 = info->texcoord1_seq[i];     
        p->index = info->blend_seq[i].indexd;
        p->weight = info->blend_seq[i].weight[0];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb3unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb3Ubyte4NormalTex2 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i]; 
        p->texcoord1 = info->texcoord1_seq[i];
        p->index = info->blend_seq[i].indexd;
        p->weight[0] = info->blend_seq[i].weight[0];
		p->weight[1] = info->blend_seq[i].weight[1];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void _ConvertMeshData_xb4unt2(void** data, DWORD* size, DWORD* stride, const lwMeshInfo* info)
{    
    typedef lwFVFStruct_Xyzb4Ubyte4NormalTex2 _FVF_STRUCT;
    _FVF_STRUCT* buf = LW_NEW(_FVF_STRUCT[info->vertex_num]);
    _FVF_STRUCT* p;

    for(DWORD i = 0; i < info->vertex_num; i++)
    {
        p = &buf[i];
        p->pos = info->vertex_seq[i];
        p->normal = info->normal_seq[i];
        p->texcoord0 = info->texcoord0_seq[i];   
        p->texcoord1 = info->texcoord1_seq[i];      
        p->index = info->blend_seq[i].indexd;
        p->weight[0] = info->blend_seq[i].weight[0];
        p->weight[1] = info->blend_seq[i].weight[1];
        p->weight[2] = info->blend_seq[i].weight[2];
    }

    *data = buf;
    *size = sizeof(_FVF_STRUCT) * info->vertex_num;
    *stride = sizeof(_FVF_STRUCT);
}

void lwScreenToWorld(lwVector3* org, lwVector3* ray, int x, int y, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view)
{
	lwVector3 v;
	lwMatrix44 mat_inv;


	v.x = ((2.0f * x / width) - 1.0f) / mat_proj->_11;
	v.y = -((2.0f * y / height) - 1.0f) / mat_proj->_22;
	v.z = 1.0f;

	lwMatrix44Inverse(&mat_inv, NULL, mat_view);
    //lwMatrix44InverseNoScaleFactor(&mat_inv, mat_view);

	ray->x = v.x * mat_inv._11 + v.y * mat_inv._21 + v.z * mat_inv._31;
	ray->y = v.x * mat_inv._12 + v.y * mat_inv._22 + v.z * mat_inv._32;
	ray->z = v.x * mat_inv._13 + v.y * mat_inv._23 + v.z * mat_inv._33;
    lwVector3Normalize(ray);

	org->x = mat_inv._41;
	org->y = mat_inv._42;
	org->z = mat_inv._43;
}

void lwWorldToScreen(int* x, int* y, float* z, const lwVector3* vec, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view)
{
	lwVector3 v(*vec);

	lwVec3Mat44Mul(&v, mat_view);
	lwVec3Mat44Mul(&v, mat_proj);

	v.x /= v.z;
	v.y /= v.z;

	*x = (int)((v.x + 1.0f) / 2 * width);
	*y = (int)((1.0f - v.y) / 2 * height);

	if(z) {
		float zn = -mat_proj->_33 / mat_proj->_43;
		float zf = mat_proj->_43 / (1.0f - mat_proj->_33);

		*z = (v.z - zn) / (zf - zn);
	}
}

void lwGetBoxVertLineList(lwVector3* vert_seq, const lwBox* box)
{
    lwVector3 v0 = box->c - box->r;
    lwVector3 v1 = box->c + box->r;

#define v vert_seq

    // bottom
    v[0].x = v0.x; v[0].y = v0.y; v[0].z = v0.z;
    v[1].x = v0.x; v[1].y = v1.y; v[1].z = v0.z;

    v[2] = v[1];
    v[3].x = v1.x; v[3].y = v1.y; v[3].z = v0.z;

    v[4] = v[3];
    v[5].x = v1.x; v[5].y = v0.y; v[5].z = v0.z;

    v[6] = v[5];
    v[7] = v[0];

    // up
    v[8].x = v0.x; v[8].y = v0.y; v[8].z = v1.z;
    v[9].x = v0.x; v[9].y = v1.y; v[9].z = v1.z;

    v[10] = v[9];
    v[11].x = v1.x; v[11].y = v1.y; v[11].z = v1.z;

    v[12] = v[11];
    v[13].x = v1.x; v[13].y = v0.y; v[13].z = v1.z;

    v[14] = v[13];
    v[15] = v[8];

    // side
    v[16] = v[0];
    v[17] = v[8];

    v[18] = v[1];
    v[19] = v[9];

    v[20] = v[3];
    v[21] = v[11];

    v[22] = v[5];
    v[23] = v[13];

#undef v

}

// left-hand coordinates
//   y
//   ^
//   |    ^ z
//   |   /
//  1|  /     2 // up is in the same order
//   | /
//   |/
// 0  ------3----------> x
//
//
void lwGetBoxTriangleList(lwVector3* vert_seq, DWORD* index_seq, const lwBox* box)
{
    lwVector3 v0 = box->c - box->r;
    lwVector3 v1 = box->c + box->r;

#define v vert_seq

    //// bottom
    //v[0].x = v0.x; v[0].y = v0.y; v[0].z = v0.z;
    //v[1].x = v0.x; v[1].y = v1.y; v[1].z = v0.z;
    //v[2].x = v1.x; v[2].y = v1.y; v[2].z = v0.z;
    //v[3].x = v1.x; v[3].y = v0.y; v[3].z = v0.z;
    //// up
    //v[4].x = v0.x; v[4].y = v0.y; v[4].z = v1.z;
    //v[5].x = v0.x; v[5].y = v1.y; v[5].z = v1.z;
    //v[6].x = v1.x; v[6].y = v1.y; v[6].z = v1.z;
    //v[7].x = v1.x; v[7].y = v0.y; v[7].z = v1.z;

    // bottom
    v[0].x = v0.x; v[0].y = v0.y; v[0].z = v0.z;
    v[1].x = v0.x; v[1].y = v0.y; v[1].z = v1.z;
    v[2].x = v1.x; v[2].y = v0.y; v[2].z = v1.z;
    v[3].x = v1.x; v[3].y = v0.y; v[3].z = v0.z;
    // up
    v[4].x = v0.x; v[4].y = v1.y; v[4].z = v0.z;
    v[5].x = v0.x; v[5].y = v1.y; v[5].z = v1.z;
    v[6].x = v1.x; v[6].y = v1.y; v[6].z = v1.z;
    v[7].x = v1.x; v[7].y = v1.y; v[7].z = v0.z;

#undef v

#define i index_seq

    //// bottom
    //i[0] = 0; i[1] = 1; i[2] = 2;
    //i[3] = 2; i[4] = 3; i[5] = 0;
    //// up
    //i[6] = 4; i[7] = 5; i[8] = 6;
    //i[9] = 6; i[10] = 7; i[11] = 4;
    //// front
    //i[12] = 0; i[13] = 4; i[14] = 7;
    //i[15] = 7; i[16] = 3; i[17] = 0;
    //// back
    //i[18] = 2; i[19] = 6; i[20] = 5;
    //i[21] = 5; i[22] = 1; i[23] = 2;
    //// left
    //i[24] = 0; i[25] = 1; i[26] = 5;
    //i[27] = 5; i[28] = 4; i[29] = 0;
    //// right
    //i[30] = 3; i[31] = 7; i[32] = 6;
    //i[33] = 6; i[34] = 2; i[35] = 3;

        // bottom
    i[0] = 0; i[1] = 2; i[2] = 1;
    i[3] = 2; i[4] = 0; i[5] = 3;
    // up
    i[6] = 4; i[7] = 5; i[8] = 6;
    i[9] = 6; i[10] = 7; i[11] = 4;
    // front
    i[12] = 0; i[13] = 4; i[14] = 7;
    i[15] = 7; i[16] = 3; i[17] = 0;
    // back
    i[18] = 1; i[19] = 2; i[20] = 6;
    i[21] = 6; i[22] = 5; i[23] = 1;
    // left
    i[24] = 1; i[25] = 5; i[26] = 4;
    i[27] = 4; i[28] = 0; i[29] = 1;
    // right
    i[30] = 3; i[31] = 7; i[32] = 6;
    i[33] = 2; i[34] = 3; i[35] = 6;

#undef i

}
void lwGetBoxTriangleList(lwVector3* vert36_seq, lwVector3* normal36_seq, const lwVector3* size)
{
    float x = size->x / 2;
    float y = size->y / 2;
    float z = size->z / 2;

    lwVector3 vvv_seq[] =
    {
        // bottom
        lwVector3(-x, -y, -z), // 0
        lwVector3(-x, -y,  z), // 1
        lwVector3( x, -y,  z), // 2
        lwVector3( x, -y, -z), // 3
        // up
        lwVector3(-x,  y, -z), // 4
        lwVector3(-x,  y,  z), // 5
        lwVector3( x,  y,  z), // 6
        lwVector3( x,  y, -z), // 7
    };

    lwVector3 www_seq[] =
    {
        lwVector3(0.0f,-1.0f, 0.0f), // bottom
        lwVector3(0.0f, 1.0f, 0.0f), // up
        lwVector3(0.0f, 0.0f,-1.0f), // front
        lwVector3(0.0f, 0.0f, 1.0f), // back
        lwVector3(-1.0f,0.0f, 0.0f), // left
        lwVector3( 1.0f,0.0f, 0.0f), // right
    };

    lwVector3* p = vert36_seq;
    lwVector3* q = normal36_seq;
    int n = 0;
    // bottom
    p[n] = vvv_seq[2];    q[n++] = www_seq[0];
    p[n] = vvv_seq[1];    q[n++] = www_seq[0];
    p[n] = vvv_seq[0];    q[n++] = www_seq[0];

    p[n] = vvv_seq[0];    q[n++] = www_seq[0];
    p[n] = vvv_seq[3];    q[n++] = www_seq[0];
    p[n] = vvv_seq[2];    q[n++] = www_seq[0];

    // up
    p[n] = vvv_seq[4];    q[n++] = www_seq[1];
    p[n] = vvv_seq[5];    q[n++] = www_seq[1];
    p[n] = vvv_seq[6];    q[n++] = www_seq[1];
                      
    p[n] = vvv_seq[6];    q[n++] = www_seq[1];
    p[n] = vvv_seq[7];    q[n++] = www_seq[1];
    p[n] = vvv_seq[4];    q[n++] = www_seq[1];

    // front
    p[n] = vvv_seq[0];    q[n++] = www_seq[2];
    p[n] = vvv_seq[4];    q[n++] = www_seq[2];
    p[n] = vvv_seq[7];    q[n++] = www_seq[2];
                      
    p[n] = vvv_seq[7];    q[n++] = www_seq[2];
    p[n] = vvv_seq[3];    q[n++] = www_seq[2];
    p[n] = vvv_seq[0];    q[n++] = www_seq[2];

    // back
    p[n] = vvv_seq[1];    q[n++] = www_seq[3];
    p[n] = vvv_seq[2];    q[n++] = www_seq[3];
    p[n] = vvv_seq[6];    q[n++] = www_seq[3];
                      
    p[n] = vvv_seq[6];    q[n++] = www_seq[3];
    p[n] = vvv_seq[5];    q[n++] = www_seq[3];
    p[n] = vvv_seq[1];    q[n++] = www_seq[3];

    // left
    p[n] = vvv_seq[0];    q[n++] = www_seq[4];
    p[n] = vvv_seq[1];    q[n++] = www_seq[4];
    p[n] = vvv_seq[4];    q[n++] = www_seq[4];
                      
    p[n] = vvv_seq[5];    q[n++] = www_seq[4];
    p[n] = vvv_seq[4];    q[n++] = www_seq[4];
    p[n] = vvv_seq[1];    q[n++] = www_seq[4];

    // right
    p[n] = vvv_seq[3];    q[n++] = www_seq[5];
    p[n] = vvv_seq[7];    q[n++] = www_seq[5];
    p[n] = vvv_seq[6];    q[n++] = www_seq[5];
                      
    p[n] = vvv_seq[2];    q[n++] = www_seq[5];
    p[n] = vvv_seq[3];    q[n++] = www_seq[5];
    p[n] = vvv_seq[6];    q[n++] = www_seq[5];


}


void lwBuildVertexNormalWithTriangleList(lwVector3* normal_seq, const lwVector3* vert_seq, const DWORD* index_seq, DWORD vert_num, DWORD index_num)
{
    // set vertices normal
	DWORD* ver_ref = LW_NEW(DWORD[vert_num]);
    lwVector3 ver_n;

	memset(ver_ref, 0, sizeof(DWORD) * vert_num);
	memset(normal_seq, 0, sizeof(lwVector3) * vert_num);

    DWORD j = index_num / 3;
    const DWORD* ptr_ind;

    for(DWORD i = 0; i < j; i++) 
    {
        ptr_ind = &index_seq[3 * i];

		// reference count
		ver_ref[ptr_ind[0]] ++;
		ver_ref[ptr_ind[1]] ++;
		ver_ref[ptr_ind[2]] ++;

        lwGetTriangleNormal(&ver_n, &vert_seq[ptr_ind[0]], &vert_seq[ptr_ind[1]], &vert_seq[ptr_ind[2]]);

		// normal value
		normal_seq[ptr_ind[0]] += ver_n;
		normal_seq[ptr_ind[1]] += ver_n;
		normal_seq[ptr_ind[2]] += ver_n;
	}

	// normalize
    for(int i = 0; i < vert_num; i++) 
    {
		// normalize vector normal
        float tmp_f = 1.0f / ver_ref[i];
		normal_seq[i].x *= tmp_f;
		normal_seq[i].y *= tmp_f;
		normal_seq[i].z *= tmp_f;

		lwVector3Normalize(&normal_seq[i]);
	}

	LW_DELETE_A(ver_ref);

}

LW_RESULT lwHitTestBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwBox* box, const lwMatrix44* mat)
{

    lwPickInfo u, v;
    v.data = LW_INVALID_INDEX;

    lwVector3 v_buf[8];
    DWORD i_buf[36];

    lwGetBoxTriangleList(v_buf, i_buf, box);

    for(int i = 0; i < 8; i++)
    {
        lwVec3Mat44Mul(&v_buf[i], mat);
    }

    for(int i = 0; i < 12; i++)
    {
        lwTriangle tri(v_buf[i_buf[i * 3]], v_buf[i_buf[i * 3 + 1]], v_buf[i_buf[i * 3 + 2]]);

        if(lwIntersectRayTriangle(&u.pos, &u.dis, org, ray, &tri))
        {
            if(v.data == LW_INVALID_INDEX || v.dis > u.dis)
            {                
                v = u;
                v.data = 0;
            }
        }
    }

    if(v.data == LW_INVALID_INDEX)
    {
        return LW_RET_FAILED;
    }

    *info = v;

    return LW_RET_OK;

}

LW_RESULT lwSetRenderState(lwRenderStateValue* rs_seq, DWORD rs_num, DWORD state, DWORD value)
{
    DWORD i;
    lwRenderStateValue* rsv;
    
    for(i = 0; i < rs_num; i++)
    {
        rsv = &rs_seq[i];

        if(rsv->state == state)
        {
            rsv->value = value;
            goto __ret;
        }
    }

    for(i = 0; i < rs_num; i++)
    {
        rsv = &rs_seq[i];

        if(rsv->state == LW_INVALID_INDEX)
        {
            rsv->state = state;
            rsv->value = value;
            goto __ret;
        }
    }

__ret:
    return i == rs_num ? LW_RET_FAILED : LW_RET_OK;

}
LW_RESULT lwClearRenderState(lwRenderStateValue* rs_seq, DWORD rs_num, DWORD state)
{
    DWORD i;
    lwRenderStateValue* rsv;

    for(i = 0; i < rs_num; i++)
    {
        rsv = &rs_seq[i];

        if(rsv->state == state)
        {
            for(DWORD j = i; j < rs_num - 1; j++)
            {
                if(rs_seq[j + 1].state == LW_INVALID_INDEX)
                    goto __ret;

                rs_seq[j] = rs_seq[j + 1];
            }

            goto __ret;
        }
    }

__ret:

    return i == rs_num ? LW_RET_FAILED : LW_RET_OK;

}

LW_RESULT lwGetDirectXVersion(char* o_buf, DWORD version)
{
    LW_RESULT ret = LW_RET_FAILED;

#define CASE_ID2STR(id) \
    case id: \
    _tcscpy(o_buf, #id); \
    break;

    switch(version)
    {
        CASE_ID2STR(DX_VERSION_8_0);
        CASE_ID2STR(DX_VERSION_8_1_a);
        CASE_ID2STR(DX_VERSION_8_1_b);
        CASE_ID2STR(DX_VERSION_8_2);
        CASE_ID2STR(DX_VERSION_9_0);
        CASE_ID2STR(DX_VERSION_9_0_a);
        CASE_ID2STR(DX_VERSION_9_0_b);
        CASE_ID2STR(DX_VERSION_9_0_c);
        CASE_ID2STR(DX_VERSION_UNKNOWN);
        CASE_ID2STR(DX_VERSION_X_X);
    default:
        goto __ret;
    }

#undef CASE_ID2STR

    ret = LW_RET_OK;
__ret:
    return ret;
}

//-----------------------------------------------------------------------------
// copied from d3dutil.cpp
//-----------------------------------------------------------------------------
void lwGetCubeMapViewMatrix(lwMatrix44* mat, DWORD face)
{
    D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    switch( face )
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
    }

    // Set the view transform for this cubemap surface
    D3DXMatrixLookAtLH((D3DXMATRIX*)mat, &vEyePt, &vLookDir, &vUpDir );
}

LW_END
