//
#include "stdafx.h"

#include "lwModel.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwResourceMgr.h"
#include "lwAnimCtrl.h"
#include "lwRenderImp.h"
#include "lwPathInfo.h"
#include "lwPreDefinition.h"
#include <fstream>
#include <iostream>
LW_BEGIN


//lwModel
LW_STD_IMPLEMENTATION(lwModel)

lwModel::lwModel(lwIResourceMgr* res_mgr)
: _id(LW_INVALID_INDEX), _res_mgr(res_mgr), _scene_mgr(0), _helper_object(0)
{
    _model_id = LW_INVALID_INDEX;
    _opacity = 1.0f;

    memset(_obj_seq, 0, sizeof(lwIPrimitive*) * LW_MAX_MODEL_GEOM_OBJ_NUM);
    _obj_num = 0;
    lwMatrix44Identity(&_mat_base);
}
lwModel::~lwModel()
{
    Destroy();
}

LW_RESULT lwModel::Load(lwIModelObjInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_obj_num > 0)
        goto __ret;

    {
        lwModelObjInfo* m_info = (lwModelObjInfo*)info;

        lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
        lwISystem* sys = sys_graphics->GetSystem();
        lwIPathInfo* path_info = sys->GetPathInfo();
        lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
        BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

        for (DWORD i = 0; i < m_info->geom_obj_num; i++)
        {
            lwIPrimitive* imp;
            _res_mgr->CreatePrimitive(&imp);

            imp->Load((lwIGeomObjInfo*)m_info->geom_obj_seq[i], path_info->GetPath(PATH_TYPE_TEXTURE_SCENE), NULL);

            _obj_seq[_obj_num] = imp;
            _obj_num += 1;
        }

        if (m_info->helper_data.type != HELPER_TYPE_INVALID)
        {
            if (_helper_object == 0)
            {
                _helper_object = LW_NEW(lwHelperObject((lwResourceMgr*)_res_mgr));
                if (LW_FAILED(_helper_object->LoadHelperInfo(&m_info->helper_data, create_helper_primitive)))
                    goto __ret;
            }
        }

        lwMatrix44Identity(&_mat_base);

        _model_id = LW_INVALID_INDEX;

        _res_mgr->RegisterObject(&_id, this, OBJ_TYPE_MODEL);

        RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwModel::Load(const char* file, DWORD model_id)
{
    LW_RESULT ret = LW_RET_OK;

    if(_obj_num > 0)
    {
        return LW_RET_FAILED;
    }
  

    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = sys->GetPathInfo();
    lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
    BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

    // query model pool
    lwModel* obj_model = 0;
    if(model_id != LW_INVALID_INDEX)
    {
        _res_mgr->QueryModelObject((void**)&obj_model, model_id);
    }
    else
    {
        _res_mgr->QueryObject((void**)&obj_model, OBJ_TYPE_MODEL, file);
    }

    if(obj_model)
    {
        // copy
        Copy(obj_model);
    }
    else
    {
        // create new model object
        char path[LW_MAX_PATH];
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_MODEL_SCENE), file);


#ifdef USE_MODEL_INFO_LOAD

        lwModelObjInfo model_info;
        lwModelObjInfo* model_info_ptr;

#if(defined USE_RES_BUF_MGR)

        lwIResBufMgr* res_buf_mgr = _res_mgr->GetResBufMgr();

#if(defined LOAD_RES_BUF_RUNTIME)

        // use path query
        if(model_id == LW_INVALID_INDEX)
        {
            if(LW_FAILED(res_buf_mgr->QueryModelObjInfo((lwIModelObjInfo**)&model_info_ptr, path)))
            {
                LW_HANDLE handle;
                if(LW_FAILED(res_buf_mgr->RegisterModelObjInfo(&handle, path)))
                    return LW_RET_FAILED;

                if(LW_FAILED(res_buf_mgr->GetModelObjInfo((lwIModelObjInfo**)&model_info_ptr, handle)))
                    return LW_RET_FAILED;
            }
        }
        // USE_MODEL_ID_QUERY
        else
        {
            // first check object existed
            if(LW_FAILED(res_buf_mgr->GetModelObjInfo((lwIModelObjInfo**)&model_info_ptr, model_id)))
            {
                if(LW_FAILED(res_buf_mgr->RegisterModelObjInfo(model_id, path)))
                    return LW_RET_FAILED;

                res_buf_mgr->GetModelObjInfo((lwIModelObjInfo**)&model_info_ptr, model_id);
            }
            else
            {
                int x = 0;
            }
        }

        goto __load_model;

#else // LOAD_RES_BUF_RUNTIME
        if(model_id == LW_INVALID_INDEX)
        {
            if(LW_SUCCEEDED(res_buf_mgr->QueryModelObjInfo((lwIModelObjInfo**)&model_info_ptr, path)))
                goto __load_model;
        }
        else
        {
            if(LW_FAILED(res_buf_mgr->GetModelObjInfo((lwIModelObjInfo**)&model_info_ptr, model_id)))
                return LW_RET_FAILED;
        }
#endif

#endif


        if(LW_FAILED(model_info.Load(path)))
        {
            LG_MSGBOX("invalid file name: %s when called lwModel::Load", path);
            return LW_RET_FAILED;
        }

        model_info_ptr = &model_info;

__load_model:

        for(DWORD i = 0; i < model_info_ptr->geom_obj_num; i++)
        {
            lwResFile res;
            res.obj_id = i;
            res.res_type = RES_FILE_TYPE_MODEL;
            _tcscpy(res.file_name, path);

            lwIPrimitive* imp;
            _res_mgr->CreatePrimitive(&imp);

            imp->Load((lwIGeomObjInfo*)model_info_ptr->geom_obj_seq[i], path_info->GetPath(PATH_TYPE_TEXTURE_SCENE), &res);

            _obj_seq[_obj_num] = imp;
            _obj_num += 1;
        }

        if(model_info_ptr->helper_data.type != HELPER_TYPE_INVALID)
        {                
            if(_helper_object == 0)
            {
                _helper_object = LW_NEW(lwHelperObject((lwResourceMgr*)_res_mgr));
                if(LW_FAILED(_helper_object->LoadHelperInfo(&model_info_ptr->helper_data, create_helper_primitive)))
                {
                    LG_MSGBOX("load helper object error with file:%s", path);
                    return LW_RET_FAILED;
                }
            }
        }

#else

#error not support!!!

#endif

        SetFileName(file);

    }
    
    lwMatrix44Identity(&_mat_base);        

    _model_id = model_id;

    _res_mgr->RegisterObject(&_id, this, OBJ_TYPE_MODEL);

    RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());
    //SortPrimitiveObj();

    //this->ShowBoundingObject(1);

    return ret;

} 

