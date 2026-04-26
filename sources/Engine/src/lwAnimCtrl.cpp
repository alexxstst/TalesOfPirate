//
#include "stdafx.h"

#include "lwAnimCtrl.h"
#include "lwMath.h"
#include "lwPrimitive.h"
#include "lwResourceMgr.h"
#include "lwRenderImp.h"

//FILE* g_fp = 0;

LW_BEGIN

//LW_STD_IMPLEMENTATION(lwAnimCtrlBone)
//LW_STD_RELEASE(lwAnimCtrlBone)
LW_RESULT lwAnimCtrlBone::Release()
{
    LW_RESULT ret;

    ret = _res_mgr->UnregisterAnimCtrl(this);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] UnregisterAnimCtrl failed: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        goto __ret;
    }

    if(_reg_id == LW_INVALID_INDEX)
    {
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlBone::GetInterface(LW_VOID** i, lwGUID guid)
{ 
    LW_RESULT ret = LW_RET_FAILED;

    switch(guid)
    {
    case LW_GUID_ANIMDATABONE:
        (*i) = static_cast< lwIAnimDataBone* >(&_data);
        ret = LW_RET_OK;
        break;
    case LW_GUID_POSECTRL:
        (*i) = &_pose_ctrl;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

lwAnimCtrlBone::lwAnimCtrlBone(lwIResourceMgr* res_mgr)
: lwAnimCtrl(res_mgr),
  _bone_rtmat_seq(0), _dummy_rtmat_seq(0), _rtbuf_seq(0), _rtmat_ptr(0),
  _bone_rtmat_blend_seq(0)
{
    _ctrl_type = ANIM_CTRL_TYPE_BONE;
}

lwAnimCtrlBone::~lwAnimCtrlBone()
{
    Destroy();
}

LW_RESULT lwAnimCtrlBone::_BuildRunTimeBoneMatrix(lwMatrix44* out_buf, float frame, DWORD start_frame, DWORD end_frame)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD i;
    lwMatrix44* mat_run;
    DWORD parent_id;

#if(defined __DUMP_BRTBM_DATA)
    FILE* fp = fopen("xxx.txt", "wt");
    fprintf(fp, "%d\n", (int)frame);
#endif

    for(i = 0; i < _data._bone_num; i++)
    {
        mat_run = &out_buf[i];

        ret = _data.GetValue(mat_run, i, (float)frame, start_frame, end_frame);
        if(LW_FAILED(ret))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] AnimDataBone::GetValue failed: i={}, frame={}, start={}, end={}, ret={}",
                         __FUNCTION__, static_cast<long long>(i),
                         frame, static_cast<long long>(start_frame), static_cast<long long>(end_frame),
                         static_cast<long long>(ret));
            goto __ret;
        }

        lwMatrix44* m = mat_run;

#if(defined __DUMP_BRTBM_DATA)
        fprintf(fp, "%8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n",
            m->_11, m->_12, m->_13, m->_14,
            m->_21, m->_22, m->_23, m->_24,
            m->_31, m->_32, m->_33, m->_34,
            m->_41, m->_42, m->_43, m->_44);
#endif

        parent_id = _data._base_seq[i].parent_id;

        if(parent_id == LW_INVALID_INDEX)
        {
        }
        else
        {
            lwMatrix44Multiply(mat_run, mat_run, &out_buf[parent_id]);
        }      

    }

#if(defined __DUMP_BRTBM_DATA)
    fclose(fp);
