//
#pragma once

#include "MindPower.h"


class RenderStateMgr {
private:
	MPIDeviceObject* _dev_obj;
	MPIRenderStateAtomSet* _rsa_scene;
	MPIRenderStateAtomSet* _rsa_cha;
	MPIRenderStateAtomSet* _rsa_sceneobj;
	MPIRenderStateAtomSet* _rsa_sceneitem;
	MPIRenderStateAtomSet* _rsa_terrain;
	MPIRenderStateAtomSet* _rsa_transpobj;

	D3DLIGHTX _scnobj_lgt;
	D3DLIGHTX _scnobj_lgt_old[3];
	BOOL _scnobj_lgt_enable_old[3];

	D3DLIGHTX _cha_lgt;
	D3DLIGHTX _cha_lgt_old;
	BOOL _cha_lgt_enable_old;

public:
	RenderStateMgr();
	~RenderStateMgr();

	HRESULT Init(MPIDeviceObject* dev_obj);

	HRESULT BeginScene();
	HRESULT BeginCharacter();
	HRESULT BeginSceneObject();
	HRESULT BeginSceneItem();
	HRESULT BeginTerrain();
	HRESULT BeginTranspObject();
	HRESULT EndScene();
	HRESULT EndCharacter();
	HRESULT EndSceneObject();
	HRESULT EndSceneItem();
	HRESULT EndTerrain();
	HRESULT EndTranspObject();
};
