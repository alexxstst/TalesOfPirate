//
#include "stdafx.h"
#include "lwMisc.h"

LW_BEGIN

// lwBuffer
LW_STD_IMPLEMENTATION(lwBuffer)

lwBuffer::lwBuffer()
: _data(0), _size(0)
{
}

lwBuffer::~lwBuffer()
{
    Free();
}

LW_RESULT lwBuffer::Alloc(DWORD size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_size > 0)
        goto __ret;

    _data = (BYTE*)LW_MALLOC(size);
    if(_data == 0)
        goto __ret;

    _size = size;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwBuffer::Realloc(DWORD size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_data == 0)
        goto __ret;

    _data = (BYTE*)LW_REALLOC(_data, size);
    if(_data == 0)
        goto __ret;

    _size = size;
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwBuffer::Free()
{
    if(_size == 0)
        return LW_RET_FAILED;

    LW_DELETE_A(_data);
    _data = 0;
    _size = 0;

    return LW_RET_OK;
}

LW_RESULT lwBuffer::SetSizeArbitrary(DWORD size)
{
    _size = size;
    return LW_RET_OK;
}
// lwByteSet
LW_STD_IMPLEMENTATION(lwByteSet)
LW_RESULT lwByteSet::SetValueSeq(DWORD start, BYTE* buf, DWORD num)
{
    if((start + num) >= _size)
        return LW_RET_FAILED;

    memcpy(&_buf[start], buf, sizeof(BYTE) * num);

    return LW_RET_OK;
}

int lwHexStrToInt(const char* str)
{
	for( ; (*str == ' ') || (*str == '\t'); ++str )
		;

	if( *(str++) != '0' )
		return 0;

	if( *str != 'x' && *str != 'X' )
		return 0;

	str++;

	int v = 0;

	for( ; *str>='0' && *str<='9'; ++str ) {
		v = v * 16 + (*str - '0');
	}

	return v;

}

LW_RESULT lwRenderStateAtomBeginSetRS(lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num)
{
    lwRenderStateAtom* p;
    for(DWORD i = 0; i < num; i++)
    {
        p = &rsa_seq[i];

        if(p->state == LW_INVALID_INDEX)
            break;

        if (p->state >= D3DRS_ZENABLE)
            dev_obj->GetRenderState(p->state, &p->value1);
        else
            dev_obj->GetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)p->state, &p->value1);
        if(p->value0 != p->value1)
        {
            if(p->state >= D3DRS_ZENABLE)
                dev_obj->SetRenderState((D3DRENDERSTATETYPE)p->state, p->value0);
            else
                dev_obj->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)p->state, p->value0);
        }
    }

    return LW_RET_OK;
}
LW_RESULT lwRenderStateAtomEndSetRS(lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num)
{
    lwRenderStateAtom* p;
    for(DWORD i = 0; i < num; i++)
    {
        p = &rsa_seq[i];

        if(p->state == LW_INVALID_INDEX)
            break;

        if(p->value0 != p->value1)
        {
            if (p->state >= D3DRS_ZENABLE)
                dev_obj->SetRenderState((D3DRENDERSTATETYPE)p->state, p->value1);
            else
                dev_obj->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)p->state, p->value1);
            p->value1 = p->value0;
        }
    }

    return LW_RET_OK;
}

LW_RESULT lwRenderStateAtomBeginSetTSS(DWORD stage, lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num)
{
    lwRenderStateAtom* p;
    for(DWORD i = 0; i < num; i++)
    {
        p = &rsa_seq[i];

        if(p->state == LW_INVALID_INDEX)
            break;

        dev_obj->GetSamplerState(stage, (D3DSAMPLERSTATETYPE)p->state, &p->value1);
        if(p->value0 != p->value1)
        {
            dev_obj->SetSamplerState(stage, (D3DSAMPLERSTATETYPE)p->state, p->value0);
        }
    }

    return LW_RET_OK;
}
LW_RESULT lwRenderStateAtomEndSetTSS(DWORD stage, lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num)
{
    lwRenderStateAtom* p;
    for(DWORD i = 0; i < num; i++)
    {
        p = &rsa_seq[i];

        if(p->state == LW_INVALID_INDEX)
            break;

        if(p->value0 != p->value1)
        {
            dev_obj->SetSamplerState(stage, (D3DSAMPLERSTATETYPE)p->state, p->value1);
            p->value1 = p->value0;
        }
    }

    return LW_RET_OK;
}

// lwRenderStateAtomSet
LW_STD_IMPLEMENTATION(lwRenderStateAtomSet)

