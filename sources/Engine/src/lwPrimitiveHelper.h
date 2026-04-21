//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwExpObj.h"
#include "lwITypes.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"

LW_BEGIN

class lwHelperBase
{
protected:
    int _visible_flag;
    lwMatrix44 _mat_parent;

public:
    lwHelperBase()
        : _visible_flag(0)
    {}

    void SetMatrixParent( const lwMatrix44* mat ) { _mat_parent = *mat; }
    
};

class lwBoundingBox : public lwHelperBase, public lwIBoundingBox
{

LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIPrimitive* _obj;
    lwBoundingBoxInfo* _obj_seq;
    DWORD _obj_num;
    DWORD _data;

public:
    lwBoundingBox();
    ~lwBoundingBox();

    void SetResourceMgr( lwIResourceMgr* mgr ) { _res_mgr = mgr; }
    LW_RESULT CreateInstance();
    LW_RESULT Clone( const lwBoundingBox* src );

    LW_RESULT SetDataInfo( const lwBoundingBoxInfo* obj_seq, DWORD obj_num );

    virtual int IsVisible() const { return _visible_flag; }
    virtual void SetVisible( int flag ) { _visible_flag = flag; }
    virtual int IsValidObject() const { return _obj_num > 0; }
    virtual lwBoundingBoxInfo* GetDataInfoWithID( DWORD obj_id );
    virtual lwBoundingBoxInfo* GetDataInfo( DWORD id ) { return &_obj_seq[ id ]; }
    virtual DWORD GetObjNum() const { return _obj_num; }
    lwMatrix44* GetMatrixParent() { return &_mat_parent; }

    virtual LW_RESULT HitTest( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwMatrix44* mat_parent );
    virtual LW_RESULT Render();
    void SetData( DWORD data ) { _data = data; }
    DWORD GetData() const { return _data; }

};

class lwBoundingSphere : public lwHelperBase, public lwIBoundingSphere
{

LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIPrimitive* _obj;

    lwBoundingSphereInfo* _obj_seq;
    DWORD _obj_num;

    DWORD _data;
public:
    lwBoundingSphere();
    ~lwBoundingSphere();

    void SetResourceMgr( lwIResourceMgr* mgr ) { _res_mgr = mgr; }
    LW_RESULT CreateInstance();
    LW_RESULT Clone( const lwBoundingSphere* src );

    LW_RESULT SetDataInfo( const lwBoundingSphereInfo* obj_seq, DWORD obj_num );
    virtual lwBoundingSphereInfo* GetDataInfoWithID( DWORD obj_id );
    virtual DWORD GetObjNum() const { return _obj_num; }
    virtual lwBoundingSphereInfo* GetDataInfo( DWORD id ) { return &_obj_seq[ id ]; }
    lwMatrix44* GetMatrixParent() { return &_mat_parent; }

    virtual int IsVisible() const { return _visible_flag; }
    virtual void SetVisible( int flag ) { _visible_flag = flag; }
    virtual int IsValidObject() const { return _obj_num > 0; }

    virtual LW_RESULT HitTest( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwMatrix44* mat_parent );
    virtual LW_RESULT Render();

    void SetData( DWORD data ) { _data = data; }
    DWORD GetData() const { return _data; }
};


class lwHelperDummy : public lwHelperBase, public lwIHelperDummy
{

LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIPrimitive* _obj;

    lwHelperDummyInfo* _obj_seq;
    DWORD _obj_num;

public:
    lwHelperDummy();
    ~lwHelperDummy();

    LW_RESULT SetDataInfo( const lwHelperDummyInfo* obj_seq, DWORD obj_num );

    LW_RESULT Clone( const lwHelperDummy* src );

    LW_RESULT CreateInstance();
    LW_RESULT Render();


