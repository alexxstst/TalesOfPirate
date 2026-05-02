//
#include "stdafx.h"


#include "lwPrimitiveHelper.h"
#include "lwD3D.h"
#include "lwRenderImp.h"
#include "lwResourceMgr.h"
#include "lwDeviceObject.h"
#include "lwHelperGeometry.h"

#define USE_VS_INDEXED

LW_BEGIN
	lwVector3 getScale(const lwMatrix44& matrix) {
		lwVector3 scale;
		scale.x = sqrt(matrix._11 * matrix._11 + matrix._12 * matrix._12 + matrix._13 * matrix._13);
		scale.y = sqrt(matrix._21 * matrix._21 + matrix._22 * matrix._22 + matrix._23 * matrix._23);
		scale.z = sqrt(matrix._31 * matrix._31 + matrix._32 * matrix._32 + matrix._33 * matrix._33);
		return scale;
	}

	float max3(const lwVector3& vec) {
		return vec.x >= vec.y ? (vec.x >= vec.z ? vec.x : vec.z) : (vec.y >= vec.z ? vec.y : vec.z);
	}

	// lwHelperDummy
	LW_STD_IMPLEMENTATION(lwHelperDummy)

	lwHelperDummy::lwHelperDummy()
		: _obj_seq(0), _obj_num(0), _obj(0) {
	}

	lwHelperDummy::~lwHelperDummy() {
		LW_IF_RELEASE(_obj);
		LW_IF_DELETE_A(_obj_seq);
	}

	LW_RESULT lwHelperDummy::SetDataInfo(const lwHelperDummyInfo* obj_seq, DWORD obj_num) {
		LW_SAFE_DELETE_A(_obj_seq);

		_obj_num = obj_num;
		_obj_seq = LW_NEW(lwHelperDummyInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &obj_seq[0], sizeof(lwHelperDummyInfo) * _obj_num);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperDummy::Clone(const lwHelperDummy* src) {
		if (src->IsValidObject() == 0)
			return LW_RET_OK;

		_res_mgr = src->_res_mgr;
		_visible_flag = src->_visible_flag;

		if (src->_obj) {
			src->_obj->Clone(&_obj);
		}

		_obj_num = src->_obj_num;
		_obj_seq = LW_NEW(lwHelperDummyInfo[ _obj_num ]);

		memcpy(&_obj_seq[0], &src->_obj_seq[0], sizeof(lwHelperDummyInfo) * _obj_num);


		return LW_RET_OK;
	}

	LW_RESULT lwHelperDummy::CreateInstance() {
		if (_obj_num == 0)
			return LW_RET_FAILED;

		_res_mgr->CreatePrimitive(&_obj);

		const float vert_len = 0.1f;

		lwVector3 vert_buf[24] =
		{
			// bottom
			lwVector3(-vert_len, -vert_len, -vert_len),
			lwVector3(-vert_len, -vert_len, vert_len),

			lwVector3(-vert_len, -vert_len, vert_len),
			lwVector3(vert_len, -vert_len, vert_len),

			lwVector3(vert_len, -vert_len, vert_len),
			lwVector3(vert_len, -vert_len, -vert_len),

			lwVector3(vert_len, -vert_len, -vert_len),
			lwVector3(-vert_len, -vert_len, -vert_len),

			// top
			lwVector3(-vert_len, vert_len, -vert_len),
			lwVector3(-vert_len, vert_len, vert_len),

			lwVector3(-vert_len, vert_len, vert_len),
			lwVector3(vert_len, vert_len, vert_len),

			lwVector3(vert_len, vert_len, vert_len),
			lwVector3(vert_len, vert_len, -vert_len),

			lwVector3(vert_len, vert_len, -vert_len),
			lwVector3(-vert_len, vert_len, -vert_len),

			// side
			lwVector3(-vert_len, -vert_len, -vert_len),
			lwVector3(-vert_len, vert_len, -vert_len),

			lwVector3(-vert_len, -vert_len, vert_len),
			lwVector3(-vert_len, vert_len, vert_len),

			lwVector3(vert_len, -vert_len, vert_len),
			lwVector3(vert_len, vert_len, vert_len),

			lwVector3(vert_len, -vert_len, -vert_len),
			lwVector3(vert_len, vert_len, -vert_len),
		};


		DWORD cor_buf[24];
		DWORD c = 0xffff0000;
		for (DWORD i = 0; i < 24; i++) {
			cor_buf[i] = c;
		}

		return lwLoadPrimitiveLineList(_obj, "object_dummy", 24, vert_buf, cor_buf);
	}

	LW_RESULT lwHelperDummy::Render() {
		if (_obj == 0 || _visible_flag == 0)
			return LW_RET_OK;


		for (DWORD i = 0; i < _obj_num; i++) {
			lwMatrix44 mat(_obj_seq[i].mat);
			lwMatrix44Multiply(&mat, &mat, &_mat_parent);

			_obj->SetMatrixParent(&mat);
			if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->Update failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
			if (LW_RESULT r = _obj->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->Render failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	lwHelperDummyInfo* lwHelperDummy::GetDataInfoWithID(DWORD obj_id) {
		for (DWORD i = 0; i < _obj_num; i++) {
			if (_obj_seq[i].id == obj_id)
				return &_obj_seq[i];
		}

		return NULL;
	}

	// lwBoundingBox
	LW_STD_IMPLEMENTATION(lwBoundingBox)

	lwBoundingBox::lwBoundingBox()
		: _obj_seq(0), _obj_num(0), _data(0), _obj(0) {
	}

	lwBoundingBox::~lwBoundingBox() {
		LW_SAFE_DELETE_A(_obj_seq);
		LW_IF_RELEASE(_obj);
	}

	LW_RESULT lwBoundingBox::SetDataInfo(const lwBoundingBoxInfo* obj_seq, DWORD obj_num) {
		_obj_num = obj_num;
		_obj_seq = LW_NEW(lwBoundingBoxInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &obj_seq[0], sizeof(lwBoundingBoxInfo) * _obj_num);

		return LW_RET_OK;
	}

	LW_RESULT lwBoundingBox::CreateInstance() {
		if (_obj_num == 0)
			return LW_RET_FAILED;

		_res_mgr->CreatePrimitive(&_obj);

		DWORD vert_num = _obj_num * 24;
		lwVector3* vert_buf = LW_NEW(lwVector3[ vert_num ]);

		lwSubsetInfo* subset_seq = LW_NEW(lwSubsetInfo[ _obj_num ]);

		for (DWORD i = 0; i < _obj_num; i++) {
			subset_seq[i].vertex_num = 24;
			subset_seq[i].primitive_num = 12;
			subset_seq[i].min_index = i * 24;
			subset_seq[i].start_index = i * 24;

			lwGetBoxVertLineList(&vert_buf[i * 24], &_obj_seq[i].box);
		}

		DWORD c = 0xffff0000;
		DWORD* color_buf = LW_NEW(DWORD[ vert_num ]);
		for (DWORD i = 0; i < vert_num; i++) {
			color_buf[i] = c;
		}

		LW_RESULT ret = lwLoadPrimitiveLineList(_obj, "HelperBox", vert_num, vert_buf, color_buf, subset_seq, _obj_num);

		LW_DELETE_A(vert_buf);
		LW_DELETE_A(color_buf);
		LW_DELETE_A(subset_seq);

		return ret;
	}

	LW_RESULT lwBoundingBox::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
									 const lwMatrix44* mat_parent) {
		LW_RESULT ret = LW_RET_FAILED;

		lwMatrix44 mat;

		lwPickInfo u, v;
		v.obj_id = LW_INVALID_INDEX;

		if (_obj_num == 0)
			goto __ret;


		for (DWORD j = 0; j < _obj_num; j++) {
			lwBox box = _obj_seq[j].box;

			lwMatrix44Multiply(&mat, &_obj_seq[j].mat, mat_parent);

			lwVector3 scale = getScale(mat);
			box.r.x *= scale.x;
			box.r.y *= scale.y;
			box.r.z *= scale.z;


			if (LW_RESULT r = lwHitTestBox(&u, org, ray, &box, &mat); LW_FAILED(r)) {
				//ToLogService("errors", LogLevel::Error,
				//			 "[{}] lwHitTestBox failed: j={}, ret={}",
				//			 __FUNCTION__, j, static_cast<long long>(r));
				continue;
			}

			if (v.obj_id == LW_INVALID_INDEX || v.dis > u.dis) {
				v = u;
				v.obj_id = _obj_seq[j].id;
			}
		}


		if (v.obj_id == LW_INVALID_INDEX)
			goto __ret;

		*info = v;

		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwBoundingBox::Render() {
		if (_obj == 0 || _visible_flag == 0)
			return LW_RET_OK;


		if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Update failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}

		lwMatrix44 mat;


		for (DWORD i = 0; i < _obj_num; i++) {
			lwMatrix44Multiply(&mat, &_obj_seq[i].mat, &_mat_parent);

			_obj->SetMatrixParent(&mat);

			if (LW_RESULT r = _obj->RenderSubset(i); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->RenderSubset failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwBoundingBox::Clone(const lwBoundingBox* src) {
		if (src->IsValidObject() == 0)
			return LW_RET_OK;

		_obj_num = src->_obj_num;
		_obj_seq = LW_NEW(lwBoundingBoxInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &src->_obj_seq[0], sizeof(lwBoundingBoxInfo) * _obj_num);

		_res_mgr = src->_res_mgr;
		_visible_flag = src->_visible_flag;

		if (src->_obj) {
			src->_obj->Clone(&_obj);
		}

		return LW_RET_OK;
	}

	lwBoundingBoxInfo* lwBoundingBox::GetDataInfoWithID(DWORD obj_id) {
		for (DWORD i = 0; i < _obj_num; i++) {
			if (_obj_seq[i].id == obj_id)
				return &_obj_seq[i];
		}

		return NULL;
	}

	// lwBoundingSphere
	LW_STD_IMPLEMENTATION(lwBoundingSphere)

	lwBoundingSphere::lwBoundingSphere()
		: _obj_seq(0), _obj_num(0), _obj(0), _data(0) {
	}

	lwBoundingSphere::~lwBoundingSphere() {
		LW_SAFE_DELETE_A(_obj_seq);
		LW_IF_RELEASE(_obj);
	}

	LW_RESULT lwBoundingSphere::SetDataInfo(const lwBoundingSphereInfo* obj_seq, DWORD obj_num) {
		_obj_num = obj_num;
		_obj_seq = LW_NEW(lwBoundingSphereInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &obj_seq[0], sizeof(lwBoundingSphereInfo) * _obj_num);

		return LW_RET_OK;
	}

	LW_RESULT lwBoundingSphere::CreateInstance() {
		if (_obj_num == 0)
			return LW_RET_FAILED;

		const DWORD default_ring = 4;
		const DWORD default_seg = 4;
		const DWORD default_sphere_vert_num = default_ring * default_seg * 2 * 2;

		DWORD vert_num = _obj_num * default_sphere_vert_num;

		lwVector3* vert_buf = LW_NEW(lwVector3[ vert_num ]);

		lwSubsetInfo* subset_seq = LW_NEW(lwSubsetInfo[ _obj_num ]);

		int index;
		float xo, yo, zo, ro;

		DWORD i, j, k;

		float seg_ang = LW_2_PI / default_seg;
		float rin_ang = LW_PI / default_ring;
		float radius;

		lwVector3* sp_buf = LW_NEW(lwVector3[ ( default_ring + 1 ) * ( default_seg + 1 ) ]);

		for (k = 0; k < _obj_num; k++) {
			radius = _obj_seq[k].sphere.r;

			for (i = 0; i <= default_ring; i++) {
				ro = radius * sinf(i * rin_ang);
				yo = radius * cosf(i * rin_ang);

				for (DWORD j = 0; j <= default_seg; j++) {
					xo = ro * cosf(j * (-seg_ang));
					zo = ro * sinf(j * (-seg_ang));

					index = i * (default_seg + 1) + j;

					sp_buf[index].x = xo;
					sp_buf[index].y = yo;
					sp_buf[index].z = zo;
				}
			}

			lwVector3* p = vert_buf + k * default_sphere_vert_num;

			// lat. line list
			for (i = 0; i < default_ring; i++) {
				for (j = 0; j < default_seg; j++) {
					*p++ = sp_buf[i * (default_seg + 1) + j];
					*p++ = sp_buf[i * (default_seg + 1) + j + 1];
				}
			}

			// long. line list
			for (i = 0; i < default_ring; i++) {
				for (j = 0; j < default_seg; j++) {
					*p++ = sp_buf[i * (default_seg + 1) + j];
					*p++ = sp_buf[(i + 1) * (default_seg + 1) + j];
				}
			}

			subset_seq[k].start_index = default_sphere_vert_num * k;
			subset_seq[k].vertex_num = default_sphere_vert_num;
			subset_seq[k].primitive_num = default_sphere_vert_num / 2;
		}

		LW_DELETE_A(sp_buf);

		DWORD c = 0xffff0000;
		DWORD* color_buf = LW_NEW(DWORD[ vert_num ]);
		for (DWORD i = 0; i < vert_num; i++) {
			color_buf[i] = c;
		}

		_res_mgr->CreatePrimitive(&_obj);

		LW_RESULT ret = lwLoadPrimitiveLineList(_obj, "BoundingSphere", vert_num, vert_buf, color_buf, subset_seq,
												_obj_num);

		LW_DELETE_A(vert_buf);
		LW_DELETE_A(color_buf);
		LW_DELETE_A(subset_seq);

		return ret;
	}

	LW_RESULT lwBoundingSphere::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
										const lwMatrix44* mat_parent) {
		LW_RESULT ret = LW_RET_FAILED;

		lwMatrix44 mat;

		lwPickInfo u, v;
		v.obj_id = LW_INVALID_INDEX;

		lwSphere s;

		if (_obj_num == 0)
			goto __ret;


		for (DWORD j = 0; j < _obj_num; j++) {
			s = _obj_seq[j].sphere;

			lwMatrix44Multiply(&mat, &_obj_seq[j].mat, mat_parent);

			lwVec3Mat44Mul(&s.c, &mat);

			lwVector3 scale = getScale(mat);
			s.r *= max3(scale);

			if (lwIntersectRaySphere(&u.pos, &s, org, ray) == 0)
				continue;

			u.dis = lwVector3Magnitude(&u.pos, org);

			if (v.obj_id == LW_INVALID_INDEX || v.dis > u.dis) {
				v = u;
				v.obj_id = _obj_seq[j].id;
			}
		}


		if (v.obj_id == LW_INVALID_INDEX)
			goto __ret;

		*info = v;

		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwBoundingSphere::Render() {
		if (_obj == 0 || _visible_flag == 0)
			return LW_RET_OK;

		if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Update failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}

		lwMatrix44 mat;

		for (DWORD i = 0; i < _obj_num; i++) {
			lwMatrix44Multiply(&mat, &_obj_seq[i].mat, &_mat_parent);

			_obj->SetMatrixParent(&mat);

			if (LW_RESULT r = _obj->RenderSubset(i); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->RenderSubset failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwBoundingSphere::Clone(const lwBoundingSphere* src) {
		if (src->IsValidObject() == 0)
			return LW_RET_OK;

		_obj_num = src->_obj_num;
		_obj_seq = LW_NEW(lwBoundingSphereInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &src->_obj_seq[0], sizeof(lwBoundingSphereInfo) * _obj_num);

		_res_mgr = src->_res_mgr;
		_visible_flag = src->_visible_flag;

		if (src->_obj) {
			src->_obj->Clone(&_obj);
		}

		return LW_RET_OK;
	}

	lwBoundingSphereInfo* lwBoundingSphere::GetDataInfoWithID(DWORD obj_id) {
		for (DWORD i = 0; i < _obj_num; i++) {
			if (_obj_seq[i].id == obj_id)
				return &_obj_seq[i];
		}

		return NULL;
	}

	// lwHelperMesh
	LW_STD_IMPLEMENTATION(lwHelperMesh)

	lwHelperMesh::lwHelperMesh()
		: _obj_seq(0), _obj_num(0), _obj(0) {
	}

	lwHelperMesh::~lwHelperMesh() {
		LW_SAFE_DELETE_A(_obj_seq);
		LW_IF_RELEASE(_obj);
	}

	void lwHelperMesh::SetDataInfo(const lwHelperMeshInfo* obj_seq, DWORD obj_num) {
		_obj_num = obj_num;
		_obj_seq = LW_NEW(lwHelperMeshInfo[ _obj_num ]);

		for (DWORD i = 0; i < _obj_num; i++) {
			_obj_seq[i].Copy(&obj_seq[i]);
		}
	}

	LW_RESULT lwHelperMesh::CreateInstance() {
		if (_obj_num == 0)
			return LW_RET_FAILED;


		_res_mgr->CreatePrimitive(&_obj);

		DWORD i, j;

		lwMeshInfo info;

		DWORD vert_num = 0;
		DWORD index_num = 0;

		for (i = 0; i < _obj_num; i++) {
			vert_num += _obj_seq[i].vertex_num;
			index_num += _obj_seq[i].face_num * 3;
		}

		info.fvf = FVF_XYZDIFFUSE;
		info.pt_type = D3DPT_TRIANGLELIST;
		info.vertex_num = vert_num;
		info.index_num = index_num;
		info.subset_num = _obj_num;
		info.vertex_seq = LW_NEW(lwVector3[ info.vertex_num ]);
		info.vercol_seq = LW_NEW(DWORD[ info.vertex_num ]);
		info.index_seq = LW_NEW(DWORD[ info.index_num ]);
		info.subset_seq = LW_NEW(lwSubsetInfo[info.subset_num]);


		info.subset_seq[0].min_index = 0;
		info.subset_seq[0].start_index = 0;
		info.subset_seq[0].vertex_num = _obj_seq[0].vertex_num;
		info.subset_seq[0].primitive_num = _obj_seq[0].face_num;

		vert_num = _obj_seq[0].vertex_num;
		index_num = _obj_seq[0].face_num * 3;

		for (i = 1; i < _obj_num; i++) {
			info.subset_seq[i].min_index = vert_num;
			info.subset_seq[i].start_index = index_num;
			info.subset_seq[i].vertex_num = _obj_seq[i].vertex_num;
			info.subset_seq[i].primitive_num = _obj_seq[i].face_num;

			vert_num += _obj_seq[i].vertex_num;
			index_num += _obj_seq[i].face_num * 3;
		}

		vert_num = 0;
		for (i = 0; i < _obj_num; i++) {
			memcpy(&info.vertex_seq[vert_num], &_obj_seq[i].vertex_seq[0], sizeof(lwVector3) * _obj_seq[i].vertex_num);
			vert_num += _obj_seq[i].vertex_num;
		}


		lwHelperMeshInfo* hmi;
		DWORD* index_ptr = info.index_seq;

		vert_num = 0;

		for (j = 0; j < _obj_num; j++) {
			hmi = &_obj_seq[j];

			for (i = 0; i < hmi->face_num; i++) {
				*(index_ptr++) = hmi->face_seq[i].vertex[0] + vert_num;
				*(index_ptr++) = hmi->face_seq[i].vertex[1] + vert_num;
				*(index_ptr++) = hmi->face_seq[i].vertex[2] + vert_num;
			}

			vert_num += hmi->vertex_num;
		}

		DWORD c = 0xff0000ff;
		for (i = 0; i < info.vertex_num; i++) {
			info.vercol_seq[i] = c;
		}


		RSA_VALUE(&info.rs_set[0], D3DRS_LIGHTING, FALSE);
		RSA_VALUE(&info.rs_set[1], D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (LW_RESULT r = _obj->LoadMesh(&info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->LoadMesh failed: vertex_num={}, index_num={}, ret={}",
						 __FUNCTION__, info.vertex_num, info.index_num, static_cast<long long>(r));
			return LW_RET_FAILED;
		}

		for (i = 0; i < _obj_num; i++) {
			lwMtlTexInfo mti;
			lwMaterial mtl;

			memset(&mtl, 0, sizeof(mtl));
			mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;

			mti.mtl = mtl;

			if (LW_RESULT r = _obj->LoadMtlTex(i, &mti, std::string_view{}); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->LoadMtlTex failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		lwRenderCtrlCreateInfo rcci;
		lwRenderCtrlCreateInfo_Construct(&rcci);
		rcci.ctrl_id = RENDERCTRL_VS_FIXEDFUNCTION;

		if (LW_RESULT r = _obj->LoadRenderCtrl(&rcci); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->LoadRenderCtrl failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}


		return LW_RET_OK;
	}

	// return value:
	// LW_RET_FAILED_2: no valid helper mesh
	// LW_RET_FAILED: hit test failed
	// LW_RET_OK: hit test succeeded
	LW_RESULT lwHelperMesh::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
									const lwMatrix44* mat_parent, std::string_view type_name) {
		// USE_INVERSE_MAThit test
#ifdef USE_INVERSE_MAT
		////if(xx == 1)

		if (_obj_num == 0)
			return LW_RET_FAILED_2;

		lwMatrix44 mat;
		lwMatrix44 mat_inv;

		lwVector3 org_inv, ray_inv;

		DWORD obj_id = LW_INVALID_INDEX;

		lwPickInfo u, v;
		lwVector3* v_seq[3];
		lwHelperMeshInfo* hmi;

		v.obj_id = LW_INVALID_INDEX;


		DWORD type_obj = 0;

		for (DWORD k = 0; k < _obj_num; k++) {
			if (!type_name.empty()) {
				if (_obj_seq[k].name != type_name)
					continue;
			}

			type_obj += 1;

			hmi = &_obj_seq[k];

			D3DXMatrixMultiply((D3DXMATRIX*)&mat, (D3DXMATRIX*)&hmi->mat, (D3DXMATRIX*)mat_parent);
			D3DXMatrixInverse((D3DXMATRIX*)&mat_inv, NULL, (D3DXMATRIX*)&mat);

			org_inv = *org;
			lwVec3Mat44Mul(&org_inv, &mat_inv);
			lwMatrix44 mat_inv_ray(mat_inv);
			mat_inv_ray._41 = mat_inv_ray._42 = mat_inv_ray._43 = 0.0f;
			ray_inv = *ray;
			lwVec3Mat44Mul(&ray_inv, &mat_inv_ray);


			for (DWORD i = 0; i < hmi->face_num; i++) {
				v_seq[0] = &hmi->vertex_seq[hmi->face_seq[i].vertex[0]];
				v_seq[1] = &hmi->vertex_seq[hmi->face_seq[i].vertex[1]];
				v_seq[2] = &hmi->vertex_seq[hmi->face_seq[i].vertex[2]];

				lwTriangle tri(*v_seq[0], *v_seq[1], *v_seq[2]);

				if (lwIntersectRayTriangle(&u.pos, &u.dis, &org_inv, &ray_inv, &tri)) {
					if ((v.obj_id == LW_INVALID_INDEX) || (v.dis > u.dis)) {
						v = u;
						v.obj_id = k;
						v.data = hmi->id;
						v.face_id = i;
					}
				}
			}

			if (v.obj_id != LW_INVALID_INDEX) {
				lwVec3Mat44Mul(&v.pos, &mat);
			}
		}

		if (type_obj == 0)
			return LW_RET_FAILED_2;

		if (v.obj_id == LW_INVALID_INDEX)
			return LW_RET_FAILED;

		*info = v;


		return LW_RET_OK;

#else

		if (_obj_num == 0)
			return LW_RET_FAILED_2;

		lwMatrix44 mat;

		DWORD obj_id = LW_INVALID_INDEX;

		lwPickInfo u, v;
		lwVector3 v_seq[3];
		lwHelperMeshInfo* hmi;

		v.obj_id = LW_INVALID_INDEX;

		DWORD f;

		DWORD type_obj = 0;

		for (DWORD k = 0; k < _obj_num; k++) {
			if (!type_name.empty()) {
				if (_obj_seq[k].name != type_name)
					continue;
			}

			type_obj += 1;

			hmi = &_obj_seq[k];

			lwMatrix44Multiply(&mat, &hmi->mat, mat_parent);

			for (DWORD i = 0; i < hmi->face_num; i++) {
				for (DWORD j = 0; j < 3; j++) {
					v_seq[j] = hmi->vertex_seq[hmi->face_seq[i].vertex[j]];
					lwVec3Mat44Mul(&v_seq[j], &mat);
				}

				lwTriangle tri(v_seq[0], v_seq[1], v_seq[2]);

				if (lwIntersectRayTriangle(&u.pos, &u.dis, org, ray, &tri)) {
					if ((v.obj_id == LW_INVALID_INDEX) || (v.dis > u.dis)) {
						v = u;
						v.obj_id = k;
						v.data = hmi->id;
						v.face_id = i;
						// debug var
						f = hmi->face_num;
					}
				}
			}
		}

		if (type_obj == 0)
			return LW_RET_FAILED_2;

		if (v.obj_id == LW_INVALID_INDEX)
			return LW_RET_FAILED;

		*info = v;


		return LW_RET_OK;
#endif
	}

	LW_RESULT lwHelperMesh::Render() {
		if (_obj == 0 || _visible_flag == 0)
			return LW_RET_OK;


		if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Update failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}

		for (DWORD i = 0; i < _obj_num; i++) {
			lwMatrix44 mat(_obj_seq[i].mat);
			lwMatrix44Multiply(&mat, &mat, &_mat_parent);

			_obj->SetMatrixParent(&mat);

			if (LW_RESULT r = _obj->RenderSubset(i); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->RenderSubset failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperMesh::Clone(const lwHelperMesh* src) {
		if (src->IsValidObject() == 0)
			return LW_RET_OK;

		_res_mgr = src->_res_mgr;
		_visible_flag = src->_visible_flag;

		if (src->_obj) {
			src->_obj->Clone(&_obj);
		}

		_obj_num = src->_obj_num;
		_obj_seq = LW_NEW(lwHelperMeshInfo[ _obj_num ]);

		for (DWORD i = 0; i < _obj_num; i++) {
			_obj_seq[i].Copy(&src->_obj_seq[i]);
		}

		return LW_RET_OK;
	}

	lwHelperMeshInfo* lwHelperMesh::GetDataInfoWithID(DWORD obj_id) {
		for (DWORD i = 0; i < _obj_num; i++) {
			if (_obj_seq[i].id == obj_id)
				return &_obj_seq[i];
		}

		return NULL;
	}

	// lwHelperBox
	LW_STD_IMPLEMENTATION(lwHelperBox)

	lwHelperBox::lwHelperBox()
		: _obj_seq(0), _obj_num(0), _obj(0) {
	}

	lwHelperBox::~lwHelperBox() {
		LW_SAFE_DELETE_A(_obj_seq);
		LW_IF_RELEASE(_obj);
	}

	LW_RESULT lwHelperBox::SetDataInfo(const lwHelperBoxInfo* obj_seq, DWORD obj_num) {
		LW_SAFE_DELETE_A(_obj_seq);

		_obj_num = obj_num;
		_obj_seq = LW_NEW(lwHelperBoxInfo[ _obj_num ]);
		memcpy(&_obj_seq[0], &obj_seq[0], sizeof(lwHelperBoxInfo) * _obj_num);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperBox::CreateInstance() {
		if (_obj_num == 0)
			return LW_RET_FAILED;

		_res_mgr->CreatePrimitive(&_obj);

		DWORD vert_num = _obj_num * 24;
		lwVector3* vert_buf = LW_NEW(lwVector3[ vert_num ]);

		lwSubsetInfo* subset_seq = LW_NEW(lwSubsetInfo[ _obj_num ]);

		for (DWORD i = 0; i < _obj_num; i++) {
			subset_seq[i].vertex_num = 24;
			subset_seq[i].primitive_num = 12;
			subset_seq[i].min_index = i * 24;
			subset_seq[i].start_index = i * 24;

			lwGetBoxVertLineList(&vert_buf[i * 24], &_obj_seq[i].box);
		}

		DWORD c = 0xff0000ff;
		DWORD* color_buf = LW_NEW(DWORD[ vert_num ]);
		for (DWORD i = 0; i < vert_num; i++) {
			color_buf[i] = c;
		}

		LW_RESULT ret = lwLoadPrimitiveLineList(_obj, "HelperBox", vert_num, vert_buf, color_buf, subset_seq, _obj_num);

		LW_DELETE_A(vert_buf);
		LW_DELETE_A(color_buf);
		LW_DELETE_A(subset_seq);

		return ret;
	}

	LW_RESULT lwHelperBox::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
								   const lwMatrix44* mat_parent, std::string_view type_name) {
		LW_RESULT ret = LW_RET_FAILED;

		lwMatrix44 mat;

		lwPickInfo u, v;
		v.obj_id = LW_INVALID_INDEX;

		if (_obj_num == 0)
			goto __ret;


		for (DWORD j = 0; j < _obj_num; j++) {
			if (!type_name.empty()) {
				if (_obj_seq[j].name != type_name)
					continue;
			}

			lwMatrix44Multiply(&mat, &_obj_seq[j].mat, mat_parent);

			if (LW_RESULT r = lwHitTestBox(&u, org, ray, &_obj_seq[j].box, &mat); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwHitTestBox failed: j={}, ret={}",
							 __FUNCTION__, j, static_cast<long long>(r));
				continue;
			}

			if (v.obj_id == LW_INVALID_INDEX || v.dis > u.dis) {
				v = u;
				v.obj_id = _obj_seq[j].id;
			}
		}


		if (v.obj_id == LW_INVALID_INDEX)
			goto __ret;

		*info = v;

		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwHelperBox::Render() {
		if (_obj == 0 || _visible_flag == 0)
			return LW_RET_OK;


		if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Update failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}

		for (DWORD i = 0; i < _obj_num; i++) {
			lwMatrix44 mat(_obj_seq[i].mat);
			lwMatrix44Multiply(&mat, &mat, &_mat_parent);

			_obj->SetMatrixParent(&mat);

			if (LW_RESULT r = _obj->RenderSubset(i); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->RenderSubset failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	lwHelperBoxInfo* lwHelperBox::GetDataInfoWithID(DWORD obj_id) {
		for (DWORD i = 0; i < _obj_num; i++) {
			if (_obj_seq[i].id == obj_id)
				return &_obj_seq[i];
		}

		return NULL;
	}

	LW_RESULT lwHelperBox::Clone(const lwHelperBox* src) {
		if (src->IsValidObject() == 0)
			return LW_RET_OK;

		_obj_num = src->_obj_num;
		_obj_seq = LW_NEW(lwHelperBoxInfo[ _obj_num ]);

		for (DWORD i = 0; i < _obj_num; i++) {
			_obj_seq[i].Copy(&src->_obj_seq[i]);
		}

		_res_mgr = src->_res_mgr;
		_visible_flag = src->_visible_flag;

		if (src->_obj) {
			src->_obj->Clone(&_obj);
		}


		return LW_RET_OK;
	}


	// lwHelperObject
	LW_STD_IMPLEMENTATION(lwHelperObject)

	lwHelperObject::lwHelperObject(lwIResourceMgr* mgr)
		: _res_mgr(mgr), _obj_dummy(0), _obj_box(0), _obj_mesh(0), _obj_boundingbox(0), _obj_boundingsphere(0) {
	}

	lwHelperObject::~lwHelperObject() {
		LW_SAFE_DELETE(_obj_dummy);
		LW_SAFE_DELETE(_obj_box);
		LW_SAFE_DELETE(_obj_mesh);
		LW_SAFE_DELETE(_obj_boundingbox);
		LW_SAFE_DELETE(_obj_boundingsphere);
	}

	LW_RESULT lwHelperObject::LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag) {
		LW_RESULT ret = LW_RET_FAILED;

		if (info->type & HELPER_TYPE_DUMMY) {
			LW_SAFE_DELETE(_obj_dummy);

			_obj_dummy = LW_NEW(lwHelperDummy);

			_obj_dummy->SetResourceMgr(_res_mgr);
			_obj_dummy->SetDataInfo(&info->dummy_seq[0], info->dummy_num);

			if (create_instance_flag) {
				if (LW_RESULT r = _obj_dummy->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj_dummy->CreateInstance failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

		if (info->type & HELPER_TYPE_BOX) {
			LW_SAFE_DELETE(_obj_box);

			_obj_box = LW_NEW(lwHelperBox);

			_obj_box->SetResourceMgr(_res_mgr);
			_obj_box->SetDataInfo(&info->box_seq[0], info->box_num);

			if (create_instance_flag) {
				if (LW_RESULT r = _obj_box->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj_box->CreateInstance failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

		if (info->type & HELPER_TYPE_MESH) {
			LW_SAFE_DELETE(_obj_mesh);

			_obj_mesh = LW_NEW(lwHelperMesh);

			_obj_mesh->SetResourceMgr(_res_mgr);
			_obj_mesh->SetDataInfo(&info->mesh_seq[0], info->mesh_num);

			if (create_instance_flag) {
				if (LW_RESULT r = _obj_mesh->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj_mesh->CreateInstance failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

		if (info->type & HELPER_TYPE_BOUNDINGBOX) {
			LW_SAFE_DELETE(_obj_boundingbox);

			_obj_boundingbox = LW_NEW(lwBoundingBox);

			_obj_boundingbox->SetResourceMgr(_res_mgr);
			_obj_boundingbox->SetDataInfo(&info->bbox_seq[0], info->bbox_num);

			if (create_instance_flag) {
				if (LW_RESULT r = _obj_boundingbox->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj_boundingbox->CreateInstance failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

		if (info->type & HELPER_TYPE_BOUNDINGSPHERE) {
			LW_SAFE_DELETE(_obj_boundingsphere);

			_obj_boundingsphere = LW_NEW(lwBoundingSphere);

			_obj_boundingsphere->SetResourceMgr(_res_mgr);
			_obj_boundingsphere->SetDataInfo(&info->bsphere_seq[0], info->bsphere_num);

			if (create_instance_flag) {
				if (LW_RESULT r = _obj_boundingsphere->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj_boundingsphere->CreateInstance failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwHelperObject::Copy(const lwIHelperObject* src) {
		lwHelperObject* s = (lwHelperObject*)src;

		if (s->_obj_dummy) {
			_obj_dummy = LW_NEW(lwHelperDummy);
			_obj_dummy->Clone(s->_obj_dummy);
		}
		if (s->_obj_box) {
			_obj_box = LW_NEW(lwHelperBox);
			_obj_box->Clone(s->_obj_box);
		}
		if (s->_obj_mesh) {
			_obj_mesh = LW_NEW(lwHelperMesh);
			_obj_mesh->Clone(s->_obj_mesh);
		}
		if (s->_obj_boundingbox) {
			_obj_boundingbox = LW_NEW(lwBoundingBox);
			_obj_boundingbox->Clone(s->_obj_boundingbox);
		}
		if (s->_obj_boundingsphere) {
			_obj_boundingsphere = LW_NEW(lwBoundingSphere);
			_obj_boundingsphere->Clone(s->_obj_boundingsphere);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperObject::Clone(lwIHelperObject** ret_obj) {
		lwIHelperObject* o;
		_res_mgr->CreateHelperObject(&o);
		o->Copy(this);
		*ret_obj = o;
		return LW_RET_OK;
	}

	void lwHelperObject::SetParentMatrix(const lwMatrix44* mat) {
		if (_obj_dummy) {
			_obj_dummy->SetMatrixParent(mat);
		}
		if (_obj_box) {
			_obj_box->SetMatrixParent(mat);
		}
		if (_obj_mesh) {
			_obj_mesh->SetMatrixParent(mat);
		}
		if (_obj_boundingbox) {
			_obj_boundingbox->SetMatrixParent(mat);
		}
		if (_obj_boundingsphere) {
			_obj_boundingsphere->SetMatrixParent(mat);
		}
	}

	LW_RESULT lwHelperObject::Update() {
		return LW_RET_OK;
	}

	LW_RESULT lwHelperObject::Render() {
		if (_obj_dummy) {
			if (LW_RESULT r = _obj_dummy->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_dummy->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
		if (_obj_box) {
			if (LW_RESULT r = _obj_box->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_box->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
		if (_obj_mesh) {
			if (LW_RESULT r = _obj_mesh->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_mesh->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
		if (_obj_boundingbox) {
			if (LW_RESULT r = _obj_boundingbox->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_boundingbox->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
		if (_obj_boundingsphere) {
			if (LW_RESULT r = _obj_boundingsphere->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_boundingsphere->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	void lwHelperObject::SetVisible(int flag) {
		if (_obj_dummy) {
			_obj_dummy->SetVisible(flag);
		}
		if (_obj_box) {
			_obj_box->SetVisible(flag);
		}
		if (_obj_mesh) {
			_obj_mesh->SetVisible(flag);
		}
		if (_obj_boundingbox) {
			_obj_boundingbox->SetVisible(flag);
		}
		if (_obj_boundingsphere) {
			_obj_boundingsphere->SetVisible(flag);
		}
	}

	LW_RESULT lwHelperObject::ExtractHelperInfo(lwHelperInfo* out_info) {
		lwHelperInfo* a = (lwHelperInfo*)out_info;

		if (_obj_dummy) {
			a->type |= HELPER_TYPE_DUMMY;
			a->dummy_num = _obj_dummy->GetObjNum();
			a->dummy_seq = LW_NEW(lwHelperDummyInfo[a->dummy_num]);
			memcpy(a->dummy_seq, _obj_dummy->GetDataInfo(0), sizeof(lwHelperDummyInfo) * a->dummy_num);
		}
		if (_obj_box) {
			a->type |= HELPER_TYPE_BOX;
			a->box_num = _obj_box->GetObjNum();
			a->box_seq = LW_NEW(lwHelperBoxInfo[a->box_num]);
			memcpy(a->box_seq, _obj_box->GetDataInfo(0), sizeof(lwHelperBoxInfo) * a->box_num);
		}
		if (_obj_mesh) {
			a->type |= HELPER_TYPE_MESH;
			a->mesh_num = _obj_mesh->GetObjNum();
			a->mesh_seq = LW_NEW(lwHelperMeshInfo[a->mesh_num]);
			for (DWORD i = 0; i < a->mesh_num; i++) {
				a->mesh_seq[i].Copy(_obj_mesh->GetDataInfo(i));
			}
		}
		if (_obj_boundingbox) {
			a->type |= HELPER_TYPE_BOUNDINGBOX;
			a->bbox_num = _obj_boundingbox->GetObjNum();
			a->bbox_seq = LW_NEW(lwBoundingBoxInfo[a->bbox_num]);
			memcpy(a->bbox_seq, _obj_boundingbox->GetDataInfo(0), sizeof(lwBoundingBoxInfo) * a->bbox_num);
		}
		if (_obj_boundingsphere) {
			a->type |= HELPER_TYPE_BOUNDINGSPHERE;
			a->bsphere_num = _obj_boundingsphere->GetObjNum();
			a->bsphere_seq = LW_NEW(lwBoundingSphereInfo[a->bsphere_num]);
			memcpy(a->bsphere_seq, _obj_boundingsphere->GetDataInfo(0), sizeof(lwBoundingSphereInfo) * a->bsphere_num);
		}

		return LW_RET_OK;
	}


LW_END
