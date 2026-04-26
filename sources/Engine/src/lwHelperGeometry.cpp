//

#include "stdafx.h"


#include "lwHelperGeometry.h"
#include "lwD3D.h"
#include "lwRenderImp.h"

LW_BEGIN
LW_RESULT lwLoadPrimitiveLineList(lwINodePrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf, const lwSubsetInfo* subset_seq, DWORD subset_num)
{
    lwMeshInfo info;

    info.fvf = FVF_XYZDIFFUSE;
    info.pt_type = D3DPT_LINELIST;

    info.vertex_num = vert_num;
    info.subset_num = subset_num;

    info.vertex_seq = LW_NEW(lwVector3[vert_num]);
    info.vercol_seq = LW_NEW(DWORD[vert_num]);
    info.subset_seq = LW_NEW(lwSubsetInfo[subset_num]);

    memcpy(info.vertex_seq, vert_buf, sizeof(lwVector3) * vert_num);
    memcpy(info.vercol_seq, color_buf, sizeof(DWORD) * vert_num);
    memcpy(&info.subset_seq[0], &subset_seq[0], sizeof(lwSubsetInfo) * subset_num);

    RSA_VALUE(&info.rs_set[0], D3DRS_LIGHTING, FALSE);

    if(LW_RESULT r = obj->LoadMesh(&info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMesh failed: name={}, vert_num={}, subset_num={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", vert_num, subset_num, static_cast<long long>(r));
        return LW_RET_FAILED;
    }

    lwMtlTexInfo mti;
    lwMaterial mtl;

    memset(&mtl, 0, sizeof(mtl));
    mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;

    mti.mtl = mtl;

    obj->AllocateMtlTexAgentSeq(subset_num);

    for(DWORD i = 0; i < subset_num; i++)
    {
        if(LW_RESULT r = obj->LoadMtlTex(i, &mti, NULL); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] obj->LoadMtlTex failed: name={}, i={}, subset_num={}, ret={}",
                         __FUNCTION__, name ? name : "(null)", i, subset_num, static_cast<long long>(r));
        }
    }

    lwRenderCtrlCreateInfo rcci;
    lwRenderCtrlCreateInfo_Construct(&rcci);
    rcci.ctrl_id = RENDERCTRL_VS_FIXEDFUNCTION;

    if(LW_RESULT r = obj->LoadRenderCtrl(&rcci); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadRenderCtrl failed: name={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", static_cast<long long>(r));
    }

    return LW_RET_OK;
}



LW_RESULT lwLoadPrimitiveLineList(lwIPrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf)
{
    lwMeshInfo info;

    info.fvf = FVF_XYZDIFFUSE;
    info.pt_type = D3DPT_LINELIST;

    info.vertex_num = vert_num;
    info.subset_num = 1;

    info.vertex_seq = LW_NEW(lwVector3[vert_num]);
    info.vercol_seq = LW_NEW(DWORD[vert_num]);
    info.subset_seq = LW_NEW(lwSubsetInfo[info.subset_num]);


    memcpy(info.vertex_seq, vert_buf, sizeof(lwVector3) * vert_num);
    memcpy(info.vercol_seq, color_buf, sizeof(DWORD) * vert_num);

    lwSubsetInfo_Construct(&info.subset_seq[0], vert_num / 2, 0, vert_num, 0);

    RSA_VALUE(&info.rs_set[0], D3DRS_LIGHTING, FALSE);

    if(LW_RESULT r = obj->LoadMesh(&info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMesh failed: name={}, vert_num={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", vert_num, static_cast<long long>(r));
        return LW_RET_FAILED;
    }

    lwMtlTexInfo mti;
    lwMaterial mtl;

    memset(&mtl, 0, sizeof(mtl));
    mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;

    mti.mtl = mtl;

    if(LW_RESULT r = obj->LoadMtlTex(0, &mti, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMtlTex failed: name={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", static_cast<long long>(r));
    }

    lwRenderCtrlCreateInfo rcci;
    lwRenderCtrlCreateInfo_Construct(&rcci);
    rcci.ctrl_id = RENDERCTRL_VS_FIXEDFUNCTION;

    if(LW_RESULT r = obj->LoadRenderCtrl(&rcci); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadRenderCtrl failed: name={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", static_cast<long long>(r));
    }


    return LW_RET_OK;
}

LW_RESULT lwLoadPrimitiveLineList(lwIPrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf, const lwSubsetInfo* subset_seq, DWORD subset_num)
{
    lwMeshInfo info;

    info.fvf = FVF_XYZDIFFUSE;
    info.pt_type = D3DPT_LINELIST;

    info.vertex_num = vert_num;
    info.subset_num = subset_num;

    info.vertex_seq = LW_NEW(lwVector3[vert_num]);
    info.vercol_seq = LW_NEW(DWORD[vert_num]);
    info.subset_seq = LW_NEW(lwSubsetInfo[subset_num]);

    memcpy(info.vertex_seq, vert_buf, sizeof(lwVector3) * vert_num);
    memcpy(info.vercol_seq, color_buf, sizeof(DWORD) * vert_num);
    memcpy(&info.subset_seq[0], &subset_seq[0], sizeof(lwSubsetInfo) * subset_num);

    RSA_VALUE(&info.rs_set[0], D3DRS_LIGHTING, FALSE);

    if(LW_RESULT r = obj->LoadMesh(&info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMesh (subset overload) failed: name={}, vert_num={}, subset_num={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", vert_num, subset_num, static_cast<long long>(r));
        return LW_RET_FAILED;
    }

    lwMtlTexInfo mti;
    lwMaterial mtl;

    memset(&mtl, 0, sizeof(mtl));
    mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;

    mti.mtl = mtl;

    for(DWORD i = 0; i < subset_num; i++)
    {
        if(LW_RESULT r = obj->LoadMtlTex(i, &mti, NULL); LW_FAILED(r))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] obj->LoadMtlTex (subset overload) failed: name={}, i={}, subset_num={}, ret={}",
                         __FUNCTION__, name ? name : "(null)", i, subset_num, static_cast<long long>(r));
        }
    }

    lwRenderCtrlCreateInfo rcci;
    lwRenderCtrlCreateInfo_Construct(&rcci);
    rcci.ctrl_id = RENDERCTRL_VS_FIXEDFUNCTION;

    if(LW_RESULT r = obj->LoadRenderCtrl(&rcci); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadRenderCtrl (subset overload) failed: name={}, ret={}",
                     __FUNCTION__, name ? name : "(null)", static_cast<long long>(r));
    }


    return LW_RET_OK;
}

LW_RESULT lwLoadPrimitiveGrid(lwIPrimitive* obj, const char* name, float width, float height, int row, int col, DWORD color)
{

	int i, id;


    float org_x = -width * 0.5f;
    float org_y = -height * 0.5f;
    float end_x = -org_x;
    float end_y = -org_y;

    float off_x = width / col;
    float off_y = height / row;

    int ver_num = (row + 1) * 2 + (col + 1) * 2;
    lwVector3* ver_buf = LW_NEW(lwVector3[ver_num]);
    DWORD* color_buf = LW_NEW(DWORD[ver_num]);

    // row line
    id = 0;
    for(i = 0; i <= row; i++) 
    {
        ver_buf[id].x = org_x;
        ver_buf[id].y = 0.0f;
        ver_buf[id].z = org_y + i * off_y;
        id += 1;

        ver_buf[id].x = end_x;
        ver_buf[id].y = 0.0f;
        ver_buf[id].z = org_y + i * off_y;
        id += 1;
    }

    // col line
    for(i = 0; i <= col; i++) 
    {
        ver_buf[id].x = org_x + i * off_x;
        ver_buf[id].y = 0.0f;
        ver_buf[id].z = org_y;
        id += 1;

        ver_buf[id].x = org_x + i * off_x;
        ver_buf[id].y = 0.0f;
        ver_buf[id].z = end_y;
        id += 1;
    }

    for(i = 0; i < ver_num; i++)
    {
        color_buf[i] = color;
    }

    LW_RESULT ret = lwLoadPrimitiveLineList(obj, name, ver_num, ver_buf, color_buf);

    LW_DELETE_A(ver_buf);
    LW_DELETE_A(color_buf);

    return ret;
}

LW_RESULT lwLoadPrimitiveAxis(lwIPrimitive* obj, const char* name, float len)
{
    const int ver_num = 6;
    lwVector3 ver_buf[ver_num] = 
    {
		lwVector3(0.0f, 0.0f, 0.0f),
		lwVector3( len, 0.0f, 0.0f),
		lwVector3(0.0f, 0.0f, 0.0f),
		lwVector3(0.0f,  len, 0.0f),
		lwVector3(0.0f, 0.0f, 0.0f),
		lwVector3(0.0f, 0.0f,  len)
	};

    DWORD color_buf[ver_num] = 
    {
        0xffff0000,
        0xffff0000,
        0xff00ff00,
        0xff00ff00,
        0xff0000ff,
        0xff0000ff
    };

    return lwLoadPrimitiveLineList(obj, name, ver_num, ver_buf, color_buf);
}

LW_RESULT lwLoadPrimitiveLineSphere(lwIPrimitive* obj, const char* name, DWORD color, float radius, int long_seg, int lat_seg)
{
    DWORD vert_num = lat_seg * long_seg * 2 * 2;

    lwVector3* vert_buf = LW_NEW(lwVector3[vert_num]);

    int index;
    float xo, yo, zo, ro;

    int i, j;

    float seg_ang = LW_2_PI / long_seg;
    float rin_ang = LW_PI / lat_seg;

    lwVector3* sp_buf = LW_NEW(lwVector3[(lat_seg + 1) * (long_seg + 1)]);

    for(i = 0; i <= lat_seg; i++)
    {
        ro = radius * sinf(i * rin_ang);
        yo = radius * cosf(i * rin_ang);

        for(j = 0; j <= long_seg; j++) 
        {
            xo = ro * cosf(j * (-seg_ang));
            zo = ro * sinf(j * (-seg_ang));

            index = i * (long_seg + 1) + j;

            sp_buf[index].x = xo;
            sp_buf[index].y = yo;
            sp_buf[index].z = zo;
        }
    }

    lwVector3* p = vert_buf;

    // lat. line list
    for(i = 0; i < lat_seg; i++)
    {
        for(j = 0; j < long_seg; j++)
        {
            *p++ = sp_buf[i * (long_seg + 1) + j];
            *p++ = sp_buf[i * (long_seg + 1) + j + 1];
        }
    }

    // long. line list
    for(i = 0; i < lat_seg; i++)
    {
        for(j = 0; j < long_seg; j++)
        {
            *p++ = sp_buf[i * (long_seg + 1) + j];
            *p++ = sp_buf[(i + 1) * (long_seg + 1) + j];
        }
    }

    LW_DELETE_A(sp_buf);

    DWORD* color_buf = LW_NEW(DWORD[vert_num]);
    for(DWORD i = 0; i < vert_num; i++)
    {
        color_buf[i] = color;
    }

    LW_RESULT ret = lwLoadPrimitiveLineList(obj, "GenericSphere", vert_num, vert_buf, color_buf);

    LW_DELETE_A(vert_buf);
    LW_DELETE_A(color_buf);

    return ret;

}

LW_RESULT lwLoadPrimitiveLineCube(lwIPrimitive* obj, const char* name, DWORD color, const lwVector3* size)
{
    const DWORD vert_num = 24;
    lwVector3* vert_buf = LW_NEW(lwVector3[vert_num]);
    lwSubsetInfo* subset_seq = LW_NEW(lwSubsetInfo[1]);

    subset_seq[0].vertex_num = 24;
    subset_seq[0].primitive_num = 12;
    subset_seq[0].min_index = 0;
    subset_seq[0].start_index = 0;

    lwBox box(lwVector3(0.0f, 0.0f, 0.0f), *size / 2);
    lwGetBoxVertLineList(&vert_buf[0], &box);

    DWORD* color_buf = LW_NEW(DWORD[vert_num]);
    for(DWORD i = 0; i < vert_num; i++)
    {
        color_buf[i] = color;
    }

    LW_RESULT ret = lwLoadPrimitiveLineList(obj, name, vert_num, vert_buf, color_buf, subset_seq, 1);

    LW_DELETE_A(vert_buf);
    LW_DELETE_A(color_buf);
    LW_DELETE_A(subset_seq);

    return ret;

}

// use standard left-hand coord.
LW_RESULT lwLoadPrimitivePlane(lwIPrimitive* obj, const char* name, DWORD color, float width, float height, int seg_width, int seg_height, BOOL two_side, BOOL wire_frame)
{
    float min_w = -width / 2;
    float max_w = -min_w;
    float min_h = -height / 2;
    float max_h = -min_h;
    float s_w = width / seg_width;
    float s_h = height / seg_height;

    int wo = seg_width + 1;
    int ho = seg_height + 1;

    // vertex seq
    int vert_num = wo * ho;
    lwVector3* vert_seq = LW_NEW(lwVector3[vert_num]);
    lwVector3* v;

    for(int i = 0; i < ho; i++)
    { // z-axis-offset
        for(int j = 0; j < wo; j++)
        { // x-axis-offset
            int m = i * wo + j;
            v = &vert_seq[m];
            v->x = min_w + s_w * j;
            v->y = 0.0f;
            v->z = min_h + s_h * i;
        }
    }

    // index seq

    // diagram
    //  d---------
    //   |   |   |
    //  c---e-----b
    //   |   |   |
    //   ---------a
    //   ab[bc]cde
    //   abb-bbc-bcc-ccd
    //   4 trianges
    const int e = 2; // ecdysis point
    int seg_j = seg_width + 1;
    int ind_num = seg_j * 2 * seg_height + (seg_height - 1) * e;
    DWORD* ind_seq = LW_NEW(DWORD[ind_num]);

    for(int i = 0; i < seg_height; i++)
    {
        int i_s_j = i * seg_j;
        int i_n = i_s_j * 2 + (i * e);

        if(i > 0)
        {
            ind_seq[i_n - 1] = i_s_j;
            ind_seq[i_n - 2] = i_s_j + seg_j - 1;
        }

        for(int j = 0; j < seg_j; j++)
        {
            ind_seq[i_n + j * 2] = i_s_j + j;
            ind_seq[i_n + j * 2 + 1] = i_s_j + j + seg_j;
        }
    }

    // color seq
    DWORD* col_seq = LW_NEW(DWORD[vert_num]);
    for(int i = 0; i < vert_num; i++)
    {
        col_seq[i] = color;
    }

    // construct lwMeshInfo
    lwMeshInfo info;
    lwMeshInfo_Construct(&info);

    info.vertex_num = vert_num;
    info.index_num = ind_num;
    info.subset_num = 1;
    info.fvf = FVF_XYZDIFFUSE;
    info.pt_type = D3DPT_TRIANGLESTRIP;

    int rss_id = 0;
    RSA_VALUE(&info.rs_set[rss_id++], D3DRS_LIGHTING, 0);
    if(two_side)
    {
        RSA_VALUE(&info.rs_set[rss_id++], D3DRS_CULLMODE, D3DCULL_NONE);
    }
    if(wire_frame)
    {
        RSA_VALUE(&info.rs_set[rss_id++], D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    }

    info.subset_seq = LW_NEW(lwSubsetInfo[info.subset_num]);
    lwSubsetInfo_Construct(&info.subset_seq[0], ind_num -2, 0, vert_num, 0);

    info.vertex_seq = vert_seq;
    info.index_seq = ind_seq;
    info.vercol_seq = col_seq;

    if(LW_RESULT r = obj->LoadMesh(&info); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMesh (Plane) failed: vertex_num={}, index_num={}, ret={}",
                     __FUNCTION__, info.vertex_num, info.index_num, static_cast<long long>(r));
        return LW_RET_FAILED;
    }

    lwMtlTexInfo mti;
    lwMaterial mtl;

    memset(&mtl, 0, sizeof(mtl));
    mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;

    mti.mtl = mtl;

    if(LW_RESULT r = obj->LoadMtlTex(0, &mti, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadMtlTex (Plane) failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
    }

    lwRenderCtrlCreateInfo rcci;
    lwRenderCtrlCreateInfo_Construct(&rcci);
    rcci.ctrl_id = RENDERCTRL_VS_FIXEDFUNCTION;

    if(LW_RESULT r = obj->LoadRenderCtrl(&rcci); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] obj->LoadRenderCtrl (Plane) failed: ret={}",
                     __FUNCTION__, static_cast<long long>(r));
    }

    return LW_RET_OK;
}

LW_END
