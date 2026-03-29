//
#pragma once

#include "lwHeader.h"
#include "lwInterface.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"
#include "lwFrontAPI.h"

LW_BEGIN


LW_FRONT_API LW_RESULT lwResetDevice(lwISysGraphics* sys_graphics, const D3DPRESENT_PARAMETERS* d3dpp);
LW_FRONT_API void lwPhysiqueSetMaterial(lwIPhysique* phy, const lwMaterial* mtl);
LW_FRONT_API lwPlayPoseInfo* lwItemGetPlayPoseInfo(lwIItem* item, DWORD ctrl_type);
LW_FRONT_API lwIAnimCtrl* lwItemGetAnimCtrl(lwIItem* item, DWORD ctrl_type);
LW_FRONT_API LW_RESULT lwPrimitiveSetRenderCtrl(lwIPrimitive* p, DWORD ctrl_type);
LW_FRONT_API LW_RESULT lwPrimitiveSetVertexShader(lwIPrimitive* p, DWORD shader_type);
LW_FRONT_API LW_RESULT lwPrimitiveGetObjHeight(lwIPrimitive* p, float* out_height);

LW_FRONT_API LW_RESULT lwPrimitiveTexLit(lwIPrimitive* p, const char* file, const char* tex_path, DWORD color_op, DWORD anim_type);
LW_FRONT_API LW_RESULT lwPrimitiveTexLitC(lwIPrimitive* p, const char* file, const char* tex_path, DWORD anim_type);
LW_FRONT_API LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* alpha_file, const char* tex_file, const char* tex_path, DWORD anim_type);
LW_FRONT_API LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* tex_file, const char* tex_path, DWORD anim_type);
LW_FRONT_API LW_RESULT lwPrimitiveTexUnLitA(lwIPrimitive* p);

LW_FRONT_API LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const char* file, const char* tex_path, D3DFORMAT fmt);
LW_FRONT_API LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const lwTexInfo* info);

LW_FRONT_API lwPoseInfo* lwAnimCtrlAgentGetPoseInfo(lwIAnimCtrlAgent* agent, DWORD subset, DWORD stage, DWORD type, DWORD id);

LW_FRONT_API void lwMessageBox( const char* fmt, ... );

LW_FRONT_API void lwReleaseTreeNodeList(lwITreeNode* node);
LW_FRONT_API int lwTreeNodeEnumPreOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);
LW_FRONT_API int lwTreeNodeEnumInOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);
LW_FRONT_API int lwTreeNodeEnumPostOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);


class LW_FRONT_API lwINodeObjectA
{
public:
    static LW_RESULT PlayDefaultPose(lwINodeObject* obj);
};

LW_END