LW_RESULT lwModel::Destroy()
{
    if(_obj_num == 0)
        return LW_RET_FAILED;
    
    for(DWORD i = 0; i < _obj_num; i++)
    {
        LW_SAFE_RELEASE(_obj_seq[i]);
    }
    _obj_num = 0;

    _res_mgr->UnregisterObject(NULL, _id, OBJ_TYPE_MODEL);
    _id = LW_INVALID_INDEX;

    LW_SAFE_RELEASE(_helper_object);

    return LW_RET_OK;
}

LW_RESULT lwModel::Copy(lwIModel* src_obj)
{
    lwModel* o = (lwModel*)src_obj;

    _tcscpy(_file_name, o->_file_name);
    _id = o->_id;
    _obj_num = o->_obj_num;
    _mat_base = o->_mat_base;
    
    for(DWORD i = 0; i < _obj_num; i++)
    {
        o->_obj_seq[i]->Clone(&_obj_seq[i]);
    }

    if(o->_helper_object)
    {
        o->_helper_object->Clone(&_helper_object);
    }

    return LW_RET_OK;
}

LW_RESULT lwModel::Clone(lwIModel** ret_obj)
{

    lwModel* o;
    _res_mgr->CreateModel((lwIModel**)&o);

    if(LW_FAILED(o->Copy(this)))
        return LW_RET_FAILED;

    *ret_obj = o;
    
    return LW_RET_OK;
}