#endif

    for(i = 0; i < _data._bone_num; i++)
    {
        lwMatrix44Multiply(&out_buf[i], &_data._invmat_seq[i], &out_buf[i]);
    }


    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwAnimCtrlBone::_UpdateFrameDataBone(lwMatrix44** o_mat_ptr, lwMatrix44* mat_buf, float frame, DWORD start_frame, DWORD end_frame, BOOL loop_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD i;
    DWORD f;
    DWORD min_f, max_f;
    float t;
    lwMatrix44* this_mat_ptr = 0;
    lwRTBD* rtbd_f;
    lwRTBD* rtbd_min;
    lwRTBD* rtbd_max;


    // by lsh
    // frame > _data._frame_num ">="PLAY_LOOP
    // 0_data._frame_num - 1
    if(frame < 0.0f || frame > _data._frame_num)
    {
        ret = ERR_INVALID_PARAM;
        goto __ret;
    }
    
    // check run time buffer enable state
    if(_rtbuf_seq)
    {
        t = lwFloatDecimal(frame);

        if(lwFloatZero(t, 1e-3f))
        {
            f = (DWORD)frame;

            if(f >= _data._frame_num)
                goto __ret;

            rtbd_f = &_rtbuf_seq[f];

            if(rtbd_f->flag == 0)
                goto __rt_get_value;

            if(rtbd_f->data == 0)
            {
                rtbd_f->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_f->data, frame, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_f) failed: frame={}, start={}, end={}, ret={}",
                                 __FUNCTION__, frame, static_cast<long long>(start_frame),
                                 static_cast<long long>(end_frame), static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_f->data);
                    goto __ret;
                }
            }


            //        fprintf(g_fp, "%8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f\n",
            //            rtbd_f->data[i]._11, rtbd_f->data[i]._12, rtbd_f->data[i]._13, rtbd_f->data[i]._14,
            //            rtbd_f->data[i]._21, rtbd_f->data[i]._22, rtbd_f->data[i]._23, rtbd_f->data[i]._24,
            //            rtbd_f->data[i]._31, rtbd_f->data[i]._32, rtbd_f->data[i]._33, rtbd_f->data[i]._34,


            this_mat_ptr = rtbd_f->data;
        }
        else
        {

            min_f = lwFloatRoundDec(frame);
            max_f = min_f + 1;

            // mirror the frame of end_frame to start_frame
            // warning: the code was not tested 
            if(loop_flag && (max_f >= end_frame))
            {
                max_f = start_frame;
            }

            rtbd_min = &_rtbuf_seq[min_f];
            rtbd_max = &_rtbuf_seq[max_f];

            if(rtbd_min->flag == 0 || rtbd_max->flag == 0)
                goto __rt_get_value;

            // buf_min
            if(rtbd_min->data == 0)
            {
                rtbd_min->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_min->data, min_f, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_min) failed: min_f={}, start={}, end={}, ret={}",
                                 __FUNCTION__, static_cast<long long>(min_f),
                                 static_cast<long long>(start_frame), static_cast<long long>(end_frame),
                                 static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_min->data);
                    goto __ret;
                }
            }
            // buf_max
            if(rtbd_max->data == 0)
            {
                rtbd_max->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_max->data, max_f, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_max) failed: max_f={}, start={}, end={}, ret={}",
                                 __FUNCTION__, static_cast<long long>(max_f),
                                 static_cast<long long>(start_frame), static_cast<long long>(end_frame),
                                 static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_max->data);
                    goto __ret;
                }
            }


            for(i = 0; i < _data._bone_num; i++)
            {
                lwMat44Slerp(&mat_buf[i], &rtbd_min->data[i], &rtbd_max->data[i], t);

                //    fprintf(g_fp, "%8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f\n",
                //        mat_buf[i]._11, mat_buf[i]._12, mat_buf[i]._13, mat_buf[i]._14,
                //        mat_buf[i]._21, mat_buf[i]._22, mat_buf[i]._23, mat_buf[i]._24,
                //        mat_buf[i]._31, mat_buf[i]._32, mat_buf[i]._33, mat_buf[i]._34,
            }


            this_mat_ptr = mat_buf;
        }

    }
    else
    {
__rt_get_value:

        if(LW_RESULT r = _BuildRunTimeBoneMatrix(mat_buf, frame, start_frame, end_frame); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] _BuildRunTimeBoneMatrix(mat_buf) failed: frame={}, start={}, end={}, ret={}",
                         __FUNCTION__, frame, static_cast<long long>(start_frame),
                         static_cast<long long>(end_frame), static_cast<long long>(r));
            goto __ret;
        }

        this_mat_ptr = mat_buf;
    }


    *o_mat_ptr = this_mat_ptr;

    ret = LW_RET_OK;

__ret:
    return ret;

}


LW_RESULT lwAnimCtrlBone::_UpdateFrameDataBoneDummy()
{
    lwBoneDummyInfo* dummy_info;

    for(DWORD i = 0; i < _data._dummy_num; i++)
    {
        dummy_info = &_data._dummy_seq[i];
        if(dummy_info->parent_bone_id != LW_INVALID_INDEX)
        {
            lwMatrix44Multiply(&_dummy_rtmat_seq[i].mat, &dummy_info->mat, &_rtmat_ptr[dummy_info->parent_bone_id]);
        }
    }

    return LW_RET_OK;

}

LW_RESULT lwAnimCtrlBone::_BlendBoneData(lwMatrix44* dst_mat_ptr, lwMatrix44* src_mat_ptr0, lwMatrix44* src_mat_ptr1, float t)
{
    for(DWORD i = 0; i < _data._bone_num; i++)
    {
        lwMat44Slerp(&dst_mat_ptr[i], &src_mat_ptr0[i], &src_mat_ptr1[i], t);
    }

    return LW_RET_OK;
}