lwRenderStateAtomSet::lwRenderStateAtomSet()
: _rsa_seq(0), _rsa_num(0)
{
}
lwRenderStateAtomSet::~lwRenderStateAtomSet()
{
    Clear();
}
LW_RESULT lwRenderStateAtomSet::Clear()
{
    if(_rsa_num)
    {
        LW_DELETE_A(_rsa_seq);
        _rsa_seq = 0;
        _rsa_num = 0;
    }
    return LW_RET_OK;
}
LW_RESULT lwRenderStateAtomSet::Clone(lwIRenderStateAtomSet** obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwRenderStateAtomSet* o = LW_NEW(lwRenderStateAtomSet);
    
    if(LW_FAILED(o->Load(_rsa_seq, _rsa_num)))
        goto __ret;

    *obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwRenderStateAtomSet::Allocate(DWORD size)
{
    Clear();

    if(size > 0)
    {
        _rsa_seq = LW_NEW(lwRenderStateAtom[size]);
        _rsa_num = size;

        for(DWORD i = 0; i < _rsa_num; i++)
        {
            lwRenderStateAtom_Construct(&_rsa_seq[i]);
        }
    }

    return LW_RET_OK;
}

LW_RESULT lwRenderStateAtomSet::Load(const lwRenderStateAtom* rsa_seq, DWORD rsa_num)
{
    if(_rsa_num != rsa_num)
    {
        Allocate(rsa_num);
    }

    memcpy(_rsa_seq, rsa_seq, sizeof(lwRenderStateAtom) * rsa_num);

    return LW_RET_OK;
}

LW_RESULT lwRenderStateAtomSet::FindState(DWORD* id, DWORD state)
{
    for(DWORD i = 0; i < _rsa_num; i++)
    {
        if(_rsa_seq[i].state == LW_INVALID_INDEX)
        {
            return LW_RET_FAILED;
        }

        if(_rsa_seq[i].state == state)
        {
            if(id)
            {
                *id = i;
            }
            return LW_RET_OK;
        }
    }

    return LW_RET_FAILED;
}
LW_RESULT lwRenderStateAtomSet::ResetStateValue(DWORD state, DWORD value, DWORD* old_value)
{
    lwRenderStateAtom* a;

    for(DWORD i = 0; i < _rsa_num; i++)
    {
        a = &_rsa_seq[i]; 

        if(a->state == LW_INVALID_INDEX)
        {
            return LW_RET_FAILED;
        }

        if(a->state == state)
        {
            if(old_value)
            {
                *old_value = value;
            }
            a->value0 = value;
            a->value1 = value;

            return LW_RET_OK;
        }
    }

    return LW_RET_FAILED;
}
LW_RESULT lwRenderStateAtomSet::GetStateAtom(lwRenderStateAtom** rsa, DWORD id) 
{ 
    if(id >= _rsa_num || rsa == 0)
        return LW_RET_FAILED;

    *rsa = &_rsa_seq[id];
    return LW_RET_OK;
}



LW_RESULT lwIAnimCtrlObj_PlayPose(lwIAnimCtrlObj* ctrl_obj, const lwPlayPoseInfo* info)
{
    return ctrl_obj->PlayPose(info);
}


lwIPoseCtrl* lwIAnimCtrlObj_GetPoseCtrl(lwIAnimCtrlObj* ctrl_obj)
{
    lwIPoseCtrl* ret = 0;
    lwIAnimCtrl* ctrl = ctrl_obj->GetAnimCtrl();
    if(ctrl == 0)
        goto __ret;

    ret = ctrl->GetPoseCtrl();
__ret:
    return ret;
}
lwPlayPoseInfo* lwIAnimCtrlObj_GetPlayPoseInfo(lwIAnimCtrlObj* ctrl_obj)
{
    return ctrl_obj->GetPlayPoseInfo();
}

DWORD lwGetBlendWeightNum(DWORD fvf)
{
    DWORD blend_num = 0;

    if(fvf & (D3DFVF_XYZ | D3DFVF_LASTBETA_UBYTE4))
    {
        blend_num = 1;
    }
    else if(fvf & (D3DFVF_XYZB2 | D3DFVF_LASTBETA_UBYTE4))
    {
        blend_num = 2;
    }
    else if(fvf & (D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4))
    {
        blend_num = 3;
    }
    else if(fvf & (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4))
    {
        blend_num = 4;
    }

    return blend_num;
}

LW_RESULT LoadFileInMemory(lwIBuffer* buf, const char* file, const char* load_flag)
{
    LW_RESULT ret = LW_RET_FAILED;
    BYTE* data = 0;
    DWORD size = 0;
    FILE* fp = nullptr;

    if(buf == 0)
        goto __ret;

    {
        fp = fopen(file, load_flag);
        if (fp == 0)
            goto __ret;

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buf->Free();
        buf->Alloc(size);

        fread(buf->GetData(), size, 1, fp);


        ret = LW_RET_OK;
    }
__ret:
    if(fp)
    {
	    fclose(fp);
    }

    return ret;
}

LW_RESULT LoadFileInMemory(BYTE** data_seq, DWORD* data_size, const char* file, const char* load_flag)
{
    LW_RESULT ret = LW_RET_FAILED;
    BYTE* data = 0;
    DWORD size = 0;

	FILE *fp = fopen(file, load_flag);
	if(fp == 0)
        goto __ret;

    fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
    data = new BYTE[size];

	fread(data, size, 1, fp);

    *data_seq = data;
    *data_size = size;

    ret = LW_RET_OK;
__ret:
    if(fp)
    {
	    fclose(fp);
    }

    return ret;
}

LW_END
