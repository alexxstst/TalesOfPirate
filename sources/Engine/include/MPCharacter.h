//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwInterface.h"
#include "lwObjectMethod.h"
#include "MPSceneItem.h"

#include "lwFrontAPI.h"

LW_BEGIN

class lwActionObjectImp;
class lwActionSmith;

struct lwSceneItemLinkInfo
{
    union
    {
        MPSceneItem* obj;
        void* obj_void;
    };

    DWORD id;
    DWORD link_item_id;
    DWORD link_parent_id;
    DWORD data;
};

struct MPItemLinkInfo
{
    MPSceneItem* obj;
    DWORD link_item_id;
    DWORD link_parent_id;
    DWORD data;
};
struct MPChaLoadInfo
{
    MPChaLoadInfo()
    {
        memset( this, 0, sizeof(MPChaLoadInfo) );

    }

	std::string pixelShader;
    char bone[64];
    char part[LW_MAX_SUBSKIN_NUM][64];
};

typedef MPChaLoadInfo mpChaLoadInfo;

class LW_FRONT_API MPCharacter : public lwMatrixCtrl
{
    struct MPLinkInfo
    {
        MPSceneItem* obj;
        DWORD data;
    };
private:
    lwIPhysique* _physique;

    MPLinkInfo _link_item_seq[ LW_MAX_LINK_ITEM_NUM ];
    DWORD _link_item_num;

    lwPoseKeyFrameProc _pose_proc;
    void* _proc_param;

public:
    MPCharacter();
    MPCharacter( lwISysGraphics* sys_graphics );
    virtual ~MPCharacter();

	void StartLoad();
	void EndLoad();
	bool IsLoaded();

	BOOL	  InitBone( const char* file );
    LW_RESULT LoadBone( const char* file );
    LW_RESULT Load( DWORD obj_id, DWORD group_id, DWORD* skin_id_seq, int load_bone = 1 );
    LW_RESULT Load( const MPChaLoadInfo* info );
    LW_RESULT LoadPart( DWORD part_id, DWORD file_id );
    LW_RESULT LoadPart( DWORD part_id, const char* file );
    LW_RESULT DestroyPart( DWORD part_id );

    LW_RESULT PlayPose( const lwPlayPoseInfo* info );
    LW_RESULT PlayPose( DWORD pose_id, DWORD play_type, float start_frame = 0.0f, float velocity = 1.0f, DWORD blend_flag = 1, DWORD blend_src_num = 5);
    LW_RESULT SetPoseKeyFrameProc( lwPoseKeyFrameProc proc, void* proc_param );
    LW_RESULT SetObjImpPoseKeyFrameProc( DWORD obj_id, DWORD ctrl_type, lwPoseKeyFrameProc proc, void* proc_param);
    LW_RESULT PlayObjImpPose( DWORD obj_id, DWORD ctrl_type, const lwPlayPoseInfo* info );
    LW_RESULT PlayObjImpPose( DWORD obj_id, DWORD ctrl_type, DWORD pose_id, DWORD play_type, float start_frame = 0.0f, float velocity = 1.0f );

    lwPlayPoseInfo* GetPlayPoseInfo();
    lwPlayPoseInfo* GetObjImpPlayPoseInfo( DWORD obj_id, DWORD ctrl_type );
    lwIPoseCtrl* GetPoseCtrl();    
    lwIPoseCtrl* GetObjImpPoseCtrl( DWORD skin_id, DWORD ctrl_type );

    LW_RESULT HitTest( lwPickInfo* info, const lwVector3* org, const lwVector3* ray );

    void FrameMove();
    void Render();

    void Destroy();


    LW_RESULT HitTestPrimitive( lwPickInfo* info, const lwVector3* org, const lwVector3* ray );
    LW_RESULT HitTestPhysique( lwPickInfo* info, const lwVector3* org, const lwVector3* ray );


    void ShowHelperObject( int show );
    void ShowBoundingObjectPhysique( int show );


    LW_RESULT SetSubObjItemLink( DWORD obj_id, const lwSceneItemLinkInfo* info );

    LW_RESULT AttachItem( const MPItemLinkInfo* info );
    LW_RESULT SetItemLink( const lwSceneItemLinkInfo* info );
    LW_RESULT ClearItemLink( DWORD type );
    LW_RESULT UnwieldItem( const MPSceneItem* obj );
    DWORD GetLinkItemNum() const { return _link_item_num; }
    MPSceneItem* GetLinkItem( DWORD id );

    DWORD CheckPosePlayingStatus();

    void SetObjState( DWORD state, BYTE value );
    DWORD GetObjState( DWORD state ) const;

    lwIPhysique* GetPhysique() { return _physique; }

    LW_RESULT GetObjDummyRunTimeMatrix( lwMatrix44* mat, DWORD obj_id, DWORD dummy_id );
    LW_RESULT GetObjDummyRunTimeMatrix( lwMatrix44* mat, DWORD dummy_id );
    void SetMaterial(const D3DMATERIALX* mtl);
    void SetOpacity(float opacity);
    void SetTextureLOD(DWORD level);
    float GetOpacity();

	void setComponentColour( size_t index, D3DCOLOR colour, const std::string& filterTextureName = "" )
	{
		_physique->setComponentColour( index, colour, filterTextureName );
	}

	void setTextureOperation( size_t index, D3DTEXTUREOP operation )
	{
		_physique->setTextureOperation( index, operation );
	}

	const char* getTextureOperationDescription( size_t operation )
	{
		return _physique->getTextureOperationDescription( operation );
	}

	void setPixelShader( size_t index, const std::string& filename )
	{
		_physique->setPixelShader( index, filename );
	}
};

typedef MPCharacter MPCharacter;

LW_END