lwMatrix44* lwAnimCtrlBone::GetDummyRTM(DWORD id)
{
    for(DWORD i = 0; i < _data._dummy_num; i++)
    {
        if(_dummy_rtmat_seq[i].id == id)
            return &_dummy_rtmat_seq[i].mat;
    }

    return NULL;
}

LW_RESULT lwAnimCtrlBone::SetFrame(float frame, DWORD start_frame, DWORD end_frame)
{
    LW_RESULT ret = LW_RET_FAILED;

    // by lsh
    // frame > _data._frame_num ">="PLAY_LOOP
    // 0_data._frame_num - 1
    if(frame < 0.0f || frame > _data._frame_num)
    {
        ret = ERR_INVALID_PARAM;
        goto __ret;
    }
    
    // check run time buffer enable state
    if(_rtbuf_seq)
    {
        float t = lwFloatDecimal(frame);

        if(lwFloatZero(t, 1e-3f))
        {
            int f = (int)frame;
            lwRTBD* rtbd_f = &_rtbuf_seq[f];

            if(rtbd_f->flag == 0)
                goto __rt_get_value;

            if(rtbd_f->data == 0)
            {
                rtbd_f->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_f->data, frame, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_f) failed: frame={}, start={}, end={}, ret={}",
                                 __FUNCTION__, frame, static_cast<long long>(start_frame),
                                 static_cast<long long>(end_frame), static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_f->data);
                    goto __ret;
                }
            }

            _rtmat_ptr = rtbd_f->data;
        }
        else
        {
            int min_f = lwFloatRoundDec(frame);
            int max_f = min_f + 1;

            lwRTBD* rtbd_min = &_rtbuf_seq[min_f];
            lwRTBD* rtbd_max = &_rtbuf_seq[max_f];

            if(rtbd_min->flag == 0 || rtbd_max->flag == 0)
                goto __rt_get_value;

            // buf_min
            if(rtbd_min->data == 0)
            {
                rtbd_min->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_min->data, frame, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_min) failed: frame={}, start={}, end={}, ret={}",
                                 __FUNCTION__, frame, static_cast<long long>(start_frame),
                                 static_cast<long long>(end_frame), static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_min->data);
                    goto __ret;
                }
            }
            // buf_max
            if(rtbd_max->data == 0)
            {
                rtbd_max->data = LW_NEW(lwMatrix44[_data._bone_num]);

                if(LW_RESULT r = _BuildRunTimeBoneMatrix(rtbd_max->data, frame, start_frame, end_frame); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] _BuildRunTimeBoneMatrix(rtbd_max) failed: frame={}, start={}, end={}, ret={}",
                                 __FUNCTION__, frame, static_cast<long long>(start_frame),
                                 static_cast<long long>(end_frame), static_cast<long long>(r));
                    LW_SAFE_DELETE_A(rtbd_max->data);
                    goto __ret;
                }
            }

            for(DWORD i = 0; i < _data._bone_num; i++)
            {
                lwMat44Slerp(&_bone_rtmat_seq[i], &rtbd_min->data[i], &rtbd_max->data[i], t);
            }

            _rtmat_ptr = _bone_rtmat_seq;
        }

        goto __addr_set_rtdummy;
    }
    else
    {
__rt_get_value:

#if 1
        if(LW_RESULT r = _BuildRunTimeBoneMatrix(_bone_rtmat_seq, frame, start_frame, end_frame); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] _BuildRunTimeBoneMatrix(_bone_rtmat_seq) failed: frame={}, start={}, end={}, ret={}",
                         __FUNCTION__, frame, static_cast<long long>(start_frame),
                         static_cast<long long>(end_frame), static_cast<long long>(r));
            goto __ret;
        }

#else  // _BuildRunTimeBoneMatrix
        DWORD i;
        lwMatrix44* mat_run;
        DWORD parent_id;

        for(i = 0; i < _data._bone_num; i++)
        {
        mat_run = &_bone_rtmat_seq[i];

        if(LW_FAILED(ret = _data.GetValue(mat_run, i, (float)frame)))
        goto __ret;


        parent_id = _data._base_seq[i].parent_id;

        if(parent_id == LW_INVALID_INDEX)
        {
        }
        else
        {
        lwMatrix44Multiply(mat_run, mat_run, &_bone_rtmat_seq[parent_id]);
        }        
        }

        for(i = 0; i < _data._bone_num; i++)
        {
        lwMatrix44Multiply(&_bone_rtmat_seq[i], &_data._invmat_seq[i], &_bone_rtmat_seq[i]);
        }
