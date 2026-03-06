#include "StdAfx.h"
#include "SceneLight.h"

int SceneLight::UpdateAnimLight()
{
    if(anim_light)
    {
        anim_light->UpdateObject(this);
        return 1;
    }

    return 0;
}
// lwAnimKeySetPRS
template< class T >
HRESULT lwIndexDataSearch( DWORD* ret_min, DWORD* ret_max, DWORD key, T* data_seq, DWORD data_num )
{
    DWORD low = 0;
    DWORD high = data_num - 1;
    DWORD k, l;

    while( high >= low )
    {
        k = ( low + high ) / 2;

        l = k + 1;

        if( l == data_num )
        {
            //if( key == data_seq[ k ].key )
            //{
            //    *ret_min = k;
            //    *ret_max = k;
            //    break;
            //}
            //else
            //{
            //    assert( 0 );
            //}
            *ret_min = k;
            *ret_max = k;
            break;
        }
        else
        {
            if( key >= data_seq[ k ].id && key < data_seq[ l ].id )
            {
                *ret_min = k;
                *ret_max = l;
                break;
            }
        }

        if( key < data_seq[ k ].id )
        {
            high = k - 1;
        }
        else
        {
            low = k + 1;
        }
    }

    return ( low <= high ) ? LW_RET_OK : LW_INVALID_INDEX;
}

void AnimCtrlLight::SetData(IndexDataSceneLight* buf, DWORD num)
{ 
    SAFE_DELETE_ARRAY(_data_seq);
    _data_seq = MP_NEW(IndexDataSceneLight[num]);
    memcpy(_data_seq, buf, sizeof(IndexDataSceneLight) * num);
    _data_num = num;

    MPPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _data_seq[_data_num-1].id;

    SAFE_RELEASE(_pose_ctrl);
    MPGUIDCreateObject((LW_VOID**)&_pose_ctrl, LW_GUID_POSECTRL);
    _pose_ctrl->InsertPose(0, &pi, 1);

}

int AnimCtrlLight::Load(const char* file)
{
    FILE* fp = fopen(file, "rt");
    if(fp == NULL)
        return 0;

    SAFE_DELETE_ARRAY(_data_seq);

    fscanf(fp, "%d", &_data_num);

    _data_seq = MP_NEW(IndexDataSceneLight[_data_num]);

    //DWORD type;
    //D3DCOLORVALUE dif;
    //D3DCOLORVALUE amb;
    //D3DVECTOR pos;
    //float range;
    //float attenuation0;
    //float attenuation1;
    //float attenuation2;

    IndexDataSceneLight* p;


    for(DWORD i = 0; i < _data_num; i++)
    {
        p = &_data_seq[i];
        fscanf(fp, "%d, %d, %f %f %f, %f, %f %f %f\n", 
            &p->id, &p->light.type,
            &p->light.amb.r, &p->light.amb.g, &p->light.amb.b,
            &p->light.range,
            &p->light.attenuation0, &p->light.attenuation1, &p->light.attenuation2);

        p->light.pos.x = p->light.pos.y = p->light.pos.z = 0.0f;
        p->light.amb.r /= 255.0f;
        p->light.amb.g /= 255.0f;
        p->light.amb.b /= 255.0f;
        p->light.amb.a = 1.0f;
        p->light.dif = p->light.amb;
    }

    MPPoseInfo pi;
    memset(&pi, 0, sizeof(pi));
    pi.start = 0;
    pi.end = _data_seq[_data_num-1].id;

    SAFE_RELEASE(_pose_ctrl);
    MPGUIDCreateObject((LW_VOID**)&_pose_ctrl, LW_GUID_POSECTRL);
    _pose_ctrl->InsertPose(0, &pi, 1);

    fclose(fp);
    return 1;
}
int AnimCtrlLight::UpdateObject(SceneLight* ret_obj)
{
    if(_data_num == 0 )
        return 0;

    _pose_ctrl->PlayPose(&ret_obj->ppi);

    DWORD key[2];
    DWORD i = (DWORD)ret_obj->ppi.ret_frame;

    if(i >= _data_seq[_data_num-1].id)
        return 0;

    lwIndexDataSearch<IndexDataSceneLight>(&key[0], &key[1], i, _data_seq, _data_num);

    float t = (float)(i - _data_seq[key[0]].id) / (float)(_data_seq[key[1]].id - _data_seq[key[0]].id);

    SceneLight* u = &_data_seq[key[0]].light;
    SceneLight* v = &_data_seq[key[1]].light;


    ret_obj->type = u->type;
    MPVector3Slerp((MPVector3*)&ret_obj->pos, (MPVector3*)&u->pos, (MPVector3*)&v->pos, t);
    MPVector3Slerp((MPVector3*)&ret_obj->amb.r, (MPVector3*)&u->amb.r, (MPVector3*)&v->amb.r, t);
    MPVector3Slerp((MPVector3*)&ret_obj->dif.r, (MPVector3*)&u->dif.r, (MPVector3*)&v->dif.r, t);
    ret_obj->range = (1.0f - t) * u->range + t * v->range;
    ret_obj->attenuation0 = (1.0f - t) * u->attenuation0 + t * v->attenuation0;
    ret_obj->attenuation1 = (1.0f - t) * u->attenuation1 + t * v->attenuation1;
    ret_obj->attenuation2 = (1.0f - t) * u->attenuation2 + t * v->attenuation2;

    return 1;
}