LW_RESULT lwModel::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIPrimitive* imp;
    lwIPrimitive* pp;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        imp = _obj_seq[i];

        // 
        lwMatrix44 mat(_mat_base);
        pp = imp;
        while(pp->GetParentID() != LW_INVALID_INDEX)
        {
            if(_obj_seq[pp->GetParentID()])
            {
                lwMatrix44Multiply(&mat, _obj_seq[pp->GetParentID()]->GetMatrixLocal(), &mat);
                pp = _obj_seq[pp->GetParentID()];
            }
            else
            {
                break;
            }
        }

        imp->SetMatrixParent(&mat);

        if(LW_FAILED(imp->Update()))
            goto __ret;

    }

    if(_helper_object)
    {
        _helper_object->SetParentMatrix(&_mat_base);
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwModel::Render()
{    
    if(_state_ctrl.GetState(STATE_VISIBLE) == 0)
        return LW_RET_OK;

#if 0
    lwISysGraphics* sys_grh = _res_mgr->GetSysGraphics();
    lwISceneMgr* scn_mgr = sys_grh->GetSceneMgr();
    lwIOptionMgr* opt_mgr = sys_grh->GetSystem()->GetOptionMgr();
    BOOL cull_flag = opt_mgr->GetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL);

    //cull_flag = 0;

    lwIPrimitive* p;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        p = _obj_seq[i];

        if(cull_flag)
        {
            if(LW_SUCCEEDED(scn_mgr->CullPrimitive(p)))
            {
                continue;
            }
        }

        if(_scene_mgr && p->GetState(STATE_TRANSPARENT))
        {
            _scene_mgr->AddTransparentPrimitive(p);
        }
        else
        {
            p->Render();
        }
    }
#endif

    lwISysGraphics* sys_grh = _res_mgr->GetSysGraphics();
    lwISceneMgr* scn_mgr = sys_grh->GetSceneMgr();

    lwIPrimitive* p;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        p = _obj_seq[i];
        if(p == 0)
            continue;

        if(p->GetState(STATE_FRAMECULLING) == 1)
            continue;

        if(_scene_mgr && p->GetState(STATE_TRANSPARENT))
        {
            _scene_mgr->AddTransparentPrimitive(p);
        }
        else
        {
            p->Render();
        }
    }

    if(_helper_object)
    {
        _helper_object->Render();
    }

    return LW_RET_OK;
}

LW_RESULT lwModel::RenderPrimitive(DWORD id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(id >= _obj_num)
        goto __ret;
    {
        lwIPrimitive* p = _obj_seq[id];

        if (_scene_mgr && p->GetState(STATE_TRANSPARENT))
        {
            if (LW_FAILED(_scene_mgr->AddTransparentPrimitive(p)))
                goto __ret;
        }
        else
        {
            if (LW_FAILED(p->Render()))
                goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

LW_RESULT lwModel::RenderHelperObject()
{
    if(_helper_object)
    {
        return _helper_object->Render();
    }

    return LW_RET_OK;
}

void lwModel::SetMaterial(const lwMaterial* mtl)
{
    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->SetMaterial(mtl);
    }
}

void lwModel::SetOpacity(float opacity)
{
    _opacity = opacity;
    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->SetOpacity(opacity);
    }
}

LW_RESULT lwModel::HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray)
{
    lwIPrimitive* p;

    lwPickInfo u, v;
    v.obj_id = LW_INVALID_INDEX;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        if((p = _obj_seq[i]) == 0)
            continue;

        if(LW_SUCCEEDED(p->HitTest(&u, org, ray)))
        {
            if((v.obj_id == LW_INVALID_INDEX) || (v.dis > u.dis))
            {
                v = u;
                v.obj_id = i;
            }
        }
    }

    if(v.obj_id == LW_INVALID_INDEX)
    {
        return LW_RET_FAILED;
    }

    *info = v;

    return LW_RET_OK;
}

LW_RESULT lwModel::HitTestPrimitiveHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const char* type_name)
{
    lwPickInfo u, v;
    v.obj_id = LW_INVALID_INDEX;

    lwHelperMesh* obj;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        obj = (lwHelperMesh*)_obj_seq[i]->GetHelperObject()->GetHelperMesh();

        if(obj == 0 || obj->IsValidObject() == 0)
            continue;

        if(LW_FAILED(obj->HitTest(&u, org, ray, _obj_seq[i]->GetMatrixGlobal(), type_name)))
            continue;

        if(v.obj_id == LW_INVALID_INDEX || v.dis > u.dis)
        {
            v = u;
            v.data = i;
        }
    }

    if(v.obj_id == LW_INVALID_INDEX)
        return LW_RET_FAILED;

    *info = v;

    return LW_RET_OK;
}

LW_RESULT lwModel::HitTestPrimitiveHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const char* type_name)
{
    lwPickInfo u, v;
    v.obj_id = LW_INVALID_INDEX;

    lwHelperBox* obj;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        obj = (lwHelperBox*)_obj_seq[i]->GetHelperObject()->GetHelperBox();

        if(obj == 0 || obj->IsValidObject() == 0)
            continue;

        if(LW_FAILED(obj->HitTest(&u, org, ray, _obj_seq[i]->GetMatrixGlobal(), type_name)))
            continue;

        if(v.obj_id == LW_INVALID_INDEX || v.dis > u.dis)
        {
            v = u;
            v.data = i;
        }
    }

    if(v.obj_id == LW_INVALID_INDEX)
        return LW_RET_FAILED;

    *info = v;

    return LW_RET_OK;
}