#endif

        _rtmat_ptr = _bone_rtmat_seq;
    }

__addr_set_rtdummy:

    if(LW_RESULT r = _UpdateFrameDataBoneDummy(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _UpdateFrameDataBoneDummy failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }


    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwAnimCtrlBone::LoadData(const void* data)
{
    LW_RESULT ret = LW_RET_FAILED;
	lwPoseInfo pi{};

    const lwAnimDataBone* data_bone = (lwAnimDataBone*)data;

    if(LW_RESULT r = _data.Copy(data_bone); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] AnimDataBone::Copy failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    _bone_rtmat_seq = LW_NEW(lwMatrix44[_data._bone_num]);
    _dummy_rtmat_seq = LW_NEW(lwIndexMatrix44[_data._dummy_num]);

    for(DWORD i = 0; i < _data._dummy_num; i++)
    {
        _dummy_rtmat_seq[i].id = _data._dummy_seq[i].id;
    }

    pi.end = _data._frame_num - 1;
    _pose_ctrl.SetFrameNum(_data._frame_num);
    _pose_ctrl.InsertPose(0, &pi, 1);
    
    ret = LW_RET_OK;

__ret:

    return ret;
}

LW_RESULT lwAnimCtrlBone::ExtractAnimData(lwIAnimDataBone* out_data)
{
    lwAnimDataBone* a = (lwAnimDataBone*)out_data;
    return a->Copy(&_data);
}

LW_RESULT lwAnimCtrlBone::DumpRunTimeBoneData(const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    if (_rtbuf_seq == 0)
        goto __ret;
    
    {
        FILE* fp = fopen(file, "wt");
        if (fp == NULL)
            goto __ret;
        {
            lwMatrix44* m;

            for (DWORD i = 0; i < _data._frame_num; i++)
            {
                if (_rtbuf_seq[i].data)
                {
                    fprintf(fp, "\n\n%d\n", i);

                    for (DWORD j = 0; j < _data._bone_num; j++)
                    {
                        m = &_rtbuf_seq[i].data[j];

                        fprintf(fp, "%6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f\n",
                            m->_11, m->_12, m->_13, m->_14,
                            m->_21, m->_22, m->_23, m->_24,
                            m->_31, m->_32, m->_33, m->_34,
                            m->_41, m->_42, m->_43, m->_44);
                    }
                }
            }

            fclose(fp);

            ret = LW_RET_OK;
        }
    }
__ret:
	return ret;
}

LW_RESULT lwAnimCtrlBone::DumpInitInvMat(const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_rtbuf_seq == 0)
        goto __ret;

    {
        FILE* fp = fopen(file, "wt");
        if (fp == NULL)
            goto __ret;

        {
            lwMatrix44* m;

            for (DWORD i = 0; i < _data._bone_num; i++)
            {
                fprintf(fp, "\n\n%d\n", i);

                m = &_data._invmat_seq[i];

                fprintf(fp, "%6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f\n",
                    m->_11, m->_12, m->_13, m->_14,
                    m->_21, m->_22, m->_23, m->_24,
                    m->_31, m->_32, m->_33, m->_34,
                    m->_41, m->_42, m->_43, m->_44);
            }

            fclose(fp);

            ret = LW_RET_OK;
        }
    }
__ret:
    return ret;
}


