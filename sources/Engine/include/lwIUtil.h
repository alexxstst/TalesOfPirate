//
#pragma once

#include "lwHeader.h"
#include "lwInterface.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"

LW_BEGIN
	LW_RESULT lwResetDevice(lwISysGraphics* sys_graphics, const D3DPRESENT_PARAMETERS* d3dpp);
	void lwPhysiqueSetMaterial(lwIPhysique* phy, const lwMaterial* mtl);
	lwPlayPoseInfo* lwItemGetPlayPoseInfo(lwIItem* item, DWORD ctrl_type);
	lwIAnimCtrl* lwItemGetAnimCtrl(lwIItem* item, DWORD ctrl_type);
	LW_RESULT lwPrimitiveSetRenderCtrl(lwIPrimitive* p, DWORD ctrl_type);
	LW_RESULT lwPrimitiveSetVertexShader(lwIPrimitive* p, DWORD shader_type);
	LW_RESULT lwPrimitiveGetObjHeight(lwIPrimitive* p, float* out_height);

	LW_RESULT lwPrimitiveTexLit(lwIPrimitive* p, std::string_view file, std::string_view tex_path, DWORD color_op,
								DWORD anim_type);
	LW_RESULT lwPrimitiveTexLitC(lwIPrimitive* p, std::string_view file, std::string_view tex_path, DWORD anim_type);
	LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* alpha_file, const char* tex_file, std::string_view tex_path,
								 DWORD anim_type);
	LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* tex_file, std::string_view tex_path, DWORD anim_type);
	LW_RESULT lwPrimitiveTexUnLitA(lwIPrimitive* p);

	LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, std::string_view file, std::string_view tex_path, D3DFORMAT fmt);
	LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const lwTexInfo* info);
	// Для TEX_TYPE_DATA: пользовательский указатель пробрасывается параметром,
	LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const lwTexInfo* info, void* user_data);

	lwPoseInfo* lwAnimCtrlAgentGetPoseInfo(lwIAnimCtrlAgent* agent, DWORD subset, DWORD stage, DWORD type, DWORD id);

	void lwReleaseTreeNodeList(lwITreeNode* node);
	int lwTreeNodeEnumPreOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);
	int lwTreeNodeEnumInOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);
	int lwTreeNodeEnumPostOrder(lwITreeNode* node, lwTreeNodeEnumProc proc, void* param);


	class lwINodeObjectA {
	public:
		static LW_RESULT PlayDefaultPose(lwINodeObject* obj);
	};

LW_END
