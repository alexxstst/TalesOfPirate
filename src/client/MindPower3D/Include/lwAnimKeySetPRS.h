//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwMath.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwITypes2.h"
#include "lwInterfaceExt.h"

LW_BEGIN

class lwAnimKeySetPRS2 : public lwIAnimKeySetPRS
{
    LW_STD_DECLARATION()
private:
    lwKeyVector3* _pos_seq;
    lwKeyQuaternion* _rot_seq;
    lwKeyVector3* _sca_seq;
    DWORD _pos_num;
    DWORD _rot_num;
    DWORD _sca_num;
    DWORD _frame_num;

private:
    void _ResetFrameNum();

public:
    lwAnimKeySetPRS2()
        : _pos_seq(0), _rot_seq(0), _sca_seq(0), _pos_num(0), _rot_num(0), _sca_num(0), _frame_num(0)
    {}

    ~lwAnimKeySetPRS2()
    {
        LW_IF_DELETE_A(_pos_seq);
        LW_IF_DELETE_A(_rot_seq);
        LW_IF_DELETE_A(_sca_seq);
    }


    LW_RESULT Destroy();
    LW_RESULT GetValue(lwMatrix44* mat, float frame);
    LW_RESULT AddKeyPosition(DWORD id, const lwKeyVector3* data, DWORD num);
    LW_RESULT AddKeyRotation(DWORD id, const lwKeyQuaternion* data, DWORD num);
    LW_RESULT AddKeyScale(DWORD id, const lwKeyVector3* data, DWORD num);
    LW_RESULT DelKeyPosition(DWORD id, DWORD num);// if num == 0xffffffff(-1) then delete sequence
    LW_RESULT DelKeyRotation(DWORD id, DWORD num);
    LW_RESULT DelKeyScale(DWORD id, DWORD num);
    DWORD GetFrameNum() const { return _frame_num; }
    DWORD GetKeyPositionNum() const { return _pos_num; }
    DWORD GetKeyRotationNum() const { return _rot_num; }
    DWORD GetKeyScaleNum() const { return _sca_num; }
};

class lwAnimKeySetFloat : public lwIAnimKeySetFloat
{
    typedef lwKeyFloat KEY_TYPE;

    LW_STD_DECLARATION();

private:
    lwKeyFloat* _data_seq;
    DWORD _data_num;
    DWORD _data_capacity;

public:
    lwAnimKeySetFloat();
    ~lwAnimKeySetFloat();

    LW_RESULT Allocate(DWORD key_capacity);
    LW_RESULT Clear();
    LW_RESULT Clone(lwIAnimKeySetFloat** obj);
    LW_RESULT SetKeyData(DWORD key, float data, DWORD slerp_type, DWORD mask);
    LW_RESULT GetKeyData(DWORD key, float* data, DWORD* slerp_type);
    LW_RESULT InsertKey(DWORD key, float data, DWORD slerp_type);
    LW_RESULT RemoveKey(DWORD key);
    LW_RESULT GetValue(float* data, float key);
    LW_RESULT SetKeySequence(const lwKeyFloat* seq, DWORD num);
    lwKeyFloat* GetKeySequence() { return _data_seq; }
    DWORD GetKeyNum() { return _data_num; }
    DWORD GetKeyCapacity() { return _data_capacity; }
    DWORD GetBeginKey() { return _data_num > 0 ? _data_seq[0].key : 0; }
    DWORD GetEndKey() { return _data_num > 0 ? _data_seq[_data_num - 1].key : 0; }
};

LW_END
