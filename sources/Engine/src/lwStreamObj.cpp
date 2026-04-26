//
#include "stdafx.h"
#include "lwStreamObj.h"
#include "lwDeviceObject.h"
#include "lwErrorCode.h"
#include "lwStdInc.h"

LW_BEGIN


LW_RESULT lwStreamObject::Init(DWORD buffer_size)
{
    _total_size = buffer_size;
    _locked_size = 0;
    _unused_size = _total_size;
    _free_size = _total_size;
    _lock_addr = 0;

    return LW_RET_OK;
}


LW_RESULT lwStreamObject::ReserveRoom(DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD a = size + stride;

    if(GetUnusedSize() < a)
        goto __ret;

    _unused_size -= a;
    _free_size -= a;

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwStreamObject::BindData(DWORD* out_off_addr, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;
    
    DWORD o = (GetLockAddr() / stride) * stride;

    if(o < GetLockAddr())
    {
        o += stride;
    }

    //assert(o / 4 * 4 == o);
    if((o + size) >= GetTotalSize())
        goto __ret;


    _lock_addr = o + size;
    _locked_size += size;

    *out_off_addr = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwStreamObject::UnbindData(DWORD size, DWORD stride)
{
    _free_size += (size + stride);
    return LW_RET_OK;
}

LW_RESULT lwStreamObject::ApplyRoom(DWORD* out_off_addr, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(GetUnusedSize() < size)
        goto __ret;

    {
        DWORD o = (GetLockAddr() / stride) * stride;

        if (o < GetLockAddr())
        {
            o += stride;
        }

        //assert(o / 4 * 4 == o);

        if ((o + size) >= GetTotalSize())
            goto __ret;

        *out_off_addr = o;

        _lock_addr = o + size;
        _unused_size = _total_size - _lock_addr;

    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwStreamObjVB
lwStreamObjVB::lwStreamObjVB(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0), _fvf(0)
{
}

lwStreamObjVB::~lwStreamObjVB()
{
    LoseDevice();
}

LW_RESULT lwStreamObjVB::Create(DWORD size, DWORD fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = lwStreamObject::Init(size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::Init failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    _fvf = fvf;

    if(LW_RESULT r = ResetDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] ResetDevice failed: size={}, fvf={}, ret={}",
            __FUNCTION__, size, fvf, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStreamObjVB::LoseDevice()
{
    LW_RESULT ret = LW_RET_OK;

    if(_buf)
    {
        IDirect3DVertexBufferX* vb = _buf;
        _buf = 0;
        if(LW_RESULT r = _dev_obj->ReleaseVertexBuffer(vb); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _dev_obj->ReleaseVertexBuffer failed: ret={}",
                __FUNCTION__, static_cast<long long>(r));
            ret = LW_RET_FAILED;
        }
    }

    // Reinit (como j fazia)
    if(LW_RESULT r = lwStreamObject::Init(_total_size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::Init failed: total_size={}, ret={}",
            __FUNCTION__, _total_size, static_cast<long long>(r));
        ret = LW_RET_FAILED;
    }

    return ret;
}
LW_RESULT lwStreamObjVB::ResetDevice()
{
    DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;

    return _dev_obj->CreateVertexBuffer(_total_size, usage, _fvf, D3DPOOL_DEFAULT, &_buf, LW_NULL);
}

LW_RESULT lwStreamObjVB::BindData(DWORD* out_off_addr, void* data, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = lwStreamObject::BindData(out_off_addr, size, stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::BindData(VB) failed: size={}, stride={}, ret={}",
            __FUNCTION__, size, stride, static_cast<long long>(r));
        goto __ret;
    }
    {
        D3DLOCK_TYPE* lock_buf;
        DWORD lock_flag = (*out_off_addr) == 0 ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE;

        if (HRESULT hr = _buf->Lock(*out_off_addr, size, (D3DLOCK_TYPE**)&lock_buf, lock_flag); FAILED(hr))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(VB) failed: offset={}, size={}, lock_flag={}, hr=0x{:08X}",
                __FUNCTION__, *out_off_addr, size, lock_flag, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        memcpy(lock_buf, data, size);

        _buf->Unlock();
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwStreamObjIB
lwStreamObjIB::lwStreamObjIB(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0), _format(D3DFMT_INDEX16)
{
}

lwStreamObjIB::~lwStreamObjIB()
{
    LoseDevice();
}

LW_RESULT lwStreamObjIB::Create(DWORD size, DWORD format)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = lwStreamObject::Init(size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::Init failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    _format = format;

    if(LW_RESULT r = ResetDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] ResetDevice failed: size={}, format={}, ret={}",
            __FUNCTION__, size, format, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwStreamObjIB::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev_obj->ReleaseIndexBuffer(_buf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->ReleaseIndexBuffer failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    _buf = 0;

    if(LW_RESULT r = lwStreamObject::Init(_total_size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::Init failed: total_size={}, ret={}",
            __FUNCTION__, _total_size, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwStreamObjIB::ResetDevice()
{
    DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;

    return _dev_obj->CreateIndexBuffer(_total_size, usage, (D3DFORMAT)_format, D3DPOOL_DEFAULT, &_buf, LW_NULL);
}

LW_RESULT lwStreamObjIB::BindData(DWORD* out_off_addr, void* data, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = lwStreamObject::BindData(out_off_addr, size, stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] lwStreamObject::BindData(IB) failed: size={}, stride={}, ret={}",
            __FUNCTION__, size, stride, static_cast<long long>(r));
        goto __ret;
    }

    {
        D3DLOCK_TYPE* lock_buf;
        DWORD lock_flag = (*out_off_addr) == 0 ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE;

        if (HRESULT hr = _buf->Lock(*out_off_addr, size, (D3DLOCK_TYPE**)&lock_buf, lock_flag); FAILED(hr))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(IB) failed: offset={}, size={}, lock_flag={}, hr=0x{:08X}",
                __FUNCTION__, *out_off_addr, size, lock_flag, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        memcpy(lock_buf, data, size);

        _buf->Unlock();
    }
    ret = LW_RET_OK;
__ret:
    return ret;

}
// lwStaticStreamMgr
// Static Stream Manager
LW_STD_IMPLEMENTATION(lwStaticStreamMgr)

lwStaticStreamMgr::lwStaticStreamMgr(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj)
{
    _index_entry_offset = 0;
    _vertex_entry_offset = 0;

    memset(_stream_vb_seq, 0, sizeof(_stream_vb_seq));
    memset(_stream_ib_seq, 0, sizeof(_stream_ib_seq));
    
    _entity_vb_seq = 0;
    _entity_ib_seq = 0;

    _entity_vb_num = 0;
    _entity_vb_seq_size = 0;
    _entity_ib_num = 0;
    _entity_ib_seq_size = 0;
    _next_entity_vb_id = 0;
    _next_entity_ib_id = 0;

    _stream_vb_num = 0;
    _stream_ib_num = 0;
}
lwStaticStreamMgr::~lwStaticStreamMgr()
{
    Destroy();
}

LW_RESULT lwStaticStreamMgr::Destroy()
{
    for(DWORD i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
    {
        LW_SAFE_DELETE(_stream_vb_seq[i]);
        LW_SAFE_DELETE(_stream_ib_seq[i]);
    }

    LW_SAFE_DELETE_A(_entity_vb_seq);
    LW_SAFE_DELETE_A(_entity_ib_seq);

    _entity_vb_num = 0;
    _entity_vb_seq_size = 0;
    _entity_ib_num = 0;
    _entity_ib_seq_size = 0;
    _next_entity_vb_id = 0;
    _next_entity_ib_id = 0;

    _stream_vb_num = 0;
    _stream_ib_num = 0;

    return LW_RET_OK;
}

LW_RESULT lwStaticStreamMgr::CreateStreamEntitySeq(DWORD entity_vb_size, DWORD entity_ib_size)
{
    _entity_vb_seq = LW_NEW(lwStreamEntity[entity_vb_size]);
    _entity_vb_num = 0;
    _entity_vb_seq_size = entity_vb_size;

    _entity_ib_seq = LW_NEW(lwStreamEntity[entity_ib_size]);
    _entity_ib_num = 0;
    _entity_ib_seq_size = entity_ib_size;

    _next_entity_vb_id = 0;
    _next_entity_ib_id = 0;

    return LW_RET_OK;
}

LW_RESULT lwStaticStreamMgr::CreateVertexBufferStream(DWORD stream_id, DWORD stream_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_stream_vb_seq[stream_id])
        goto __ret;
    {
        lwStreamObjVB* s = LW_NEW(lwStreamObjVB(_dev_obj));

        if (LW_RESULT r = s->Create(stream_size, 0); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->Create(VB stream) failed: stream_id={}, stream_size={}, ret={}",
                __FUNCTION__, stream_id, stream_size, static_cast<long long>(r));
            LW_DELETE(s);
            goto __ret;
        }

        _stream_vb_seq[stream_id] = s;
        _stream_vb_num += 1;

        s = 0;

    }
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwStaticStreamMgr::CreateIndexBufferStream(DWORD stream_id, DWORD stream_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_stream_ib_seq[stream_id])
        goto __ret;
    {
        lwStreamObjIB* s = LW_NEW(lwStreamObjIB(_dev_obj));

        if (LW_RESULT r = s->Create(stream_size, (DWORD)D3DFMT_INDEX16); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->Create(IB stream) failed: stream_id={}, stream_size={}, ret={}",
                __FUNCTION__, stream_id, stream_size, static_cast<long long>(r));
            LW_DELETE(s);
            goto __ret;
        }

        _stream_ib_seq[stream_id] = s;
        _stream_ib_num += 1;

        s = 0;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::RegisterVertexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_entity_vb_num >= _entity_vb_seq_size)
        goto __ret;
    {
        DWORD i;
        DWORD stream_id = 0;
        lwStreamObject* s;

        for (i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
        {
            if ((s = _stream_vb_seq[i]) == 0)
                continue;

            if (LW_SUCCEEDED(s->ReserveRoom(size, stride)))
            {
                stream_id = i;
                break;
            }
        }


        if (i == MAX_STREAM_SEQ_SIZE)
        {
            DWORD room_cost = 0;
            stream_id = LW_INVALID_INDEX;

            for (i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
            {
                if ((s = _stream_vb_seq[i]) == 0)
                    continue;

                DWORD free_size = s->GetFreeSize();
                if ((free_size > (size + stride)) && free_size > room_cost)
                {
                    room_cost = free_size;
                    stream_id = i;
                }
            }

            if (stream_id == LW_INVALID_INDEX)
            {
                // here need throw an exception
                goto __ret;
            }

            // reset stream
            s = _stream_vb_seq[stream_id];
            if (LW_RESULT r = s->Reset(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->Reset(VB) failed: stream_id={}, ret={}",
                    __FUNCTION__, stream_id, static_cast<long long>(r));
                goto __ret;
            }

            // reset entity
            lwStreamEntity* e = 0;
            for (i = 0; i < _entity_vb_seq_size; i++)
            {
                e = &_entity_vb_seq[i];

                if ((e->stream_id == stream_id) && (e->state != STREAMENTITY_STATE_INVALID))
                {
                    if (LW_RESULT r = s->ReserveRoom(e->size, e->stride); LW_FAILED(r))
                    {
                        ToLogService("errors", LogLevel::Error,
                            "[{}] s->ReserveRoom(VB realloc) failed: stream_id={}, e_size={}, e_stride={}, ret={}",
                            __FUNCTION__, stream_id, e->size, e->stride, static_cast<long long>(r));
                        goto __ret;
                    }

                    e->state = STREAMENTITY_STATE_INIT;
                }
            }

            if (LW_RESULT r = s->ReserveRoom(size, stride); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->ReserveRoom(VB new) failed: stream_id={}, size={}, stride={}, ret={}",
                    __FUNCTION__, stream_id, size, stride, static_cast<long long>(r));
                // this is impossible...@@
                goto __ret;
            }
        }
        {
            DWORD this_id = _next_entity_vb_id;
            lwStreamEntity* e = &_entity_vb_seq[this_id];

            e->stream_id = stream_id;
            e->data = data;
            e->size = size;
            e->stride = stride;
            e->state = STREAMENTITY_STATE_INIT;

            _entity_vb_num += 1;

            i = this_id + 1;
            if (i == _entity_vb_seq_size)
                i = 0;

            for (; i < _entity_vb_seq_size; i++)
            {
                if (_entity_vb_seq[i].state == STREAMENTITY_STATE_INVALID)
                {
                    _next_entity_vb_id = i;
                    break;
                }
            }

            *out_handle = this_id;
        }
    }
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwStaticStreamMgr::RegisterIndexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_entity_ib_num >= _entity_ib_seq_size)
        goto __ret;
    {
        DWORD i;
        DWORD stream_id = 0;
        lwStreamObject* s;

        for (i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
        {
            if ((s = _stream_ib_seq[i]) == 0)
                continue;

            if (LW_SUCCEEDED(s->ReserveRoom(size, stride)))
            {
                stream_id = i;
                break;
            }
        }

        if (i == MAX_STREAM_SEQ_SIZE)
        {
            DWORD room_cost = 0;
            stream_id = LW_INVALID_INDEX;

            for (i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
            {
                if ((s = _stream_ib_seq[i]) == 0)
                    continue;

                DWORD free_size = s->GetFreeSize();
                if ((free_size > (size + stride)) && free_size > room_cost)
                {
                    room_cost = free_size;
                    stream_id = i;
                }
            }

            if (stream_id == LW_INVALID_INDEX)
            {
                // here need throw an exception
                goto __ret;
            }

            // reset stream
            s = _stream_ib_seq[stream_id];
            if (LW_RESULT r = s->Reset(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->Reset(IB) failed: stream_id={}, ret={}",
                    __FUNCTION__, stream_id, static_cast<long long>(r));
                goto __ret;
            }

            // reset entity
            lwStreamEntity* e = 0;
            for (i = 0; i < _entity_ib_seq_size; i++)
            {
                e = &_entity_ib_seq[i];

                if ((e->stream_id == stream_id) && (e->state != STREAMENTITY_STATE_INVALID))
                {
                    if (LW_RESULT r = s->ReserveRoom(e->size, e->stride); LW_FAILED(r))
                    {
                        ToLogService("errors", LogLevel::Error,
                            "[{}] s->ReserveRoom(IB realloc) failed: stream_id={}, e_size={}, e_stride={}, ret={}",
                            __FUNCTION__, stream_id, e->size, e->stride, static_cast<long long>(r));
                        goto __ret;
                    }

                    e->state = STREAMENTITY_STATE_INIT;
                }
            }

            if (LW_RESULT r = s->ReserveRoom(size, stride); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->ReserveRoom(IB new) failed: stream_id={}, size={}, stride={}, ret={}",
                    __FUNCTION__, stream_id, size, stride, static_cast<long long>(r));
                // this is impossible...@@
                goto __ret;
            }
        }
        {
            DWORD this_id = _next_entity_ib_id;
            lwStreamEntity* e = &_entity_ib_seq[this_id];

            e->stream_id = stream_id;
            e->data = data;
            e->size = size;
            e->stride = stride;
            e->state = STREAMENTITY_STATE_INIT;

            _entity_ib_num += 1;

            i = this_id + 1;
            if (i == _entity_ib_seq_size)
                i = 0;

            for (; i < _entity_ib_seq_size; i++)
            {
                if (_entity_ib_seq[i].state == STREAMENTITY_STATE_INVALID)
                {
                    _next_entity_ib_id = i;
                    break;
                }
            }

            *out_handle = this_id;
        }
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::UnregisterVertexBuffer(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(handle >= _entity_vb_seq_size)
        goto __ret;
    {
        lwStreamEntity* e = &_entity_vb_seq[handle];
        lwStreamObject* s = _stream_vb_seq[e->stream_id];

        if (LW_RESULT r = s->UnbindData(e->size, e->stride); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->UnbindData(VB) failed: handle={}, size={}, stride={}, ret={}",
                __FUNCTION__, static_cast<std::uint64_t>(handle), e->size, e->stride, static_cast<long long>(r));
            goto __ret;
        }

        e->Reset();

        _entity_vb_num -= 1;

        if (_next_entity_vb_id > handle)
            _next_entity_vb_id = handle;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::UnregisterIndexBuffer(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(handle >= _entity_ib_seq_size)
        goto __ret;
    {
        lwStreamEntity* e = &_entity_ib_seq[handle];
        lwStreamObject* s = _stream_ib_seq[e->stream_id];

        if (LW_RESULT r = s->UnbindData(e->size, e->stride); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->UnbindData(IB) failed: handle={}, size={}, stride={}, ret={}",
                __FUNCTION__, static_cast<std::uint64_t>(handle), e->size, e->stride, static_cast<long long>(r));
            goto __ret;
        }

        e->Reset();

        _entity_ib_num -= 1;

        if (_next_entity_ib_id > handle)
            _next_entity_ib_id = handle;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::BindVertexBuffer(LW_HANDLE handle, UINT channel)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(handle >= _entity_vb_seq_size)
        goto __ret;

    {
        lwStreamEntity* e = &_entity_vb_seq[handle];
        lwStreamObjVB* s = _stream_vb_seq[e->stream_id];

        if (e->state == STREAMENTITY_STATE_INVALID)
            goto __ret;

        if (e->state == STREAMENTITY_STATE_INIT)
        {
            if (LW_RESULT r = s->BindData(&e->offset, e->data, e->size, e->stride); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->BindData(VB) failed: handle={}, size={}, stride={}, ret={}",
                    __FUNCTION__, static_cast<std::uint64_t>(handle), e->size, e->stride, static_cast<long long>(r));
                goto __ret;
            }

            e->state = STREAMENTITY_STATE_BIND;
        }



        //  Бьёмся offset'ом не на vertex-стороне, а через BaseVertexIndex
        //  в DrawIndexedPrimitive — см. lwMesh::DrawSubset.
        _dev_obj->SetStreamSource(channel, s->_buf, 0, e->stride);

        _vertex_entry_offset = e->offset / e->stride;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::BindIndexBuffer(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(handle >= _entity_ib_seq_size)
        goto __ret;
    {
        lwStreamEntity* e = &_entity_ib_seq[handle];
        lwStreamObjIB* s = _stream_ib_seq[e->stream_id];

        if (e->state == STREAMENTITY_STATE_INVALID)
            goto __ret;

        if (e->state == STREAMENTITY_STATE_INIT)
        {
            if (LW_RESULT r = s->BindData(&e->offset, e->data, e->size, e->stride); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] s->BindData(IB) failed: handle={}, size={}, stride={}, ret={}",
                    __FUNCTION__, static_cast<std::uint64_t>(handle), e->size, e->stride, static_cast<long long>(r));
                goto __ret;
            }

            e->state = STREAMENTITY_STATE_BIND;
        }

        //if(IsBadReadPtr(s->_buf,e->size))
        //	goto __ret;

        _dev_obj->SetIndices(s->_buf, _vertex_entry_offset);

        _index_entry_offset = e->offset / e->stride;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
    {
        if(_stream_vb_seq[i])
        {
            if(LW_RESULT r = _stream_vb_seq[i]->LoseDevice(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] _stream_vb_seq[{}]->LoseDevice failed: ret={}",
                    __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
        }
        if(_stream_ib_seq[i])
        {
            if(LW_RESULT r = _stream_ib_seq[i]->LoseDevice(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] _stream_ib_seq[{}]->LoseDevice failed: ret={}",
                    __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
        }
    }

    for(DWORD i = 0; i < _entity_vb_seq_size; i++)
    {
        if(_entity_vb_seq[i].state == STREAMENTITY_STATE_BIND)
        {
            _entity_vb_seq[i].state = STREAMENTITY_STATE_INIT;
        }
    }


    for(DWORD i = 0; i < _entity_ib_seq_size; i++)
    {
        if(_entity_ib_seq[i].state == STREAMENTITY_STATE_BIND)
        {
            _entity_ib_seq[i].state = STREAMENTITY_STATE_INIT;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwStaticStreamMgr::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
    {
        if(_stream_vb_seq[i])
        {
            if(LW_RESULT r = _stream_vb_seq[i]->ResetDevice(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] _stream_vb_seq[{}]->ResetDevice failed: ret={}",
                    __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
        }
        if(_stream_ib_seq[i])
        {
            if(LW_RESULT r = _stream_ib_seq[i]->ResetDevice(); LW_FAILED(r))
            {
                ToLogService("errors", LogLevel::Error,
                    "[{}] _stream_ib_seq[{}]->ResetDevice failed: ret={}",
                    __FUNCTION__, i, static_cast<long long>(r));
                goto __ret;
            }
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwStaticStreamMgr::GetDebugInfo(lwStaticStreamMgrDebugInfo* info)
{
    DWORD size = 0;
    lwStreamObject* s;

    memset(info, 0, sizeof(lwStaticStreamMgrDebugInfo));

    for(DWORD i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
    {
        if((s = _stream_vb_seq[i]) != 0)
        {
            info->vbs_size += s->GetTotalSize();
            info->vbs_free_size += s->GetFreeSize();
            info->vbs_used_size += s->GetTotalSize() - s->GetFreeSize();
            info->vbs_unused_size += s->GetUnusedSize();
            info->vbs_locked_size += s->GetLockedSize();
        }
    }

    for(DWORD i = 0; i < MAX_STREAM_SEQ_SIZE; i++)
    {
        if((s = _stream_ib_seq[i]) != 0)
        {
            info->ibs_size += s->GetTotalSize();
            info->ibs_free_size += s->GetFreeSize();
            info->ibs_used_size += s->GetTotalSize() - s->GetFreeSize();
            info->ibs_unused_size += s->GetUnusedSize();
            info->ibs_locked_size += s->GetLockedSize();
        }
    }

    return LW_RET_OK;
}

// lwDynamicStream
lwDynamicStream::lwDynamicStream()
: _total_size(0), _free_size(0), _free_addr(0), _base_index(0), _fvf(0)
{
}
lwDynamicStream::~lwDynamicStream()
{
}

LW_RESULT lwDynamicStream::GetEntryOffset(DWORD* offset, DWORD size, DWORD stride)
{
    DWORD v_size = GetValidSize();

    if(v_size < size)
        return LW_RET_FAILED;

    DWORD o = (GetFreeAddr() / stride) * stride;

    if(o < GetFreeAddr())
    {
        o += stride;
    }

    //assert(o / 4 * 4 == o);

    if((o + size) >= GetTotalSize())
        return LW_RET_FAILED;

    *offset = o;

    return LW_RET_OK;

}

LW_RESULT lwDynamicStream::ResetFreeSize(DWORD size, DWORD offset)
{
    _free_size -= size;
    _free_addr = offset + size;

    if(_free_size <= 0)
    {
        _free_size = _total_size;
        _free_addr = 0;
    }

    return LW_RET_OK;
}
// lwDynamicStreamVB
lwDynamicStreamVB::lwDynamicStreamVB(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0)
{
}

lwDynamicStreamVB::~lwDynamicStreamVB()
{
    if(_buf) // s chama se ainda existir
        LoseDevice();
}

LW_RESULT lwDynamicStreamVB::Create(DWORD buf_size, D3DFORMAT fmt)
{
    _total_size = buf_size;
    _free_size = _total_size;
    _free_addr = 0;
    _fvf = fmt;

    return ResetDevice();
}
LW_RESULT lwDynamicStreamVB::LoseDevice()
{
    
    if(_buf)
    {
        IDirect3DVertexBufferX* vb = _buf;
        _buf = 0;
        _dev_obj->ReleaseVertexBuffer(vb);
    }
    return LW_RET_OK;
}
LW_RESULT lwDynamicStreamVB::ResetDevice()
{
    DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
    return _dev_obj->CreateVertexBuffer(_total_size, usage, _fvf, D3DPOOL_DEFAULT, &_buf, LW_NULL);
}

LW_RESULT lwDynamicStreamVB::Bind(DWORD channel, const void* data, DWORD size, DWORD stride)
{
    DWORD offset = 0;
    DWORD lock_flag = D3DLOCK_NOOVERWRITE;
    D3DLOCK_TYPE* lock_buf = 0;
    
    if(LW_RESULT r = GetEntryOffset(&offset, size, stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Warning,
            "[{}] GetEntryOffset(VB first) failed; resetting and retrying: size={}, stride={}, ret={}",
            __FUNCTION__, size, stride, static_cast<long long>(r));
        _free_addr = 0;
        _free_size = _total_size;

        if(LW_RESULT r2 = GetEntryOffset(&offset, size, stride); LW_FAILED(r2))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] GetEntryOffset(VB retry) failed: size={}, stride={}, ret={}",
                __FUNCTION__, size, stride, static_cast<long long>(r2));
            return LW_RET_FAILED;
        }

        lock_flag = D3DLOCK_DISCARD;
    }

    lock_flag = (offset == 0) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE;

    if(HRESULT hr = _buf->Lock(offset, lock_flag == D3DLOCK_DISCARD ? 0 : size, (D3DLOCK_TYPE**)&lock_buf, lock_flag); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(VB) failed: offset={}, size={}, lock_flag={}, hr=0x{:08X}",
            __FUNCTION__, offset, size, lock_flag, static_cast<std::uint32_t>(hr));
        return LW_RET_FAILED;
    }

    memcpy(lock_buf, data, size);

    _buf->Unlock();

    //  offset на vertex-стороне не используем — bind с нуля,
    //  смещение применяет DrawIndexedPrimitive через BaseVertexIndex.
    _dev_obj->SetStreamSource(channel, _buf, 0, stride);

    _base_index = offset / stride;

    ResetFreeSize(size, offset);

    return LW_RET_OK;

}

// lwDynamicStreamIB
lwDynamicStreamIB::lwDynamicStreamIB(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0)
{
}

lwDynamicStreamIB::~lwDynamicStreamIB()
{
    if(_buf)
        LoseDevice();
}

LW_RESULT lwDynamicStreamIB::Create(DWORD buf_size, D3DFORMAT fmt)
{
    _total_size = buf_size;
    _free_size = _total_size;
    _free_addr = 0;
    _fvf = fmt;

    return ResetDevice();

}
LW_RESULT lwDynamicStreamIB::LoseDevice()
{
    if(_buf)
    {
        IDirect3DIndexBufferX* ib = _buf;
        _buf = 0;
        _dev_obj->ReleaseIndexBuffer(ib);
    }
    return LW_RET_OK;
}
LW_RESULT lwDynamicStreamIB::ResetDevice()
{
    DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
    return _dev_obj->CreateIndexBuffer(_total_size, usage, (D3DFORMAT)_fvf, D3DPOOL_DEFAULT, &_buf, LW_NULL);
}

LW_RESULT lwDynamicStreamIB::Bind(DWORD vert_index, const void* data, DWORD size, DWORD stride)
{
    DWORD offset = 0;
    DWORD lock_flag = D3DLOCK_NOOVERWRITE;
    D3DLOCK_TYPE* lock_buf = 0;
    
    if(LW_RESULT r = GetEntryOffset(&offset, size, stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Warning,
            "[{}] GetEntryOffset(IB first) failed; resetting and retrying: size={}, stride={}, ret={}",
            __FUNCTION__, size, stride, static_cast<long long>(r));
        _free_addr = 0;
        _free_size = _total_size;

        if(LW_RESULT r2 = GetEntryOffset(&offset, size, stride); LW_FAILED(r2))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] GetEntryOffset(IB retry) failed: size={}, stride={}, ret={}",
                __FUNCTION__, size, stride, static_cast<long long>(r2));
            return LW_RET_FAILED;
        }

        lock_flag = D3DLOCK_DISCARD;
    }

    if(HRESULT hr = _buf->Lock(offset, size, (D3DLOCK_TYPE**)&lock_buf, lock_flag); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(IB) failed: offset={}, size={}, lock_flag={}, hr=0x{:08X}",
            __FUNCTION__, offset, size, lock_flag, static_cast<std::uint32_t>(hr));
        return LW_RET_FAILED;
    }

    memcpy(lock_buf, data, size);

    _buf->Unlock();

    _dev_obj->SetIndices(_buf, vert_index);

    _base_index = offset / stride;

    ResetFreeSize(size, offset);

    return LW_RET_OK;

}

// lwDynamicStreamMgr
LW_STD_IMPLEMENTATION(lwDynamicStreamMgr)

DWORD lwDynamicStreamMgr::_GetVerticesSize(D3DPRIMITIVETYPE pt_type, DWORD count, DWORD stride)
{
    DWORD size = 0;

    switch(pt_type)
    {
    case D3DPT_POINTLIST:
        size = stride * count;
        break;
    case D3DPT_LINESTRIP:
        size = stride * (count + 1);
        break;
    case D3DPT_LINELIST:
        size = stride * count * 2;
        break;
    case D3DPT_TRIANGLESTRIP:
        size = stride * (count + 2);
        break;
    case D3DPT_TRIANGLELIST:
        size = stride * count * 3;
        break;
    default:
        ;
    }

    return size;
}

DWORD lwDynamicStreamMgr::_GetIndicesSize(D3DPRIMITIVETYPE pt_type, DWORD count, DWORD stride)
{
    DWORD size = 0;

    switch(pt_type)
    {
    case D3DPT_POINTLIST:
        size = stride * count;
        break;
    case D3DPT_LINESTRIP:
        size = stride * (count + 1);
        break;
    case D3DPT_LINELIST:
        size = stride * count * 2;
        break;
    case D3DPT_TRIANGLESTRIP:
        size = stride * (count + 2);
        break;
    case D3DPT_TRIANGLELIST:
        size = stride * count * 3;
        break;
    default:
        ;
    }

    return size;
}

LW_RESULT lwDynamicStreamMgr::Create(DWORD vb_size, DWORD ib_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _vb.Create(vb_size, (D3DFORMAT)0); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _vb.Create failed: vb_size={}, ret={}",
            __FUNCTION__, vb_size, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _ib.Create(ib_size, D3DFMT_INDEX16); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _ib.Create failed: ib_size={}, ret={}",
            __FUNCTION__, ib_size, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwDynamicStreamMgr::DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT count, const void* vert_data, UINT vert_stride, D3DFORMAT fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD vert_size = _GetVerticesSize(pt_type, count, vert_stride);

    if(LW_RESULT r = _vb.Bind(0, vert_data, vert_size, vert_stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _vb.Bind failed: pt_type={}, count={}, vert_size={}, vert_stride={}, ret={}",
            __FUNCTION__, static_cast<std::uint32_t>(pt_type), count, vert_size, vert_stride, static_cast<long long>(r));
        goto __ret;
    }

    if(HRESULT hr = _dev_obj->SetFVF(fvf); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->SetFVF failed: fvf={}, hr=0x{:08X}",
            __FUNCTION__, static_cast<std::uint32_t>(fvf), static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    if(HRESULT hr = _dev_obj->DrawPrimitive(pt_type, _vb.GetBaseIndex(), count); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->DrawPrimitive failed: pt_type={}, count={}, hr=0x{:08X}",
            __FUNCTION__, static_cast<std::uint32_t>(pt_type), count, static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT lwDynamicStreamMgr::DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, UINT min_vert_index, UINT num_vert_indices, UINT count,
        const void* index_data, D3DFORMAT index_format, const void* vertex_data, UINT vert_stride, D3DFORMAT fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD index_stride = index_format == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);
    DWORD vert_size = _GetVerticesSize(pt_type, count, vert_stride);
    DWORD index_size = _GetIndicesSize(pt_type, count, index_stride);

    if(LW_RESULT r = _vb.Bind(0, vertex_data, vert_size, vert_stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _vb.Bind(idx) failed: pt_type={}, vert_size={}, vert_stride={}, ret={}",
            __FUNCTION__, static_cast<std::uint32_t>(pt_type), vert_size, vert_stride, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _ib.Bind(_vb.GetBaseIndex(), index_data, index_size , index_stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _ib.Bind(idx) failed: pt_type={}, index_size={}, index_stride={}, ret={}",
            __FUNCTION__, static_cast<std::uint32_t>(pt_type), index_size, index_stride, static_cast<long long>(r));
        goto __ret;
    }

    if(HRESULT hr = _dev_obj->SetFVF(fvf); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->SetFVF(idx) failed: fvf={}, hr=0x{:08X}",
            __FUNCTION__, static_cast<std::uint32_t>(fvf), static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    if(HRESULT hr = _dev_obj->DrawIndexedPrimitive(pt_type, 0, min_vert_index, num_vert_indices, _ib.GetBaseIndex(), count); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->DrawIndexedPrimitive failed: pt_type={}, num_vert={}, count={}, hr=0x{:08X}",
            __FUNCTION__, static_cast<std::uint32_t>(pt_type), num_vert_indices, count, static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}

LW_RESULT lwDynamicStreamMgr::BindDataVB(DWORD channel, const void* data, DWORD size, UINT stride)
{
    return _vb.Bind(channel, data, size, stride);
}
LW_RESULT lwDynamicStreamMgr::BindDataIB(const void* data, DWORD size, DWORD stride)
{
    return _ib.Bind(_vb.GetBaseIndex(), data, size, stride);
}
LW_RESULT lwDynamicStreamMgr::DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT start_vert, UINT count)
{
    return _dev_obj->DrawPrimitive(pt_type, _vb.GetBaseIndex() + start_vert, count);
}
LW_RESULT lwDynamicStreamMgr::DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, INT base_vert_index, UINT min_index, UINT num_vert, UINT start_index, UINT count)
{
    return _dev_obj->DrawIndexedPrimitive(pt_type, base_vert_index, min_index, num_vert, _ib.GetBaseIndex() + start_index, count);
}

LW_RESULT lwDynamicStreamMgr::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _vb.LoseDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _vb.LoseDevice failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _ib.LoseDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _ib.LoseDevice failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;


}
LW_RESULT lwDynamicStreamMgr::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _vb.ResetDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _vb.ResetDevice failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _ib.ResetDevice(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _ib.ResetDevice failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

// lwLockableStreamVB
LW_STD_IMPLEMENTATION(lwLockableStreamVB)

LW_RESULT lwLockableStreamVB::Create(void* data, DWORD size, DWORD usage, DWORD fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev_obj->CreateVertexBuffer(size, usage, fvf, D3DPOOL_DEFAULT, &_buf, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->CreateVertexBuffer failed: size={}, usage={}, fvf={}, ret={}",
            __FUNCTION__, size, usage, fvf, static_cast<long long>(r));
        goto __ret;
    }

    D3DLOCK_TYPE* p;

    if(LW_RESULT r = _buf->Lock(0, 0, &p, 0); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(VB Create) failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    memcpy(p, data, size);

    if(LW_RESULT r = _buf->Unlock(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Unlock(VB Create) failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    _size = size;
    _usage = usage;
    _fvf = fvf;
    _data = (BYTE*)data;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwLockableStreamVB::Destroy()
{
    Reset();
    LoseDevice();

    return LW_RET_OK;
}

LW_RESULT lwLockableStreamVB::Lock(UINT offset, UINT size, void** data, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || _lock_cnt > 0 || (offset + size) >= _size)
        goto __ret;

    _lock_offset = offset;
    _lock_size = size;
    _lock_flag = flag;
    *data = &_data[_lock_offset];
    _lock_cnt += 1;

    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwLockableStreamVB::Unlock()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || _lock_cnt == 0)
        goto __ret;

    D3DLOCK_TYPE* p;
    void* d;
    DWORD s;

    if(LW_RESULT r = _buf->Lock(_lock_offset, _lock_size, &p, _lock_flag); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(VB Unlock) failed: offset={}, size={}, flag={}, ret={}",
            __FUNCTION__, _lock_offset, _lock_size, _lock_flag, static_cast<long long>(r));
        goto __ret;
    }

    if(_lock_offset == 0 && _lock_size == 0)
    {
        d = _data;
        s = _size;
    }
    else
    {
        d = &_data[_lock_offset];
        s = _lock_size;
    }

    memcpy(p, d, s);

    _buf->Unlock();

    _lock_cnt -= 1;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwLockableStreamVB::Bind(UINT channel, UINT offset_byte, UINT stride)
{
    return _dev_obj->SetStreamSource(channel, _buf, offset_byte, stride);
}

LW_RESULT lwLockableStreamVB::LoseDevice()
{
    if(_buf)
    {
        IDirect3DVertexBufferX* vb = _buf;
        _buf = 0; 
        _dev_obj->ReleaseVertexBuffer(vb);
    }
    return LW_RET_OK;
}

LW_RESULT lwLockableStreamVB::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 && _size != 0)
    {
        if(LW_RESULT r = _dev_obj->CreateVertexBuffer(_size, _usage, _fvf, D3DPOOL_DEFAULT, &_buf, NULL); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _dev_obj->CreateVertexBuffer(reset VB) failed: size={}, usage={}, fvf={}, ret={}",
                __FUNCTION__, _size, _usage, _fvf, static_cast<long long>(r));
            goto __ret;
        }

        void* p;

        if(LW_RESULT r = Lock(0, 0, &p, 0); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] Lock(reset VB) failed: ret={}",
                __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

        if(LW_RESULT r = Unlock(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] Unlock(reset VB) failed: ret={}",
                __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}


// lwLockableStreamIB
LW_STD_IMPLEMENTATION(lwLockableStreamIB)

LW_RESULT lwLockableStreamIB::Create(void* data, DWORD size, DWORD usage, DWORD fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev_obj->CreateIndexBuffer(size, usage, (D3DFORMAT)fvf, D3DPOOL_DEFAULT, &_buf, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->CreateIndexBuffer failed: size={}, usage={}, fvf={}, ret={}",
            __FUNCTION__, size, usage, fvf, static_cast<long long>(r));
        goto __ret;
    }

    D3DLOCK_TYPE* p;

    if(LW_RESULT r = _buf->Lock(0, 0, &p, 0); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(IB Create) failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    memcpy(p, data, size);

    if(LW_RESULT r = _buf->Unlock(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Unlock(IB Create) failed: size={}, ret={}",
            __FUNCTION__, size, static_cast<long long>(r));
        goto __ret;
    }

    _size = size;
    _usage = usage;
    _fvf = fvf;
    _data = (BYTE*)data;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwLockableStreamIB::Destroy()
{
    Reset();
    LoseDevice();

    return LW_RET_OK;
}

LW_RESULT lwLockableStreamIB::Lock(UINT offset, UINT size, void** data, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || _lock_cnt > 0 || (offset + size) >= _size)
        goto __ret;

    _lock_offset = offset;
    _lock_size = size;
    _lock_flag = flag;
    *data = &_data[_lock_offset];
    _lock_cnt += 1;

    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwLockableStreamIB::Unlock()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || _lock_cnt == 0)
        goto __ret;

    D3DLOCK_TYPE* p;
    void* d;
    DWORD s;

    if(LW_RESULT r = _buf->Lock(_lock_offset, _lock_size, &p, _lock_flag); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _buf->Lock(IB Unlock) failed: offset={}, size={}, flag={}, ret={}",
            __FUNCTION__, _lock_offset, _lock_size, _lock_flag, static_cast<long long>(r));
        goto __ret;
    }

    if(_lock_offset == 0 && _lock_size == 0)
    {
        d = _data;
        s = _size;
    }
    else
    {
        d = &_data[_lock_offset];
        s = _lock_size;
    }

    memcpy(p, d, s);

    _buf->Unlock();

    _lock_cnt -= 1;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwLockableStreamIB::Bind(UINT base_vert_index)
{
    return _dev_obj->SetIndices(_buf, base_vert_index);
}

LW_RESULT lwLockableStreamIB::LoseDevice()
{
    if(_buf)
    {
        IDirect3DIndexBufferX* ib = _buf;
        _buf = 0;
        _dev_obj->ReleaseIndexBuffer(ib);
    }
    return LW_RET_OK;
}
LW_RESULT lwLockableStreamIB::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 && _size != 0)
    {
        if(LW_RESULT r = _dev_obj->CreateIndexBuffer(_size, _usage, (D3DFORMAT)_fvf, D3DPOOL_DEFAULT, &_buf, NULL); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _dev_obj->CreateIndexBuffer(reset IB) failed: size={}, usage={}, fvf={}, ret={}",
                __FUNCTION__, _size, _usage, _fvf, static_cast<long long>(r));
            goto __ret;
        }

        void* p;

        if(LW_RESULT r = Lock(0, 0, &p, D3DLOCK_DISCARD); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] Lock(reset IB) failed: ret={}",
                __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }

        if(LW_RESULT r = Unlock(); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] Unlock(reset IB) failed: ret={}",
                __FUNCTION__, static_cast<long long>(r));
            goto __ret;
        }
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}

// lwLockableStreamMgr
LW_STD_IMPLEMENTATION(lwLockableStreamMgr)

lwLockableStreamMgr::lwLockableStreamMgr(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
}
lwLockableStreamMgr::~lwLockableStreamMgr()
{
    LoseDevice();
}

LW_RESULT lwLockableStreamMgr::RegisterVertexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamVB* s = LW_NEW(lwLockableStreamVB(_res_mgr->GetDeviceObject()));

    if(LW_RESULT r = s->Create(data, size, usage, fvf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->Create(VB) failed: size={}, usage={}, fvf={}, ret={}",
            __FUNCTION__, size, usage, fvf, static_cast<long long>(r));
        goto __ret;
    }

    DWORD ret_id;

    if(LW_RESULT r = _pool_vb.Register(&ret_id, s); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_vb.Register failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    s =  0;

    *handle = ret_id;

    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE(s);
    return ret;
}
LW_RESULT lwLockableStreamMgr::RegisterIndexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamIB* s = LW_NEW(lwLockableStreamIB(_res_mgr->GetDeviceObject()));

    if(LW_RESULT r = s->Create(data, size, usage, fvf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->Create(IB) failed: size={}, usage={}, fvf={}, ret={}",
            __FUNCTION__, size, usage, fvf, static_cast<long long>(r));
        goto __ret;
    }

    DWORD ret_id;

    if(LW_RESULT r = _pool_ib.Register(&ret_id, s); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_ib.Register failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    s =  0;

    *handle = ret_id;

    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE(s);
    return ret;
}

LW_RESULT lwLockableStreamMgr::UnregisterVertexBuffer(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamVB* s;
    ret = _pool_vb.Unregister((lwPoolEntity*)&s, handle);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_vb.Unregister failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(ret));
        return ret;
    }

    if(ret == LW_RET_OK_1)
    {
        // ~lwLockableStreamVB chama LoseDevice(), agora idempotente
        LW_DELETE(s);
    }

    return LW_RET_OK;
}
LW_RESULT lwLockableStreamMgr::UnregisterIndexBuffer(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamIB* s;

    ret = _pool_ib.Unregister((lwPoolEntity*)&s, handle);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_ib.Unregister failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(ret));
        goto __ret;
    }

    if(ret == LW_RET_OK_1)
    {
        LW_DELETE(s);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwLockableStreamMgr::BindVertexBuffer(LW_HANDLE handle, UINT channel, UINT offset_byte, UINT stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamVB* s;

     if(LW_RESULT r = _pool_vb.GetObj((lwPoolEntity*)&s, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_vb.GetObj failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = s->Bind(channel, offset_byte, stride); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->Bind(VB) failed: handle={}, channel={}, offset={}, stride={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), channel, offset_byte, stride, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwLockableStreamMgr::BindIndexBuffer(LW_HANDLE handle, UINT base_vert_index)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwLockableStreamIB* s;

    if(LW_RESULT r = _pool_ib.GetObj((lwPoolEntity*)&s, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_ib.GetObj failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = s->Bind(base_vert_index); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->Bind(IB) failed: handle={}, base_vert_index={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), base_vert_index, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwLockableStreamMgr::LoseDevice()
{
    LW_RESULT ret = LW_RET_OK;

    _pool_vb.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* vb = static_cast<lwLockableStreamVB*>(raw);
        if (LW_RESULT r = vb->LoseDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] vb->LoseDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    if (LW_FAILED(ret)) {
        return ret;
    }

    _pool_ib.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* ib = static_cast<lwLockableStreamIB*>(raw);
        if (LW_RESULT r = ib->LoseDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] ib->LoseDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    return ret;
}
LW_RESULT lwLockableStreamMgr::ResetDevice()
{
    LW_RESULT ret = LW_RET_OK;

    _pool_vb.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* vb = static_cast<lwLockableStreamVB*>(raw);
        if (LW_RESULT r = vb->ResetDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] vb->ResetDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    if (LW_FAILED(ret)) {
        return ret;
    }

    _pool_ib.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* ib = static_cast<lwLockableStreamIB*>(raw);
        if (LW_RESULT r = ib->ResetDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] ib->ResetDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    return ret;
}

lwILockableStreamVB* lwLockableStreamMgr::GetStreamVB(LW_HANDLE handle)
{
    lwLockableStreamVB* s = 0;

    if(LW_RESULT r = _pool_vb.GetObj((lwPoolEntity*)&s, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_vb.GetObj failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }
__ret:
    return (lwILockableStreamVB*)s;
}
lwILockableStreamIB* lwLockableStreamMgr::GetStreamIB(LW_HANDLE handle)
{
    lwLockableStreamIB* s = 0;

    if(LW_RESULT r = _pool_ib.GetObj((lwPoolEntity*)&s, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_ib.GetObj failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }
__ret:
    return (lwILockableStreamIB*)s;
}

// lwSurfaceStream
//LW_STD_IMPLEMENTATION(lwSurfaceStream)
LW_STD_GETINTERFACE(lwSurfaceStream)

LW_RESULT lwSurfaceStream::Release()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _ss_mgr->UnregisterSurface(_reg_id); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _ss_mgr->UnregisterSurface failed: reg_id={}, ret={}",
            __FUNCTION__, _reg_id, static_cast<long long>(r));
        goto __ret;
    }

    LW_DELETE(this);

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwSurfaceStream::lwSurfaceStream(lwISurfaceStreamMgr* ss_mgr)
: _ss_mgr(ss_mgr)
{
    _type = SURFACESTREAM_INVALID;
    _width = 0;
    _height = 0;
    _format = D3DFMT_UNKNOWN;
    _multi_sample = D3DMULTISAMPLE_NONE;
    _multi_sample_quality = 0;
    _surface = 0;
}

lwSurfaceStream::~lwSurfaceStream()
{
    LoseDevice();
}

LW_RESULT lwSurfaceStream::CreateRenderTarget(UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL lockable, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _ss_mgr->GetResourceMgr()->GetDeviceObject();

    if(HRESULT hr = dev_obj->CreateRenderTarget(&_surface, width, height, format, multi_sample, multi_sample_quality, lockable, NULL); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] dev_obj->CreateRenderTarget failed: w={}, h={}, format={}, lockable={}, hr=0x{:08X}",
            __FUNCTION__, width, height, static_cast<std::uint32_t>(format), lockable, static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    _type = SURFACESTREAM_RENDERTARGET;
    _width = width;
    _height = height;
    _format = format;
    _lockable = lockable;
    _multi_sample = multi_sample;
    _multi_sample_quality = multi_sample_quality;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwSurfaceStream::CreateDepthStencilSurface(UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL discard, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _ss_mgr->GetResourceMgr()->GetDeviceObject();

    if(HRESULT hr = dev_obj->CreateDepthStencilSurface(&_surface, width, height, format, multi_sample, multi_sample_quality, discard, handle); FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] dev_obj->CreateDepthStencilSurface failed: w={}, h={}, format={}, discard={}, hr=0x{:08X}",
            __FUNCTION__, width, height, static_cast<std::uint32_t>(format), discard, static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    _type = SURFACESTREAM_DEPTHSTENCIL;
    _width = width;
    _height = height;
    _format = format;
    _discard = discard;
    _multi_sample = multi_sample;
    _multi_sample_quality = multi_sample_quality;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwSurfaceStream::LoseDevice()
{
    LW_SAFE_RELEASE(_surface);
    return LW_RET_OK;
}
LW_RESULT lwSurfaceStream::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _ss_mgr->GetResourceMgr()->GetDeviceObject();

    if(_type == SURFACESTREAM_RENDERTARGET)
    {
        ret = dev_obj->CreateRenderTarget(&_surface, _width, _height, (D3DFORMAT)_format, _multi_sample, _multi_sample_quality, _lockable, NULL);
    }
    else if(_type == SURFACESTREAM_DEPTHSTENCIL)
    {
        ret = dev_obj->CreateDepthStencilSurface(&_surface, _width, _height, (D3DFORMAT)_format, _multi_sample, _multi_sample_quality, _discard, NULL);
    }

    return ret;
}

// lwSurfaceStreamMgr
LW_STD_IMPLEMENTATION(lwSurfaceStreamMgr)

lwSurfaceStreamMgr::lwSurfaceStreamMgr(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
}

lwSurfaceStreamMgr::~lwSurfaceStreamMgr()
{
    LoseDevice();
}

LW_RESULT lwSurfaceStreamMgr::LoseDevice()
{
    LW_RESULT ret = LW_RET_OK;
    _pool_surface.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* s = static_cast<lwISurfaceStream*>(raw);
        if (LW_RESULT r = s->LoseDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->LoseDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    return ret;
}
LW_RESULT lwSurfaceStreamMgr::ResetDevice()
{
    LW_RESULT ret = LW_RET_OK;
    _pool_surface.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* s = static_cast<lwISurfaceStream*>(raw);
        if (LW_RESULT r = s->ResetDevice(); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                "[{}] s->ResetDevice failed: handle={}, ret={}",
                __FUNCTION__, handle, static_cast<long long>(r));
            ret = LW_RET_FAILED;
            return false;
        }
        return true;
    });
    return ret;
}

LW_RESULT lwSurfaceStreamMgr::CreateRenderTarget(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL lockable, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwISurfaceStream* s = LW_NEW(lwSurfaceStream(this));

    if(LW_RESULT r = s->CreateRenderTarget(width, height, format, multi_sample, multi_sample_quality, lockable, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->CreateRenderTarget failed: w={}, h={}, format={}, lockable={}, ret={}",
            __FUNCTION__, width, height, static_cast<std::uint32_t>(format), lockable, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _pool_surface.Register(ret_handle, s); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_surface.Register(RT) failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    s->SetRegisterID(*ret_handle);
    s = 0;

    ret = LW_RET_OK;
__ret:
    LW_IF_RELEASE(s);
    return ret;

}
LW_RESULT lwSurfaceStreamMgr::CreateDepthStencilSurface(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL discard, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwISurfaceStream* s = LW_NEW(lwSurfaceStream(this));

    if(LW_RESULT r = s->CreateDepthStencilSurface(width, height, format, multi_sample, multi_sample_quality, discard, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] s->CreateDepthStencilSurface failed: w={}, h={}, format={}, discard={}, ret={}",
            __FUNCTION__, width, height, static_cast<std::uint32_t>(format), discard, static_cast<long long>(r));
        goto __ret;
    }

    if(LW_RESULT r = _pool_surface.Register(ret_handle, s); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_surface.Register(DS) failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    s->SetRegisterID(*ret_handle);
    s = 0;

    ret = LW_RET_OK;
__ret:
    LW_IF_RELEASE(s);
    return ret;

}
LW_RESULT lwSurfaceStreamMgr::UnregisterSurface(LW_HANDLE handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwISurfaceStream* ss;

    if(LW_RESULT r = _pool_surface.Unregister((void**)&ss, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_surface.Unregister failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }

    if(ss)
    {
        ss->SetRegisterID(LW_INVALID_INDEX);
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

lwISurfaceStream* lwSurfaceStreamMgr::GetSurfaceStream(LW_HANDLE handle)
{
    lwISurfaceStream* ret = 0;

    if(LW_RESULT r = _pool_surface.GetObj((void**)&ret, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _pool_surface.GetObj failed: handle={}, ret={}",
            __FUNCTION__, static_cast<std::uint64_t>(handle), static_cast<long long>(r));
        goto __ret;
    }

__ret:
    return ret;
}

//lwVertexBuffer
//LW_STD_IMPLEMENTATION(lwVertexBuffer)
LW_STD_GETINTERFACE(lwVertexBuffer);

LW_RESULT lwVertexBuffer::Release()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = Destroy(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] Destroy(VB) failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }
    {
        lwDeviceObject* devobj = reinterpret_cast<lwDeviceObject*>(_dev_obj);
        devobj->_ReleaseStreamBuffer(this);

        LW_DELETE(this);
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

lwVertexBuffer::lwVertexBuffer(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0), _next(0), _prev(0)
{
    _dlock_pos = 0;
    memset(&_buf_info, 0, sizeof(_buf_info));
}
lwVertexBuffer::~lwVertexBuffer()
{
}

LW_RESULT lwVertexBuffer::Create(UINT length, DWORD usage, DWORD fvf, D3DPOOL pool, DWORD stride, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev_obj->CreateVertexBuffer(length, usage, fvf, pool, &_buf, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->CreateVertexBuffer failed: length={}, usage={}, fvf={}, pool={}, ret={}",
            __FUNCTION__, length, usage, fvf, static_cast<std::uint32_t>(pool), static_cast<long long>(r));
        goto __ret;
    }

    _buf_info.fvf = fvf;
    _buf_info.size = length;
    _buf_info.usage = usage;
    _buf_info.stride = stride;
    _buf_info.pool = pool;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwVertexBuffer::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0)
        goto __addre_ret_ok;

    if(LW_RESULT r = _dev_obj->ReleaseVertexBuffer(_buf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->ReleaseVertexBuffer failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    _buf = 0;
    memset(&_buf_info, 0, sizeof(_buf_info));

__addre_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwVertexBuffer::LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0)
        goto __ret;

    if(data_seq == 0 || data_size == 0)
        goto __ret;

    if((offset + data_size) > _buf_info.size)
        goto __ret;
    {
        D3DLOCK_TYPE* p_buf = 0;

        if (LW_RESULT r = _buf->Lock(offset, (data_size == _buf_info.size) ? 0 : data_size, &p_buf, lock_flag); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(VB LoadData) failed: offset={}, data_size={}, lock_flag={}, ret={}",
                __FUNCTION__, offset, data_size, lock_flag, static_cast<long long>(r));
            goto __ret;
        }

        memcpy(p_buf, data_seq, data_size);

        _buf->Unlock();
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwVertexBuffer::LoadDataDynamic(const void* data_seq, DWORD data_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || ((_buf_info.usage & D3DUSAGE_DYNAMIC) == 0))
        goto __ret;

    if(data_seq == 0 || data_size == 0)
        goto __ret;

    if(data_size > _buf_info.size)
        goto __ret;
    {
        DWORD lock_size;
        DWORD lock_flag;
        D3DLOCK_TYPE* p_buf = 0;

        if ((_dlock_pos + data_size) > _buf_info.size)
        {
            lock_flag = D3DLOCK_DISCARD;
            _dlock_pos = 0;
            lock_size = 0;
        }
        else
        {
            lock_flag = D3DLOCK_NOOVERWRITE;
            lock_size = data_size;
        }

        if (LW_RESULT r = _buf->Lock(_dlock_pos, lock_size, &p_buf, lock_flag); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(VB LoadDataDynamic) failed: dlock_pos={}, lock_size={}, lock_flag={}, ret={}",
                __FUNCTION__, _dlock_pos, lock_size, lock_flag, static_cast<long long>(r));
            goto __ret;
        }

        memcpy(p_buf, data_seq, data_size);

        _buf->Unlock();

        _dlock_pos += data_size;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

//lwIndexBuffer
//LW_STD_IMPLEMENTATION(lwIndexBuffer)
LW_STD_GETINTERFACE(lwIndexBuffer);

LW_RESULT lwIndexBuffer::Release()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = Destroy(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] Destroy(IB) failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }
    {
        lwDeviceObject* devobj = reinterpret_cast<lwDeviceObject*>(_dev_obj);
        devobj->_ReleaseStreamBuffer(this);

        LW_DELETE(this);
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

lwIndexBuffer::lwIndexBuffer(lwIDeviceObject* dev_obj)
: _dev_obj(dev_obj), _buf(0), _next(0), _prev(0)
{
    _dlock_pos = 0;
    memset(&_buf_info, 0, sizeof(_buf_info));
}
lwIndexBuffer::~lwIndexBuffer()
{
}

LW_RESULT lwIndexBuffer::Create(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev_obj->CreateIndexBuffer(length, usage, format, pool, &_buf, handle); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->CreateIndexBuffer failed: length={}, usage={}, format={}, pool={}, ret={}",
            __FUNCTION__, length, usage, static_cast<std::uint32_t>(format), static_cast<std::uint32_t>(pool), static_cast<long long>(r));
        goto __ret;
    }

    _buf_info.size = length;
    _buf_info.pool = pool;
    _buf_info.usage = usage;
    _buf_info.format = format;
    _buf_info.stride = format == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwIndexBuffer::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0)
        goto __addre_ret_ok;

    if(LW_RESULT r = _dev_obj->ReleaseIndexBuffer(_buf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
            "[{}] _dev_obj->ReleaseIndexBuffer failed: ret={}",
            __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    _buf = 0;
    memset(&_buf_info, 0, sizeof(_buf_info));

__addre_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwIndexBuffer::LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0)
        goto __ret;

    if(data_seq == 0 || data_size == 0)
        goto __ret;

    if((offset + data_size) > _buf_info.size)
        goto __ret;
    {
        D3DLOCK_TYPE* p_buf = 0;

        if (LW_RESULT r = _buf->Lock(offset, (data_size == _buf_info.size) ? 0 : data_size, &p_buf, lock_flag); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(IB LoadData) failed: offset={}, data_size={}, lock_flag={}, ret={}",
                __FUNCTION__, offset, data_size, lock_flag, static_cast<long long>(r));
            goto __ret;
        }

        memcpy(p_buf, data_seq, data_size);

        _buf->Unlock();
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwIndexBuffer::LoadDataDynamic(const void* data_seq, DWORD data_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_buf == 0 || ((_buf_info.usage & D3DUSAGE_DYNAMIC) == 0))
        goto __ret;

    if(data_seq == 0 || data_size == 0)
        goto __ret;

    if(data_size > _buf_info.size)
        goto __ret;
    {
        DWORD lock_size;
        DWORD lock_flag;
        D3DLOCK_TYPE* p_buf = 0;

        if ((_dlock_pos + data_size) > _buf_info.size)
        {
            lock_flag = D3DLOCK_DISCARD;
            _dlock_pos = 0;
            lock_size = 0;
        }
        else
        {
            lock_flag = D3DLOCK_NOOVERWRITE;
            lock_size = data_size;
        }

        if (LW_RESULT r = _buf->Lock(_dlock_pos, lock_size, &p_buf, lock_flag); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                "[{}] _buf->Lock(IB LoadDataDynamic) failed: dlock_pos={}, lock_size={}, lock_flag={}, ret={}",
                __FUNCTION__, _dlock_pos, lock_size, lock_flag, static_cast<long long>(r));
            goto __ret;
        }

        memcpy(p_buf, data_seq, data_size);

        _buf->Unlock();

        _dlock_pos += data_size;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}


LW_END