LW_RESULT lwAnimCtrlBone::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    LW_SAFE_DELETE_A(_bone_rtmat_seq);
    LW_SAFE_DELETE_A(_bone_rtmat_blend_seq);
    LW_SAFE_DELETE_A(_dummy_rtmat_seq);

    if(_rtbuf_seq)
    {
        for(DWORD i = 0; i < _data._frame_num; i++)
        {
            LW_IF_DELETE_A(_rtbuf_seq[i].data);
        }

        LW_DELETE_A(_rtbuf_seq);
        _rtbuf_seq = 0;
    }

    if(LW_RESULT r = _data.Destroy(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] AnimDataBone::Destroy failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlBone::CreateRunTimeDataBuffer()
{
    if(_rtbuf_seq)
        return LW_RET_FAILED;

    _rtbuf_seq = LW_NEW(lwRTBD[_data._frame_num]);
    memset(_rtbuf_seq, 0, sizeof(lwRTBD) * _data._frame_num);

    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlBone::EnableRunTimeFrameBuffer(DWORD frame, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_rtbuf_seq == 0)
        goto __ret;

    if(frame >= _data._frame_num)
        goto __ret;

    _rtbuf_seq[frame].flag = flag;

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwAnimCtrlBone::UpdatePose(lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _pose_ctrl.PlayPose(info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::PlayPose failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(r));
        goto __ret;
    }



__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlBone::UpdateAnimData(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwPoseInfo* pi = _pose_ctrl.GetPoseInfo(info->pose);

    if(LW_RESULT r = _UpdateFrameDataBone(&_rtmat_ptr, _bone_rtmat_seq, info->ret_frame, pi->start, pi->end, info->type == PLAY_LOOP ? 1 : 0); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _UpdateFrameDataBone(main) failed: pose={}, frame={}, start={}, end={}, ret={}",
                     __FUNCTION__, static_cast<long long>(info->pose),
                     info->ret_frame, static_cast<long long>(pi->start),
                     static_cast<long long>(pi->end), static_cast<long long>(r));
        goto __ret;
    }

    if(info->blend_info.op_state)
    {
        lwMatrix44* rtmat_blend_ptr = 0;

        if(_bone_rtmat_blend_seq == 0)
        {
            _bone_rtmat_blend_seq = LW_NEW(lwMatrix44[_data._bone_num]);
        }

        pi = _pose_ctrl.GetPoseInfo(info->blend_info.blend_pose);

        if(LW_RESULT r = _UpdateFrameDataBone(&rtmat_blend_ptr, _bone_rtmat_blend_seq, info->blend_info.blend_ret_frame, pi->start, pi->end, info->blend_info.blend_type == PLAY_LOOP ? 1 : 0); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] _UpdateFrameDataBone(blend) failed: blend_pose={}, blend_frame={}, ret={}",
                         __FUNCTION__, static_cast<long long>(info->blend_info.blend_pose),
                         info->blend_info.blend_ret_frame, static_cast<long long>(r));
            goto __ret;
        }

        if(LW_RESULT r = _BlendBoneData(_bone_rtmat_seq, rtmat_blend_ptr, _rtmat_ptr, info->blend_info.weight); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] _BlendBoneData failed: weight={}, ret={}",
                         __FUNCTION__, info->blend_info.weight, static_cast<long long>(r));
            goto __ret;
        }

        _rtmat_ptr = _bone_rtmat_seq;
    }

    if(LW_RESULT r = _UpdateFrameDataBoneDummy(); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] _UpdateFrameDataBoneDummy failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlBone::UpdatePoseKeyFrameProc(lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    // frame type cannot invoke CallBack proc
    if(info->type == PLAY_FRAME)
        goto __addr_ret_ok;

    if(LW_RESULT r = _pose_ctrl.CallBack(info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::CallBack failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(r));
        goto __ret;
    }

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
// lwAnimCtrlMatrix
//LW_STD_RELEASE(lwAnimCtrlMatrix)
LW_RESULT lwAnimCtrlMatrix::Release()
{
    LW_RESULT ret;

    ret = _res_mgr->UnregisterAnimCtrl(this);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] UnregisterAnimCtrl failed: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        goto __ret;
    }

    if(_reg_id == LW_INVALID_INDEX)
    {
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlMatrix::GetInterface(LW_VOID** i, lwGUID guid)
{ 
    LW_RESULT ret = LW_RET_FAILED;

    switch(guid)
    {
    case LW_GUID_ANIMDATAMATRIX:
        (*i) = static_cast< lwIAnimDataMatrix* >(this);
        ret = LW_RET_OK;
        break;
    case LW_GUID_POSECTRL:
        (*i) = &_pose_ctrl;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

lwAnimCtrlMatrix::lwAnimCtrlMatrix(lwIResourceMgr* res_mgr)
: lwAnimCtrl(res_mgr)
{
}

lwAnimCtrlMatrix::~lwAnimCtrlMatrix()
{
}

LW_RESULT lwAnimCtrlMatrix::LoadData(const void* data)
{
    lwAnimDataMatrix* data_mat = (lwAnimDataMatrix*)data;

#ifdef USE_ANIMKEY_PRS
    lwCopyAnimKeySetPRS(&_data, data_mat);
#else
    Copy(data_mat);
#endif

    // insert default pose 0
    lwPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _frame_num - 1;
    _pose_ctrl.SetFrameNum(_frame_num);
    _pose_ctrl.InsertPose(0, &pi, 1);

    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlMatrix::ExtractAnimData(lwIAnimDataMatrix* out_data)
{
    lwAnimDataMatrix* a = (lwAnimDataMatrix*)out_data;

#ifdef USE_ANIMKEY_PRS
    return lwCopyAnimKeySetPRS(a, this);
#else
    return a->Copy((const lwAnimDataMatrix*)this);
#endif

}

LW_RESULT lwAnimCtrlMatrix::SetFrame(float frame)
{

    if(frame >= _frame_num)
        return ERR_INVALID_PARAM;

    return GetValue(&_rtmat_seq[0], frame);
}

LW_RESULT lwAnimCtrlMatrix::SetFrame(DWORD frame)
{
    if(frame >= _frame_num)
        return ERR_INVALID_PARAM;

    return GetValue(&_rtmat_seq[0], (float)frame);

}

LW_RESULT lwAnimCtrlMatrix::UpdatePose(lwPlayPoseInfo* info)
{
    LW_RESULT ret;

    ret = _pose_ctrl.PlayPose(info);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::PlayPose failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(ret));
        return ret;
    }

    ret = SetFrame(info->ret_frame);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] SetFrame failed: frame={}, ret={}",
                     __FUNCTION__, info->ret_frame, static_cast<long long>(ret));
        return ret;
    }

    return _pose_ctrl.CallBack(info);
}
LW_RESULT lwAnimCtrlMatrix::UpdateAnimData(const lwPlayPoseInfo* info)
{
    return SetFrame(info->ret_frame);
}

