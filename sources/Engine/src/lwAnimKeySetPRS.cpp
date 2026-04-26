//
#include "stdafx.h"

#include "lwAnimKeySetPRS.h"


LW_BEGIN
	// lwAnimKeySetPRS2
	template <class T>
	LW_RESULT lwKeyDataSearch(DWORD* ret_min, DWORD* ret_max, DWORD key, T* data_seq, DWORD data_num) {
		DWORD low = 0;
		DWORD high = data_num - 1;
		DWORD k, l;

		if (data_num == 0) {
			*ret_min = 0;
			*ret_max = 0;

			return LW_RET_OK;
		}

		while (high >= low) {
			k = (low + high) / 2;

			l = k + 1;

			if (l == data_num) {
				*ret_min = k;
				*ret_max = k;
				break;
			}
			else {
				if (key >= data_seq[k].key && key < data_seq[l].key) {
					*ret_min = k;
					*ret_max = l;
					break;
				}
			}

			if (key < data_seq[k].key) {
				high = k - 1;
			}
			else {
				low = k + 1;
			}
		}

		return (low <= high) ? LW_RET_OK : LW_INVALID_INDEX;
	}

	LW_RESULT lwGetKeySlerpCoefficent(float* out, DWORD slerp_type, float key, float k0, float k1) {
		float t = 0.0f;

		t = (float)(key - k0) / (float)(k1 - k0);

		switch (slerp_type) {
		case AKST_LINEAR:
			break;
		case AKST_SIN1: // 0 - 90
			t = sinf(t * LW_HALF_PI);
			break;
		case AKST_SIN2: // 90 - 180
			t = sinf(LW_HALF_PI + t * LW_HALF_PI);
			break;
		case AKST_SIN3: // 180 - 270
			t = sinf(LW_PI + t * LW_HALF_PI);
			break;
		case AKST_SIN4: // 270 - 360
			t = sinf(LW_HALF_PI * 3 + t * LW_HALF_PI);
			break;
		case AKST_COS1: // 0 - 90
			t = cosf(t * LW_HALF_PI);
			break;
		case AKST_COS2: // 90 - 180
			t = cosf(LW_HALF_PI + t * LW_HALF_PI);
			break;
		case AKST_COS3: // 180 - 270
			t = cosf(LW_PI + t * LW_HALF_PI);
			break;
		case AKST_COS4: // 270 - 360
			t = cosf(LW_HALF_PI * 3 + t * LW_HALF_PI);
			break;
		case AKST_TAN1: // 0 - 45
			t = tanf(t * LW_HALF_PI * 0.5f);
			break;
		default:
			return LW_RET_FAILED;
		}

		*out = t;

		return LW_RET_OK;
	}


	LW_STD_IMPLEMENTATION(lwAnimKeySetPRS2)

	void lwAnimKeySetPRS2::_ResetFrameNum() {
		DWORD f = 0;

		if (_pos_num && _pos_seq[_pos_num - 1].key > f) {
			f = _pos_seq[_pos_num - 1].key;
		}
		if (_rot_num && _rot_seq[_rot_num - 1].key > f) {
			f = _rot_seq[_rot_num - 1].key;
		}
		if (_sca_num && _sca_seq[_sca_num - 1].key > f) {
			f = _sca_seq[_sca_num - 1].key;
		}

		_frame_num = f;
	}

	LW_RESULT lwAnimKeySetPRS2::GetValue(lwMatrix44* mat, float frame) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD f = (DWORD)frame;
		if ((float)f != frame) {
			f += 1;
		}

		lwVector3 pos;
		lwQuaternion quat;
		lwVector3 scale;
		float t;

		lwMatrix44 mat_pos;
		lwMatrix44 mat_rot;
		lwMatrix44 mat_sca;

		if (f < 0 || f >= _frame_num)
			goto __ret;

		{
			DWORD key_pos[2];
			DWORD key_rot[2];
			DWORD key_sca[2];

			if (_pos_num > 0) {
				if (LW_RESULT r = lwKeyDataSearch<lwKeyVector3>(&key_pos[0], &key_pos[1], f, _pos_seq, _pos_num);
					LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwKeyDataSearch<pos> failed: f={}, pos_num={}, ret={}",
								 __FUNCTION__, f, _pos_num, static_cast<long long>(r));
					assert(0);
					goto __ret;
				}
				if (key_pos[0] == key_pos[1]) {
					pos = _pos_seq[key_pos[0]].data;
				}
				else {
					lwKeyVector3* k0 = &_pos_seq[key_pos[0]];
					lwKeyVector3* k1 = &_pos_seq[key_pos[1]];

					lwGetKeySlerpCoefficent(&t, k0->slerp_type, frame, (float)k0->key, (float)k1->key);

					lwVector3Slerp(&pos, &k0->data, &k1->data, t);
				}

				mat_pos = lwMatrix44Translate(pos);
			}

			if (_rot_num > 0) {
				if (LW_RESULT r = lwKeyDataSearch<lwKeyQuaternion>(&key_rot[0], &key_rot[1], f, _rot_seq, _rot_num);
					LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwKeyDataSearch<rot> failed: f={}, rot_num={}, ret={}",
								 __FUNCTION__, f, _rot_num, static_cast<long long>(r));
					assert(0);
					goto __ret;
				}

				if (key_rot[0] == key_rot[1]) {
					quat = _rot_seq[key_rot[0]].data;
				}
				else {
					lwKeyQuaternion* k0 = &_rot_seq[key_rot[0]];
					lwKeyQuaternion* k1 = &_rot_seq[key_rot[1]];

					lwGetKeySlerpCoefficent(&t, k0->slerp_type, frame, (float)k0->key, (float)k1->key);

					lwQuaternionSlerp(&quat, &k0->data, &k1->data, t);
				}

				lwQuaternionToMatrix44(&mat_rot, &quat);
			}

			if (_sca_num > 0) {
				if (LW_RESULT r = lwKeyDataSearch<lwKeyVector3>(&key_sca[0], &key_sca[1], f, _sca_seq, _sca_num);
					LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwKeyDataSearch<sca> failed: f={}, sca_num={}, ret={}",
								 __FUNCTION__, f, _sca_num, static_cast<long long>(r));
					assert(0);
					goto __ret;
				}


				if (key_sca[0] == key_sca[1]) {
					scale = _sca_seq[key_sca[0]].data;
				}
				else {
					lwKeyVector3* k0 = &_sca_seq[key_sca[0]];
					lwKeyVector3* k1 = &_sca_seq[key_sca[1]];

					lwGetKeySlerpCoefficent(&t, k0->slerp_type, frame, (float)k0->key, (float)k1->key);

					lwVector3Slerp(&scale, &k0->data, &k1->data, t);
				}

				mat_sca = lwMatrix44Scale(scale);
			}

			const auto matrix = lwMatrix44Translate(pos);
			lwMatrix44Multiply(mat, mat, &matrix);


			lwMatrix44Identity(mat);

			if (_pos_num > 0) {
				lwMatrix44Multiply(mat, &mat_pos, mat);
			}
			if (_rot_num > 0) {
				lwMatrix44Multiply(mat, &mat_rot, mat);
			}
			if (_sca_num > 0) {
				lwMatrix44Multiply(mat, &mat_sca, mat);
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetPRS2::AddKeyPosition(DWORD id, const lwKeyVector3* data, DWORD num) {
		lwKeyVector3* buf = LW_NEW(lwKeyVector3[_pos_num + num]);

		if (_pos_seq) {
			memcpy(buf, _pos_seq, sizeof(lwKeyVector3) * id);
		}

		memcpy(&buf[id], data, sizeof(lwKeyVector3) * num);

		if (_pos_num > id) {
			memcpy(&buf[id + num], &_pos_seq[id], sizeof(lwKeyVector3) * (_pos_num - id));
		}

		LW_IF_DELETE_A(_pos_seq);

		_pos_seq = buf;
		_pos_num += num;

		_ResetFrameNum();

		return LW_RET_OK;
	}

	LW_RESULT lwAnimKeySetPRS2::AddKeyRotation(DWORD id, const lwKeyQuaternion* data, DWORD num) {
		lwKeyQuaternion* buf = LW_NEW(lwKeyQuaternion[_rot_num + num]);

		if (_rot_seq) {
			memcpy(buf, _rot_seq, sizeof(lwKeyQuaternion) * id);
		}

		memcpy(&buf[id], data, sizeof(lwKeyQuaternion) * num);

		if (_rot_num - id > 0) {
			memcpy(&buf[id + num], &_rot_seq[id], sizeof(lwKeyQuaternion) * (_rot_num - id));
		}

		LW_IF_DELETE_A(_rot_seq);

		_rot_seq = buf;
		_rot_num += num;

		_ResetFrameNum();

		return LW_RET_OK;
	}

	LW_RESULT lwAnimKeySetPRS2::AddKeyScale(DWORD id, const lwKeyVector3* data, DWORD num) {
		lwKeyVector3* buf = LW_NEW(lwKeyVector3[_sca_num + num]);

		if (_sca_seq) {
			memcpy(buf, _sca_seq, sizeof(lwKeyVector3) * id);
		}

		memcpy(&buf[id], data, sizeof(lwKeyVector3) * num);

		if (_sca_num - id > 0) {
			memcpy(&buf[id + num], &_sca_seq[id], sizeof(lwKeyVector3) * (_sca_num - id));
		}

		LW_IF_DELETE_A(_sca_seq);

		_sca_seq = buf;
		_sca_num += num;

		_ResetFrameNum();

		return LW_RET_OK;
	}

	LW_RESULT lwAnimKeySetPRS2::DelKeyPosition(DWORD id, DWORD num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (num == 0xffffffff) {
			LW_SAFE_DELETE_A(_pos_seq);
			_pos_num = 0;
			goto __addr_ret_ok;
		}

		if ((id + num) > _pos_num)
			goto __ret;

		{
			lwKeyVector3* buf = LW_NEW(lwKeyVector3[_pos_num - num]);

			memcpy(&buf[0], &_pos_seq[0], sizeof(lwKeyVector3) * id);
			memcpy(&buf[id], &_pos_seq[id + num], sizeof(lwKeyVector3) * (_pos_num - id - num));

			_pos_seq = buf;
			_pos_num -= num;
		}

	__addr_ret_ok:
		_ResetFrameNum();
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetPRS2::DelKeyRotation(DWORD id, DWORD num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (num == 0xffffffff) {
			LW_SAFE_DELETE_A(_rot_seq);
			_rot_num = 0;
			goto __addr_ret_ok;
		}

		if ((id + num) > _rot_num)
			goto __ret;

		{
			lwKeyQuaternion* buf = LW_NEW(lwKeyQuaternion[_rot_num - num]);

			memcpy(&buf[0], &_pos_seq[0], sizeof(lwKeyQuaternion) * id);
			memcpy(&buf[id], &_pos_seq[id + num], sizeof(lwKeyQuaternion) * (_rot_num - id - num));

			_rot_seq = buf;
			_rot_num -= num;
		}
	__addr_ret_ok:
		_ResetFrameNum();
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetPRS2::DelKeyScale(DWORD id, DWORD num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (num == 0xffffffff) {
			LW_SAFE_DELETE_A(_sca_seq);
			_sca_num = 0;
			goto __addr_ret_ok;
		}

		if ((id + num) > _sca_num)
			goto __ret;

		{
			lwKeyVector3* buf = LW_NEW(lwKeyVector3[_sca_num - num]);

			memcpy(&buf[0], &_pos_seq[0], sizeof(lwKeyVector3) * id);
			memcpy(&buf[id], &_pos_seq[id + num], sizeof(lwKeyVector3) * (_sca_num - id - num));

			_sca_seq = buf;
			_sca_num -= num;
		}

	__addr_ret_ok:
		_ResetFrameNum();
		ret = LW_RET_OK;
	__ret:
		return ret;
	}


	LW_RESULT lwAnimKeySetPRS2::Destroy() {
		LW_SAFE_DELETE_A(_pos_seq);
		LW_SAFE_DELETE_A(_rot_seq);
		LW_SAFE_DELETE_A(_sca_seq);

		_pos_num = 0;
		_rot_num = 0;
		_sca_num = 0;
		_frame_num = 0;

		return LW_RET_OK;
	}


	// lwAnimKeySetFloat
	LW_STD_IMPLEMENTATION(lwAnimKeySetFloat)

	lwAnimKeySetFloat::lwAnimKeySetFloat()
		: _data_seq(0), _data_num(0), _data_capacity(0) {
	}

	lwAnimKeySetFloat::~lwAnimKeySetFloat() {
		Clear();
	}

	LW_RESULT lwAnimKeySetFloat::Allocate(DWORD key_capacity) {
		LW_RESULT ret = LW_RET_FAILED;

		if (LW_RESULT r = Clear(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] Clear failed: key_capacity={}, ret={}",
						 __FUNCTION__, key_capacity, static_cast<long long>(r));
			goto __ret;
		}

		_data_num = 0;
		_data_capacity = key_capacity;
		_data_seq = LW_NEW(KEY_TYPE[_data_capacity]);
		memset(_data_seq, 0, sizeof(KEY_TYPE) * _data_capacity);

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::Clear() {
		if (_data_capacity == 0)
			goto __ret;

		_data_num = 0;
		_data_capacity = 0;
		LW_SAFE_DELETE_A(_data_seq);

	__ret:
		return LW_RET_OK;
	}

	LW_RESULT lwAnimKeySetFloat::Clone(lwIAnimKeySetFloat** obj) {
		lwAnimKeySetFloat* o = LW_NEW(lwAnimKeySetFloat);
		o->_data_capacity = _data_capacity;
		o->_data_num = _data_num;

		if (_data_capacity) {
			o->_data_seq = LW_NEW(lwKeyFloat[o->_data_capacity]);
			memset(o->_data_seq, 0, sizeof(KEY_TYPE) * o->_data_capacity);
			memcpy(o->_data_seq, _data_seq, sizeof(KEY_TYPE) * o->_data_num);
		}

		*obj = o;

		return LW_RET_OK;
	}

	LW_RESULT lwAnimKeySetFloat::SetKeyData(DWORD key, float data, DWORD slerp_type, DWORD mask) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD j = 0;
		for (; j < _data_num; j++) {
			if (_data_seq[j].key == key)
				break;
		}

		if (j == _data_num)
			goto __ret;

		if (mask & AKSM_DATA) {
			_data_seq[j].data = data;
		}
		if (mask & AKSM_SLERPTYPE) {
			if (!(slerp_type > __akst_begin && slerp_type < __akst_end))
				goto __ret;

			_data_seq[j].slerp_type = slerp_type;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::GetKeyData(DWORD key, float* data, DWORD* slerp_type) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD j = 0;
		for (; j < _data_num; j++) {
			if (_data_seq[j].key == key)
				break;
		}

		if (j == _data_num)
			goto __ret;

		if (data) {
			*data = _data_seq[j].data;
		}
		if (slerp_type) {
			*slerp_type = _data_seq[j].slerp_type;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::InsertKey(DWORD key, float data, DWORD slerp_type) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_data_num == _data_capacity)
			goto __ret;

		if (!(slerp_type > __akst_begin && slerp_type < __akst_end))
			goto __ret;

		{
			DWORD j = 0;
			for (; j < _data_num; j++) {
				if (_data_seq[j].key < key)
					continue;

				if (_data_seq[j].key == key)
					goto __ret;

				for (DWORD i = _data_num; i > j; i--) {
					_data_seq[i] = _data_seq[i - 1];
				}
			}

			_data_seq[j].key = key;
			_data_seq[j].data = data;
			_data_seq[j].slerp_type = slerp_type;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::RemoveKey(DWORD key) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD i = 0;
		for (; i < _data_num; i++) {
			if (_data_seq[i].key == key)
				break;
		}

		if (i == _data_num)
			goto __ret;

		_data_num -= 1;

		for (; i < _data_num; i++) {
			_data_seq[i] = _data_seq[i + 1];
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::GetValue(float* data, float key) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_data_num == 0)
			goto __ret;

		{
			DWORD i_key = (DWORD)key;
			if ((float)i_key != key) {
				i_key += 1;
			}

			if (i_key > _data_seq[_data_num - 1].key)
				goto __ret;

			DWORD prev_key, next_key;

			if (LW_RESULT r = lwKeyDataSearch(&prev_key, &next_key, i_key, _data_seq, _data_num); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwKeyDataSearch failed: i_key={}, data_num={}, ret={}",
							 __FUNCTION__, i_key, _data_num, static_cast<long long>(r));
				goto __ret;
			}

			lwKeyFloat* k0 = &_data_seq[prev_key];
			lwKeyFloat* k1 = &_data_seq[next_key];

			if (prev_key == next_key) {
				*data = k0->data;
			}
			else {
				float t;

				if (LW_RESULT r = lwGetKeySlerpCoefficent(&t, k0->slerp_type, key, (float)k0->key, (float)k1->key);
					LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwGetKeySlerpCoefficent failed: slerp_type={}, ret={}",
								 __FUNCTION__, k0->slerp_type, static_cast<long long>(r));
					goto __ret;
				}

				*data = lwFloatSlerp(k0->data, k1->data, t);
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwAnimKeySetFloat::SetKeySequence(const lwKeyFloat* seq, DWORD num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (LW_RESULT r = Clear(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] Clear failed: num={}, ret={}",
						 __FUNCTION__, num, static_cast<long long>(r));
			goto __ret;
		}

		_data_num = num;
		_data_capacity = num;
		_data_seq = LW_NEW(KEY_TYPE[_data_capacity]);
		memcpy(_data_seq, seq, sizeof(KEY_TYPE) * _data_num);

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

LW_END
