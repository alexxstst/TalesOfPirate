//
#include "stdafx.h"
#include "lwSceneMgr.h"

LW_BEGIN
	static int __qsort_transparent_primitive(const void* e1, const void* e2) {
		return ((lwSortPriInfo*)e1)->d < ((lwSortPriInfo*)e2)->d;
	}

	//lwSceneMgr
	LW_STD_IMPLEMENTATION(lwSceneMgr)

	lwSceneMgr::lwSceneMgr(lwISysGraphics* sys_graphics)
		: _sys_graphics(sys_graphics), _vf(0)
		  , _sort_obj_seq(0), _sort_obj_seq_size(0), _sort_obj_num(0), _max_sort_num(0) {
		_vf = LW_NEW(lwViewFrustum);

		_sort_obj_seq_size = 1024;
		_sort_obj_seq = LW_NEW(lwSortPriInfo[_sort_obj_seq_size]);

		_sppi.proc = 0;
		_sppi.param = 0;

		memset(&_frame_info, 0, sizeof(_frame_info));
	}

	lwSceneMgr::~lwSceneMgr() {
		_vf->Release();
		LW_IF_DELETE_A(_sort_obj_seq);
	}

	LW_RESULT lwSceneMgr::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		_frame_info._update_count++;

		return ret;
	}

	LW_RESULT lwSceneMgr::BeginRender() {
		LW_RESULT ret = LW_RET_FAILED;

		_frame_info._render_count++;

		_cull_pri_info.total_cnt = 0;
		_cull_pri_info.culling_cnt = 0;

		lwIDeviceObject* dev_obj = _sys_graphics->GetDeviceObject();

		_vf->Update(dev_obj->GetMatViewProj());

		return ret;
	}

	LW_RESULT lwSceneMgr::EndRender() {
		LW_RESULT ret = LW_RET_FAILED;

		return ret;
	}


	LW_RESULT lwSceneMgr::Render() {
		return LW_RET_OK;
	}

	LW_RESULT lwSceneMgr::RenderTransparentPrimitive() {
		LW_RESULT ret = LW_RET_FAILED;

		lwSortPriInfo* spi;
		for (DWORD i = 0; i < _sort_obj_num; i++) {
			spi = &_sort_obj_seq[i];
			if (spi->sppi.proc) {
				(*spi->sppi.proc)(spi->obj, spi->sppi.param);
			}
			else {
				if (LW_RESULT r = spi->obj->Render(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwIPrimitive::Render failed: idx={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
				}
			}
		}

		if (_sort_obj_num > _max_sort_num) {
			_max_sort_num = _sort_obj_num;
		}

		_sort_obj_num = 0;

		return ret;
	}

	LW_RESULT lwSceneMgr::AddTransparentPrimitive(lwIPrimitive* obj) {
		if (_sort_obj_num == _sort_obj_seq_size) {
			DWORD new_size = _sort_obj_seq_size * 2;
			lwSortPriInfo* new_seq = LW_NEW(lwSortPriInfo[new_size]);
			memcpy(new_seq, _sort_obj_seq, sizeof(lwSortPriInfo) * _sort_obj_num);

			LW_DELETE_A(_sort_obj_seq);

			_sort_obj_seq_size = new_size;
			_sort_obj_seq = new_seq;
		}

		lwSortPriInfo* spi = &_sort_obj_seq[_sort_obj_num++];
		spi->obj = obj;
		spi->sppi = _sppi;

		return LW_RET_OK;
	}


	LW_RESULT lwSceneMgr::SortTransparentPrimitive() {
		lwPlane* p = _vf->GetPlane(VIEWFRUSTUM_FRONT);
		lwSortPriInfo* o;

		for (DWORD i = 0; i < _sort_obj_num; i++) {
			o = &_sort_obj_seq[i];
			o->d = lwPlaneClassify(p, (lwVector3*)&o->obj->GetMatrixGlobal()->_41);
		}

		// now use quick sort algorithm
		qsort(_sort_obj_seq, _sort_obj_num, sizeof(lwSortPriInfo), __qsort_transparent_primitive);

		return LW_RET_OK;
	}

	LW_RESULT lwSceneMgr::CullPrimitive(lwIPrimitive* obj) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD d = 0;
		lwMatrix44 mat;
		lwBox box;

		lwIHelperObject* hb = obj->GetHelperObject();
		if (hb == 0) {
			ret = LW_RET_FAILED_2;
			goto __ret;
		}
		{
			lwIBoundingSphere* bs = hb->GetBoundingSphere();
			lwIBoundingBox* bb = hb->GetBoundingBox();

			if ((bs == 0) && (bb == 0)) {
				ret = LW_RET_FAILED_2;
				goto __ret;
			}

			if (bs) {
				lwSphere s;
				lwBoundingSphereInfo* bsi;
				DWORD bs_num = bs->GetObjNum();
				for (DWORD i = 0; i < bs_num; i++) {
					bsi = bs->GetDataInfo(i);
					lwMatrix44Multiply(&mat, &bsi->mat, bs->GetMatrixParent());

					s = bsi->sphere;
					lwVec3Mat44Mul(&s.c, &mat);

					d = bs->GetData();
					ret = _vf->IsSphereInFrustum(&s, &d) ? LW_RET_FAILED : LW_RET_OK;
					if (LW_SUCCEEDED(ret)) {
						bs->SetData(d);
						break;
					}
				}
			}
			else {
				lwBoundingBoxInfo* bbi = bb->GetDataInfo(0);
				if (bbi == 0) {
					ret = LW_RET_FAILED_2;
					goto __ret;
				}

				box = bbi->box;
				if (box.r.x < 0.0f) {
					box.r.x = -box.r.x;
				}
				if (box.r.y < 0.0f) {
					box.r.y = -box.r.y;
				}
				if (box.r.z < 0.0f) {
					box.r.z = -box.r.z;
				}

				lwMatrix44Multiply(&mat, &bbi->mat, bb->GetMatrixParent());

				d = bb->GetData();

				// begin defined
#define SPHERE_CP

#if(defined BOX_CP)
				ret = _vf->IsBoxInFrustum(&box, &mat, &d) ? LW_RET_FAILED : LW_RET_OK;
#endif

#if(defined SPHERE_CP)
				{
					lwSphere s;


					s.c = box.c;
					lwVec3Mat44Mul(&s.c, &mat);
					s.r = lwVector3Magnitude(&box.r);


					ret = _vf->IsSphereInFrustum(&s, &d) ? LW_RET_FAILED : LW_RET_OK;
				}
#endif

#undef SPHERE_CP

				bb->SetData(d);
			}
		}

		// end

	__ret:
		_cull_pri_info.total_cnt += 1;
		if (ret == LW_RET_OK) {
			_cull_pri_info.culling_cnt++;
		}

		return ret;
	}

LW_END