LW_RESULT lwAnimCtrlMatrix::GetRTM(lwMatrix44* mat)
{
    *mat = _rtmat_seq[0];
    return LW_RET_OK;
}

// lwAnimCtrlTexUV
//LW_STD_RELEASE(lwAnimCtrlTexUV)
LW_RESULT lwAnimCtrlTexUV::Release()
{
    LW_RESULT ret;

    ret = _res_mgr->UnregisterAnimCtrl(this);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] UnregisterAnimCtrl failed: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        goto __ret;
    }

    if(_reg_id == LW_INVALID_INDEX)
    {
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlTexUV::GetInterface(LW_VOID** i, lwGUID guid)
{ 
    LW_RESULT ret = LW_RET_FAILED;

    switch(guid)
    {
    case LW_GUID_ANIMDATATEXUV:
        (*i) = static_cast< lwIAnimDataTexUV* >(this);
        ret = LW_RET_OK;
        break;
    case LW_GUID_POSECTRL:
        (*i) = &_pose_ctrl;
        ret = LW_RET_OK;
        break;
    }

    return ret;
}

lwAnimCtrlTexUV::lwAnimCtrlTexUV(lwIResourceMgr* res_mgr)
: lwAnimCtrl(res_mgr), _keyset_prs(0)
{
}

lwAnimCtrlTexUV::~lwAnimCtrlTexUV()
{
    LW_IF_RELEASE(_keyset_prs);
}

LW_RESULT lwAnimCtrlTexUV::LoadData(const void* data)
{
    Copy((lwAnimDataTexUV*)data);

    // insert default pose 0
    lwPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _frame_num - 1;
    _pose_ctrl.SetFrameNum(_frame_num);
    _pose_ctrl.InsertPose(0, &pi, 1);

    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlTexUV::ExtractAnimData(lwIAnimDataTexUV* out_data)
{
    lwAnimDataTexUV* a = (lwAnimDataTexUV*)out_data;
    return a->Copy((const lwAnimDataTexUV*)this);
}

LW_RESULT lwAnimCtrlTexUV::LoadData(const lwMatrix44* mat_seq, DWORD mat_num)
{
    LW_SAFE_DELETE_A(_mat_seq);
    _frame_num = mat_num;
    _mat_seq = LW_NEW(lwMatrix44[_frame_num]);
    memcpy(_mat_seq, mat_seq, sizeof(lwMatrix44) * _frame_num);

    // insert default pose 0
    _pose_ctrl.RemoveAll();
    lwPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _frame_num - 1;
    _pose_ctrl.SetFrameNum(_frame_num);
    _pose_ctrl.InsertPose(0, &pi, 1);

    return LW_RET_OK;
}

void lwAnimCtrlTexUV::SetAnimKeySetPRS(lwIAnimKeySetPRS* keyset)
{
    _keyset_prs = keyset;

    // insert default pose 0
    _pose_ctrl.RemoveAll();
    lwPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _keyset_prs->GetFrameNum() - 1;
    _pose_ctrl.SetFrameNum(pi.end + 1);
    _pose_ctrl.InsertPose(0, &pi, 1);

}

LW_RESULT lwAnimCtrlTexUV::SetFrame(float frame)
{

    LW_RESULT ret = LW_RET_FAILED;

    if(_keyset_prs)
    {
        if(frame >= _keyset_prs->GetFrameNum())
            goto __ret;

        if(LW_RESULT r = _keyset_prs->GetValue(&_rtmat_seq[0], frame); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] keyset_prs::GetValue failed: frame={}, ret={}",
                         __FUNCTION__, frame, static_cast<long long>(r));
            goto __ret;
        }

        // because uv translation only use m._31 and m._32
        lwMatrix44* mat = &_rtmat_seq[0];
        mat->_31 = mat->_41;
        mat->_32 = mat->_42;
        mat->_41 = mat->_42 = 0.0f;
    }
    else
    {
        if(frame >= _frame_num)
            goto __ret;

        if(LW_RESULT r = GetValue(&_rtmat_seq[0], frame); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] GetValue failed: frame={}, frame_num={}, ret={}",
                         __FUNCTION__, frame, static_cast<long long>(_frame_num),
                         static_cast<long long>(r));
            goto __ret;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlTexUV::SetFrame(DWORD frame)
{
    return SetFrame((float)frame);
}

LW_RESULT lwAnimCtrlTexUV::UpdatePose(lwPlayPoseInfo* info)
{
    LW_RESULT ret;

    ret = _pose_ctrl.PlayPose(info);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::PlayPose failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(ret));
        return ret;
    }

    return _pose_ctrl.CallBack(info);
}
LW_RESULT lwAnimCtrlTexUV::UpdateAnimData(const lwPlayPoseInfo* info)
{
    return SetFrame(info->ret_frame);
}
LW_RESULT lwAnimCtrlTexUV::GetRunTimeMatrix(lwMatrix44* mat)
{
    *mat = _rtmat_seq[0];
    return LW_RET_OK;
}