LW_RESULT lwModel::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray)
{
    if(_helper_object == 0)
        return LW_RET_FAILED_2;

    LW_RESULT r1 = LW_RET_FAILED;
    LW_RESULT r2 = LW_RET_FAILED;

    lwPickInfo u;
    memset(&u, 0, sizeof(u));

    lwIBoundingSphere* bs = _helper_object->GetBoundingSphere();
    lwIBoundingBox* bb = _helper_object->GetBoundingBox();

    if(bs && LW_SUCCEEDED(r1 = bs->HitTest(&u, org, ray, &_mat_base)))
    {
        *info = u;
    }
    if(bb && LW_SUCCEEDED(r2 = bb->HitTest(&u, org, ray, &_mat_base)))
    {
        if(LW_FAILED(r1))
        {
            *info = u;
        }
        else if(info->dis > u.dis)
        {
            *info = u;
        }
    }

    return (LW_SUCCEEDED(r1) || LW_SUCCEEDED(r2)) ? LW_RET_OK : LW_RET_FAILED;
}

// return value:
// LW_RET_FAILED_2: no valid helper mesh
// LW_RET_FAILED: hit test failed
// LW_RET_OK: hit test succeeded
LW_RESULT lwModel::HitTestHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const char* type_name)
{
    lwPickInfo u;

    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
    {
        ret = LW_RET_FAILED_2;
        goto __ret;
    }

    // first check bounding object
#if 0
    if(_helper_object->GetBoundingBox() == 0
        && _helper_object->GetBoundingSphere() == 0)
    {
        return LW_RET_FAILED_2;
    }

    if(LW_FAILED(HitTest(&u, org, ray)))
    {
        return LW_RET_FAILED_2;
    }
#endif
    // end
    {
        lwHelperMesh* obj = (lwHelperMesh*)_helper_object->GetHelperMesh();

        if (obj == 0 || obj->IsValidObject() == 0)
        {
            ret = LW_RET_FAILED_2;
            goto __ret;
        }


        if (LW_FAILED(ret = obj->HitTest(&u, org, ray, &_mat_base, type_name)))
            goto __ret;

        *info = u;
    }
__ret:
    return ret;
}
// return value:
// LW_RET_FAILED_2: no valid helper mesh
// LW_RET_FAILED: hit test failed
// LW_RET_OK: hit test succeeded
LW_RESULT lwModel::HitTestHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const char* type_name)
{
    if(_helper_object == 0)
        return LW_RET_FAILED_2;

    lwHelperBox* obj = (lwHelperBox*)_helper_object->GetHelperBox();

    if(obj->IsValidObject() == 0)
        return LW_RET_FAILED_2;

    lwPickInfo u;

    if(LW_FAILED(obj->HitTest(&u, org, ray, &_mat_base, type_name)))
        return LW_RET_FAILED;

    *info = u;

    return LW_RET_OK;
}

void lwModel::ShowHelperObject(int show)
{
    if(_helper_object)
    {
        _helper_object->SetVisible(show);
    }

    lwIHelperObject* helper_obj;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        if((helper_obj = _obj_seq[i]->GetHelperObject()) != 0)
        {
            helper_obj->SetVisible(show);
        }
    }
}
void lwModel::ShowHelperMesh(int show)
{
    lwIHelperMesh* h;

    if(_helper_object)
    {
        if(h = _helper_object->GetHelperMesh())
        {
            h->SetVisible(show);
        }
    }

    for(DWORD i = 0; i < _obj_num; i++)
    {
        if(h = _obj_seq[i]->GetHelperObject()->GetHelperMesh())
        {
            h->SetVisible(show);
        }
    }
}
void lwModel::ShowHelperBox(int show)
{
    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->GetHelperObject()->GetHelperBox()->SetVisible(show);
    }
}
void lwModel::ShowBoundingObject(int show)
{
    lwIBoundingBox* bb;
    lwIBoundingSphere* bs;

    for(DWORD i = 0; i < _obj_num; i++)
    { 
        if(bb = _obj_seq[i]->GetHelperObject()->GetBoundingBox())
        {
            bb->SetVisible(show);
        }
        if(bs = _obj_seq[i]->GetHelperObject()->GetBoundingSphere())
        {
            bs->SetVisible(show);
        }
    }
}