    void SetResourceMgr( lwIResourceMgr* mgr ) { _res_mgr = mgr; }
    virtual int IsVisible() const { return _visible_flag; }
    virtual void SetVisible( int flag ) { _visible_flag = flag; }
    virtual int IsValidObject() const { return _obj_num > 0; }
    virtual lwMatrix44* GetMatrixParent() { return &_mat_parent; }
    DWORD GetObjNum() { return _obj_num; }
    lwHelperDummyInfo* GetDataInfo(DWORD id) { return &_obj_seq[id]; }
    lwHelperDummyInfo* GetDataInfoWithID(DWORD obj_id);
};

class lwHelperMesh : public lwHelperBase, public lwIHelperMesh
{
LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIPrimitive* _obj;

    lwHelperMeshInfo* _obj_seq;
    DWORD _obj_num;

public:
    lwHelperMesh();
    ~lwHelperMesh();

    void SetResourceMgr( lwIResourceMgr* mgr ) { _res_mgr = mgr; }
    void SetDataInfo( const lwHelperMeshInfo* obj_seq, DWORD obj_num );
    virtual lwHelperMeshInfo* GetDataInfoWithID( DWORD obj_id );
    lwHelperMeshInfo* GetDataInfo(DWORD id) { return &_obj_seq[id]; }
    DWORD GetObjNum() { return _obj_num; }

    LW_RESULT Clone( const lwHelperMesh* src );

    LW_RESULT CreateInstance();
    LW_RESULT HitTest( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwMatrix44* mat_parent, const char* type_name );
    LW_RESULT Render();

    virtual int IsVisible() const { return _visible_flag; }
    virtual void SetVisible( int flag ) { _visible_flag = flag; }
    virtual int IsValidObject() const { return _obj_num > 0; }

};

class lwHelperBox : public lwHelperBase, public lwIHelperBox
{

LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIPrimitive* _obj;

    lwHelperBoxInfo* _obj_seq;
    DWORD _obj_num;

public:
    lwHelperBox();
    ~lwHelperBox();

    void SetResourceMgr( lwIResourceMgr* mgr ) { _res_mgr = mgr; }
    LW_RESULT SetDataInfo( const lwHelperBoxInfo* obj_seq, DWORD obj_num );
    virtual lwHelperBoxInfo* GetDataInfoWithID( DWORD obj_id );
    lwHelperBoxInfo* GetDataInfo(DWORD id) { return &_obj_seq[id]; }
    DWORD GetObjNum() { return _obj_num; }

    LW_RESULT Clone( const lwHelperBox* src );

   LW_RESULT CreateInstance();
   LW_RESULT HitTest( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwMatrix44* mat_parent, const char* type_name );
   LW_RESULT Render();

    virtual int IsVisible() const { return _visible_flag; }
    virtual void SetVisible( int flag ) { _visible_flag = flag; }
    virtual int IsValidObject() const { return _obj_num > 0; }

};


class lwHelperObject : public lwIHelperObject
{
LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;

    lwHelperDummy* _obj_dummy;
    lwHelperBox* _obj_box;
    lwHelperMesh* _obj_mesh;

    lwBoundingBox* _obj_boundingbox;
    lwBoundingSphere* _obj_boundingsphere;

public:

    lwHelperObject( lwIResourceMgr* mgr );
    ~lwHelperObject();

    LW_RESULT LoadHelperInfo( const lwHelperInfo* info, int create_instance_flag );
    LW_RESULT Copy( const lwIHelperObject* src );
    LW_RESULT Clone(lwIHelperObject** ret_obj);

    void SetParentMatrix(const lwMatrix44* mat);
    LW_RESULT Update();
    LW_RESULT Render();

    void SetVisible( int flag );
    lwIHelperDummy* GetHelperDummy() { return _obj_dummy; }
    lwIHelperBox* GetHelperBox() { return _obj_box; }
    lwIHelperMesh* GetHelperMesh() { return _obj_mesh; }
    lwIBoundingBox* GetBoundingBox() { return _obj_boundingbox; }
    lwIBoundingSphere* GetBoundingSphere() { return _obj_boundingsphere; }

    LW_RESULT ExtractHelperInfo(lwIHelperInfo* out_info);

};

LW_END