//LW_STD_IMPLEMENTATION(lwAnimCtrlTexImg)
LW_STD_GETINTERFACE(lwAnimCtrlTexImg)
LW_RESULT lwAnimCtrlTexImg::Release()
{
    LW_RESULT ret;

    ret = _res_mgr->UnregisterAnimCtrl(this);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] UnregisterAnimCtrl failed: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        goto __ret;
    }

    if(_reg_id == LW_INVALID_INDEX)
    {
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwAnimCtrlTexImg::lwAnimCtrlTexImg(lwIResourceMgr* res_mgr)
: lwAnimCtrl(res_mgr),
  _tex_seq(0), _tex_num(0), _rttex(LW_INVALID_INDEX)
{
}
lwAnimCtrlTexImg::~lwAnimCtrlTexImg()
{
    Destroy();
}
LW_RESULT lwAnimCtrlTexImg::Destroy()
{
    if(_tex_num > 0)
    {
        for(DWORD i = 0; i < _tex_num; i++)
        {
            if(_tex_seq[i] == 0)
                continue;

            _tex_seq[i]->Release();
        }
        LW_SAFE_DELETE_A(_tex_seq);
        _tex_num = 0;
    }
    return LW_RET_OK;
}
LW_RESULT lwAnimCtrlTexImg::LoadData(const void* data)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimDataTexImg* adti = (lwAnimDataTexImg*)data;
	lwPoseInfo pi{};

    if(LW_RESULT r = _data.Copy(adti); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] AnimDataTexImg::Copy failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    _tex_num = adti->_data_num;
    _tex_seq = LW_NEW(lwITex*[_tex_num]);
    memset(_tex_seq, 0, sizeof(lwITex*) * _tex_num);

    lwITex* tex;

    for(DWORD i = 0; i < _tex_num; i++)
    {
        if(LW_RESULT r = _res_mgr->CreateTex(&tex); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateTex failed: i={}, ret={}",
                         __FUNCTION__, static_cast<long long>(i), static_cast<long long>(r));
            goto __ret;
        }

        if(LW_RESULT r = tex->LoadTexInfo(&adti->_data_seq[i], adti->_tex_path); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LoadTexInfo failed: i={}, tex_path='{}', ret={}",
                         __FUNCTION__, static_cast<long long>(i),
                         adti->_tex_path[0] ? adti->_tex_path : "(empty)",
                         static_cast<long long>(r));
            goto __ret;
        }

        tex->SetLoadResType(LOADINGRES_TYPE_RUNTIME);
        _tex_seq[i] = tex;
        tex = 0;
    }

    // insert default pose 0
    _pose_ctrl.RemoveAll();
    pi.end = _tex_num - 1;
    _pose_ctrl.SetFrameNum(_tex_num);
    _pose_ctrl.InsertPose(0, &pi, 1);

    ret = LW_RET_OK;

__ret:
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] LoadData failed, cleaning up: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        LW_IF_RELEASE(tex);
        Destroy();
    }

    return ret;
}