LW_RESULT lwModel::SortPrimitiveObj()
{
    lwIPrimitive* p;

    for(DWORD i = 0; i < _obj_num - 1; i++)
    {
        for(DWORD j = i + 1; j< _obj_num; j++)
        {
            p = _obj_seq[i];
            _obj_seq[i] = _obj_seq[j];
            _obj_seq[j] = p;
            //if(_obj_seq[j]->_
        }
    }

    return LW_RET_OK;
}

LW_RESULT lwModel::PlayDefaultAnimation(bool IsGlitched)
{
    lwPlayPoseInfo ppi;
    memset(&ppi, 0, sizeof(ppi));
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.pose = 0;
    ppi.frame = 0.0f;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f; //Mdr.st : doesnt seem to change anything

    for(DWORD i = 0; i < _obj_num; i++)
    {
        lwPrimitivePlayDefaultAnimation(_obj_seq[i], IsGlitched);
        
    }

    return LW_RET_OK;
}

LW_RESULT lwModel::GetLinkCtrlMatrix(lwMatrix44* mat, DWORD link_id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == NULL)
        goto __ret;
    {
        lwIHelperDummy* d = _helper_object->GetHelperDummy();
        if (d == NULL)
            goto __ret;

        lwHelperDummyInfo* hdi = d->GetDataInfoWithID(link_id);
        if (hdi == NULL)
            goto __ret;
        {
            lwMatrix44* mat_parent = d->GetMatrixParent();
            lwMatrix44Multiply(mat, &hdi->mat, mat_parent);

            ret = LW_RET_OK;
        }
    }
__ret:
    return ret;
}

LW_RESULT lwModel::SetItemLink(const lwItemLinkInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == NULL)
        goto __ret;
    {
        lwIHelperDummy* d = _helper_object->GetHelperDummy();
        if (d == NULL)
            goto __ret;

        {
            lwHelperDummyInfo* hdi = d->GetDataInfoWithID(info->link_parent_id);
            if (hdi == NULL)
                goto __ret;

            ret = info->obj->SetLinkCtrl(this, info->link_parent_id, info->link_item_id);
        }
    }
__ret:
    return ret;
}

LW_RESULT lwModel::ClearItemLink(lwIItem* obj)
{
    return obj->ClearLinkCtrl();
}

LW_RESULT lwModel::SetTextureLOD(DWORD level)
{
    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->SetTextureLOD(level);
    }

    return LW_RET_OK;
}
LW_RESULT lwModel::CullPrimitive()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwISysGraphics* sys_grh = _res_mgr->GetSysGraphics();
    lwISceneMgr* scn_mgr = sys_grh->GetSceneMgr();
    lwIOptionMgr* opt_mgr = sys_grh->GetSystem()->GetOptionMgr();
    BOOL cull_flag = opt_mgr->GetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL);

    if(cull_flag == 0)
        goto __ret_ok;

    lwIPrimitive* p;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        p = _obj_seq[i];
        if(p == 0)
            continue;

        BYTE v = LW_SUCCEEDED(scn_mgr->CullPrimitive(p)) ? 1 : 0;

        p->SetState(STATE_FRAMECULLING, v);
    }

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
DWORD lwModel::GetCullingPrimitiveNum()
{
    DWORD n = 0;
    lwIPrimitive* p;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        p = _obj_seq[i];
        if(p == 0)
            continue;

        if(p->GetState(STATE_FRAMECULLING))
        {
            n++;
        }
    }

    return n;
}

LW_RESULT lwModel::ExtractModelInfo(lwIModelObjInfo* out_info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwModelObjInfo* a = (lwModelObjInfo*)out_info;

    DWORD pri_num = _obj_num;
    lwIPrimitive* pri = 0;

    // primitive
    for(DWORD i = 0; i < pri_num; i++)
    {
        pri = _obj_seq[i];
        a->geom_obj_seq[i] = LW_NEW(lwGeomObjInfo);
        if(LW_FAILED(pri->ExtractGeomObjInfo(a->geom_obj_seq[i])))
            goto __ret;
    }
    a->geom_obj_num = pri_num;

    // helper object
    if(_helper_object)
    {
        if(LW_FAILED(_helper_object->ExtractHelperInfo(&a->helper_data)))
            goto __ret;
    }

    
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_END
