#include "stdafx.h"
#include "lwShaderMgr.h"
#include "lwInterface.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwResourceMgr.h"
#include "lwShaderDeclMgr.h"

LW_BEGIN


//

// =================================
#if(defined LW_USE_DX9)

LW_STD_IMPLEMENTATION(lwShaderMgr9)

lwShaderMgr9::lwShaderMgr9(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _vs_seq(0), _vs_size(0), _vs_num(0),
  _decl_seq(0), _decl_size(0), _decl_num(0), _decl_mgr(0)
{
}

lwShaderMgr9::~lwShaderMgr9()
{
    for(DWORD i = 0; _vs_num > 0; i++)
    {
        if(_vs_seq[i].handle)
        {
            LW_DELETE_A(_vs_seq[i].data);
            LW_RELEASE(_vs_seq[i].handle);
            _vs_num -= 1;
        }
    }

    for(DWORD i = 0; _decl_num > 0; i++)
    {
        if(_decl_seq[i].handle)
        {
            LW_DELETE_A(_decl_seq[i].data);
            LW_RELEASE(_decl_seq[i].handle);
            _decl_num -= 1;
        }
    }

    LW_IF_RELEASE(_decl_mgr);
}

LW_RESULT lwShaderMgr9::Init(DWORD vs_buf_size, DWORD decl_buf_size, DWORD ps_buf_size)
{
    _vs_num = 0;
    _vs_size = vs_buf_size;
    _vs_seq = LW_NEW(lwVertexShaderInfo[_vs_size]); 
    memset(_vs_seq, 0, sizeof(lwVertexShaderInfo) * _vs_size);

    _decl_num = 0;
    _decl_size = decl_buf_size;
    _decl_seq = LW_NEW(lwVertDeclInfo9[_decl_size]);
    memset(_decl_seq, 0, sizeof(lwVertDeclInfo9) * _decl_size);

    _decl_mgr = LW_NEW(lwShaderDeclMgr(this));

    return LW_RET_OK;
}

LW_RESULT lwShaderMgr9::RegisterVertexShader(DWORD type, BYTE* data, DWORD size)
{
    LW_RESULT ret = LW_RET_FAILED;
    IDirect3DDeviceX* dev = _dev_obj->GetDevice();
    IDirect3DVertexShaderX* handle = 0;
    lwVertexShaderInfo* i = 0;            // << declarado antes de qualquer goto

    if (type >= _vs_size)                 // DWORD  unsigned; "type < 0" nunca  verdadeiro
        goto __ret;

    if (_vs_seq[type].handle)
        goto __ret;

    if (!data || size == 0)
        goto __ret;

    if (FAILED(dev->CreateVertexShader((DWORD*)data, &handle)))
        goto __ret;

    i = &_vs_seq[type];
    i->handle = handle;
    i->size   = size;
    i->data   = LW_NEW(BYTE[size]);
    memcpy(i->data, data, size);

    _vs_num += 1;
    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwShaderMgr9::RegisterVertexShader(DWORD type, const char* file, DWORD file_flag, const D3DXMACRO* defines)
{
    LW_RESULT ret = LW_RET_FAILED;

    long size = 0;
    BYTE* data = 0;
    BYTE* code = 0;

    ID3DXBuffer* buf_code = 0;
    ID3DXBuffer* buf_error = 0;

    FILE* fp = fopen(file, "rb");
    if(fp == NULL)
        goto __ret;

	fseek(fp, 0, SEEK_END);
	
	size = ftell(fp);
    data = LW_NEW(BYTE[size]);

	fseek(fp, 0, SEEK_SET);

    fread(data, size, 1, fp);

    fclose(fp);
   
    if(file_flag == VS_FILE_OBJECT)
    {
        code = data;
    }
    else
    {

#if(defined LW_SHADER_DEBUG_VS)
        DWORD compile_flag = 0;
        compile_flag |= D3DXSHADER_DEBUG;
        //compile_flag |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;

        if(file_flag == VS_FILE_ASM)
        {
            if(FAILED(D3DXAssembleShaderFromFile(
                file,
                NULL, 
                NULL, 
                compile_flag, 
                &buf_code, 
                &buf_error)))
            {
                goto __ret;
            }
        }
        else if(file_flag == VS_FILE_HLSL)
        {

            if(FAILED(D3DXCompileShaderFromFile(
                file,
                NULL,
                NULL,
                "main",
                "vs_3_0",
                compile_flag,
                &buf_code,
                &buf_error,
                NULL)))
            {
                goto __ret;
            }

        }

#else
        if(file_flag == VS_FILE_ASM)
        {
            if (FAILED(D3DXAssembleShader((LPCSTR)data, size, NULL, NULL, 0, &buf_code, &buf_error)))
                goto __ret;
        }
        else if(file_flag == VS_FILE_HLSL)
        {
            DWORD compile_flag = 0;
            // defines — указатель на массив D3DXMACRO, терминированный
            // {NULL, NULL}. Может быть nullptr (без макросов). Пропускаем
            // напрямую в D3DXCompileShader — он корректно обрабатывает оба случая.
            const D3DXMACRO* macro_ptr = nullptr;
            if (defines && defines->Name) {
                macro_ptr = defines;
            }
            if(FAILED(D3DXCompileShader(
                (LPCSTR)data,
                size,
                macro_ptr,
                NULL,
                "main",
                "vs_3_0",
                compile_flag,
                &buf_code,
                &buf_error,
                NULL)))
            {
                goto __ret;
            }
        }
#endif

        code = (BYTE*)buf_code->GetBufferPointer();
        size = buf_code->GetBufferSize();

    }

    if(LW_FAILED(RegisterVertexShader(type, code, size)))
        goto __ret;

    ret = LW_RET_OK;

__ret:
    LW_SAFE_DELETE_A(data);
    LW_SAFE_RELEASE(buf_code);
    LW_SAFE_RELEASE(buf_error);
    return ret;

}
LW_RESULT lwShaderMgr9::RegisterVertexDeclaration(DWORD type, D3DVERTEXELEMENT9* data)
{
    LW_RESULT ret = LW_RET_FAILED;

    // Declaraes antecipadas (evita "init skipped by goto")
    IDirect3DVertexDeclarationX* handle = 0;
    IDirect3DDeviceX* dev = _dev_obj->GetDevice();
    int i = 0;
    D3DVERTEXELEMENT9* p = 0;

    // (Com DWORD, "type < 0" nunca  verdadeiro; pode remover se quiser)
    if (type >= _decl_size)
        goto __ret;

    if (_decl_seq[type].handle)
        goto __ret;

    if (!data)  // segurana
        goto __ret;

    if (FAILED(dev->CreateVertexDeclaration(data, &handle)))
        goto __ret;

    _decl_seq[type].handle = handle;

    // Agora sim inicializa p e usa
    p = data;
    while (p->Stream != 0xFF)
    {
        ++i;
        ++p;
    }
    ++i;

    _decl_seq[type].data = LW_NEW(D3DVERTEXELEMENT9[i]);
    memcpy(_decl_seq[type].data, data, sizeof(D3DVERTEXELEMENT9) * i);

    _decl_num += 1;
    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwShaderMgr9::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    IDirect3DDeviceX* dev = _dev_obj->GetDevice();

    lwVertexShaderInfo* s;

    for(DWORD i = 0; i < _vs_size; i++)
    {
        s = &_vs_seq[i];

        LW_SAFE_RELEASE(s->handle);
    }

    ret = LW_RET_OK;

//__ret:
    return ret;
}
LW_RESULT lwShaderMgr9::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    IDirect3DDeviceX* dev = _dev_obj->GetDevice();

    lwVertexShaderInfo* s;

    for(DWORD i = 0; i < _vs_size; i++)
    {
        s = &_vs_seq[i];

        if(s->handle == 0 && s->data)
        {
            if(FAILED(dev->CreateVertexShader((DWORD*)s->data, &s->handle)))
                goto __ret;
        }
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwShaderMgr9::QueryVertexShader(IDirect3DVertexShaderX** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(type < 0 || type >= _vs_size)
        goto __ret;

    if(_vs_seq[type].handle == 0)
        goto __ret;

    *ret_obj = _vs_seq[type].handle;

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT lwShaderMgr9::QueryVertexDeclaration(IDirect3DVertexDeclarationX** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(type < 0 || type >= _decl_size)
        goto __ret;

    if(_decl_seq[type].handle == 0)
        goto __ret;

    *ret_obj = _decl_seq[type].handle;

    ret = LW_RET_OK;

__ret:
    return ret;

}

#endif


LW_END