LW_RESULT lwAnimCtrlTexImg::ExtractAnimData(lwIAnimDataTexImg* out_data)
{
    lwAnimDataTexImg* a = (lwAnimDataTexImg*)out_data;
    return a->Copy(&_data);
}

LW_RESULT lwAnimCtrlTexImg::UpdatePose(lwPlayPoseInfo* info)
{
    LW_RESULT ret;

    ret = _pose_ctrl.PlayPose(info);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::PlayPose failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(ret));
        return ret;
    }

    return _pose_ctrl.CallBack(info);
}
LW_RESULT lwAnimCtrlTexImg::UpdateAnimData(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD f = (DWORD)info->ret_frame;

    if(f >= _tex_num)
        goto __ret;

    if(_tex_seq[f] == NULL)
        goto __ret;

    _rttex = f;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlTexImg::GetRunTimeTex(lwITex** tex)
{
    *tex = _tex_seq[_rttex];
    return LW_RET_OK;
}

// lwAnimCtrlMtlOpacity
//LW_STD_IMPLEMENTATION(lwAnimCtrlMtlOpacity)
LW_STD_GETINTERFACE(lwAnimCtrlMtlOpacity)
LW_RESULT lwAnimCtrlMtlOpacity::Release()
{
    LW_RESULT ret;

    ret = _res_mgr->UnregisterAnimCtrl(this);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] UnregisterAnimCtrl failed: ret={}",
                     __FUNCTION__, static_cast<long long>(ret));
        goto __ret;
    }

    if(_reg_id == LW_INVALID_INDEX)
    {
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwAnimCtrlMtlOpacity::lwAnimCtrlMtlOpacity(lwIResourceMgr* res_mgr)
: lwAnimCtrl(res_mgr), _data(0), _rt_opacity(0.0f)
{
}

lwAnimCtrlMtlOpacity::~lwAnimCtrlMtlOpacity()
{
    Destroy();
}
LW_RESULT lwAnimCtrlMtlOpacity::Destroy()
{
    LW_SAFE_RELEASE(_data);
    return LW_RET_OK;
}
LW_RESULT lwAnimCtrlMtlOpacity::LoadData(const void* data)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = ((lwIAnimDataMtlOpacity*)data)->Clone(&_data); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] AnimDataMtlOpacity::Clone failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    {
        lwIAnimKeySetFloat* aks = _data->GetAnimKeySet();

        // insert default pose 0
        _pose_ctrl.RemoveAll();
        lwPoseInfo pi;
        memset(&pi, 0, sizeof(pi));
        pi.start = aks->GetBeginKey();
        pi.end = aks->GetEndKey();

        if (pi.start >= pi.end)
            goto __ret;

        _pose_ctrl.SetFrameNum(pi.end + 1);
        _pose_ctrl.InsertPose(0, &pi, 1);

        ret = LW_RET_OK;
    }
__ret:

    return ret;
}

LW_RESULT lwAnimCtrlMtlOpacity::ExtractAnimData(lwIAnimDataMtlOpacity* out_data)
{
    LW_RESULT ret;

    lwIAnimKeySetFloat* aksf;
    
    lwIAnimKeySetFloat* this_aksf = _data->GetAnimKeySet();

    if(this_aksf == 0)
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] GetAnimKeySet returned null",
                     __FUNCTION__);
        goto __ret;
    }
    if(LW_RESULT r = this_aksf->Clone(&aksf); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] AnimKeySetFloat::Clone failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
        goto __ret;
    }

    out_data->SetAnimKeySet(aksf);

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlMtlOpacity::UpdatePose(lwPlayPoseInfo* info)
{
    LW_RESULT ret;

    ret = _pose_ctrl.PlayPose(info);
    if(LW_FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] PoseCtrl::PlayPose failed: pose={}, ret={}",
                     __FUNCTION__, info ? static_cast<long long>(info->pose) : -1LL,
                     static_cast<long long>(ret));
        return ret;
    }

    return _pose_ctrl.CallBack(info);
}
LW_RESULT lwAnimCtrlMtlOpacity::UpdateAnimData(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_data == 0)
        goto __ret;

    {
        lwIAnimKeySetFloat* aksf = _data->GetAnimKeySet();
        if (aksf == 0)
            goto __ret;

        if (LW_RESULT r = aksf->GetValue(&_rt_opacity, info->ret_frame); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] AnimKeySetFloat::GetValue failed: frame={}, ret={}",
                         __FUNCTION__, info->ret_frame, static_cast<long long>(r));
            goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlMtlOpacity::GetRunTimeOpacity(float* opacity)
{
    *opacity = _rt_opacity;
    return LW_RET_OK;
}

LW_END
