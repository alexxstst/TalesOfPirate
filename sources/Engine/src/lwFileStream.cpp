//
#include "stdafx.h"
#include "lwFileStream.h"
#include "lwFile.h"

LW_BEGIN

//lwFileStream
LW_STD_IMPLEMENTATION(lwFileStream)

lwFileStream::lwFileStream()
{
    _adapter_file = 0;
}

lwFileStream::~lwFileStream()
{
    Close();
}

LW_RESULT lwFileStream::Open(const char* file, const lwFileStreamOpenInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(Close()))
        goto __ret;

    if(info->adapter_type == FS_ADAPTER_FILE)
    {
        _adapter_file = LW_NEW(lwFile);
        if(!_adapter_file) {
            goto __ret;
        }

        if(LW_FAILED(_adapter_file->CreateFile(file, info->access_flag, NULL, 0, info->create_flag, info->attributes_flag)))
            goto __ret;
    }


    _fsoi = *info;

    ret = LW_RET_OK;

__ret:
    if(LW_FAILED(ret))
    {
        Close();
    }
    return ret;
}

LW_RESULT lwFileStream::Close()
{
    LW_SAFE_RELEASE(_adapter_file);
    memset(&_fsoi, 0, sizeof(_fsoi));

    return LW_RET_OK;
}

LW_RESULT lwFileStream::Read(void* buf, DWORD in_size, DWORD* out_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(LW_FAILED(_adapter_file->Read(buf, in_size, out_size)))
            goto __ret;
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFileStream::Write(const void* buf, DWORD in_size, DWORD* out_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(LW_FAILED(_adapter_file->Write(buf, in_size, out_size)))
            goto __ret;
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFileStream::Seek(DWORD* pos, long offset, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(pos)
        {
            *pos = _adapter_file->Seek(offset, flag);
        }
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFileStream::GetSize(DWORD* size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(size)
        {
            *size = _adapter_file->GetSize();
        }
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwFileStream::Flush()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(LW_FAILED(_adapter_file->Flush()))
            goto __ret;
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFileStream::SetEnd()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_fsoi.adapter_type == FS_ADAPTER_FILE)
    {
        if(_adapter_file == 0)
            goto __ret;

        if(LW_FAILED(_adapter_file->SetEnd()))
            goto __ret;
    }
    else if(_fsoi.adapter_type == FS_ADAPTER_PACKET)
    {
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_END
