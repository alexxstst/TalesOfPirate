//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwShaderTypes.h"
#include "lwInterfaceExt.h"

LW_BEGIN

// ===============================================
// directX8 shader manager

// ===============================================
// directX9 shader manager

class lwShaderMgr9 : public lwIShaderMgr
{
LW_STD_DECLARATION()

private:
    lwIDeviceObject* _dev_obj;
    lwVertexShaderInfo* _vs_seq;
    lwVertDeclInfo9* _decl_seq;
    DWORD _vs_size;
    DWORD _vs_num;
    DWORD _decl_size;
    DWORD _decl_num;

    lwIShaderDeclMgr* _decl_mgr;

public:
    lwShaderMgr9(lwIDeviceObject* dev_obj);
    ~lwShaderMgr9();


    LW_RESULT Init(DWORD vs_buf_size, DWORD decl_buf_size, DWORD ps_buf_size);
    LW_RESULT RegisterVertexShader(DWORD type, BYTE* data, DWORD size);
    LW_RESULT RegisterVertexShader(DWORD type, const char* file, DWORD binary_flag, const D3DXMACRO* defines = NULL);
    LW_RESULT RegisterVertexDeclaration(DWORD type, D3DVERTEXELEMENT9* data);
    LW_RESULT LoseDevice();
    LW_RESULT ResetDevice();

    LW_RESULT QueryVertexShader(IDirect3DVertexShaderX** ret_obj, DWORD type);
    LW_RESULT QueryVertexDeclaration(IDirect3DVertexDeclarationX** ret_obj, DWORD type);

    lwVertexShaderInfo* GetVertexShaderInfo(DWORD type) { return &_vs_seq[type]; }
    
    lwIShaderDeclMgr* GetShaderDeclMgr() { return _decl_mgr; }
};

typedef lwShaderMgr9 lwShaderMgr;


LW_END
