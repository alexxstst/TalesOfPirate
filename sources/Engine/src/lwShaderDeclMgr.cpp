#include "stdafx.h"
#include "lwShaderDeclMgr.h"

LW_BEGIN
	LW_STD_IMPLEMENTATION(lwShaderDeclMgr)


	lwShaderDeclMgr::lwShaderDeclMgr(lwIShaderMgr* shader_mgr)
		: _shader_mgr(shader_mgr) {
		memset(_decl_set, 0, sizeof(lwShaderDeclSet*) * SHADER_DECL_NUM);
	}

	lwShaderDeclMgr::~lwShaderDeclMgr() {
		for (DWORD i = 0; i < SHADER_DECL_NUM; i++) {
			LW_SAFE_DELETE(_decl_set[ i ]);
		}
	}


	LW_RESULT lwShaderDeclMgr::CreateShaderDeclSet(DWORD decl_type, DWORD buf_size) {
		LW_RESULT ret = LW_RET_FAILED;

		if (decl_type < 0 || decl_type >= SHADER_DECL_NUM)
			goto __ret;

		if (_decl_set[decl_type])
			goto __ret;

		_decl_set[decl_type] = LW_NEW(lwShaderDeclSet( decl_type, buf_size ));

		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwShaderDeclMgr::SetShaderDeclInfo(lwShaderDeclCreateInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		if (info->decl_type < 0 || info->decl_type >= SHADER_DECL_NUM)
			goto __ret;

		if (_decl_set[info->decl_type] == NULL)
			goto __ret;

		{
			lwShaderDeclSet* s = _decl_set[info->decl_type];

			DWORD i = 0;
			for (; i < s->decl_num; i++) {
				if (s->decl_seq[i].shader_id == LW_INVALID_INDEX)
					break;
			}

			if (i == s->decl_num)
				goto __ret;

			{
				lwShaderDeclInfo* d = &s->decl_seq[i];
				d->shader_id = info->shader_id;
				d->light_type = info->light_type;
				d->anim_type = info->anim_type;
				_tcscpy(d->file, info->file);

				ret = LW_RET_OK;
			}
		}
	__ret:
		return ret;
	}

	LW_RESULT lwShaderDeclMgr::QueryShaderHandle(DWORD* shader_handle, const lwShaderDeclQueryInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		if (info->decl_type < 0 || info->decl_type >= SHADER_DECL_NUM)
			goto __ret;

		if (_decl_set[info->decl_type] == NULL)
			goto __ret;

		{
			lwShaderDeclInfo* d;
			lwShaderDeclSet* s = _decl_set[info->decl_type];

			DWORD i = 0;
			for (; i < s->decl_num; i++) {
				d = &s->decl_seq[i];
				if ((d->light_type == info->light_type) && (d->anim_type == info->anim_type))
					break;
			}

			if (i == s->decl_num)
				goto __ret;

			*shader_handle = d->shader_id;

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

LW_END
