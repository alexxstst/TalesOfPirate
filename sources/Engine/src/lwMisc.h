//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwITypes.h"
#include "lwITypes2.h"
#include "lwInterfaceExt.h"

LW_BEGIN
	LW_RESULT lwRenderStateAtomBeginSetRS(lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num);
	LW_RESULT lwRenderStateAtomEndSetRS(lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num);
	LW_RESULT lwRenderStateAtomBeginSetTSS(DWORD stage, lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq,
										   DWORD num);
	LW_RESULT lwRenderStateAtomEndSetTSS(DWORD stage, lwIDeviceObject* dev_obj, lwRenderStateAtom* rsa_seq, DWORD num);

	template <DWORD seq_size>
	struct lwRenderStateAtomSeq {
		enum {
			SEQUENCE_SIZE = seq_size
		};

		lwRenderStateAtom rsa_seq[SEQUENCE_SIZE];
	};

	template <DWORD seq_size>
	inline void lwRenderStateAtomSeq_Construct(lwRenderStateAtomSeq<seq_size>* obj) {
		typedef lwRenderStateAtomSeq<seq_size> type_value;

		lwRenderStateAtom* p;
		for (DWORD i = 0; i < type_value::SEQUENCE_SIZE; i++) {
			p = &obj[i];
			p->state = LW_INVALID_INDEX;
			p->value0 = LW_INVALID_INDEX;
			p->value1 = LW_INVALID_INDEX;
		}
	}

	template <DWORD seq_size>
	inline void lwSetRenderStateAtomSeq(lwRenderStateAtomSeq<seq_size>* obj, DWORD id, DWORD state, DWORD value) {
		lwRenderStateAtom* p = &obj->rsa_seq[id];
		p->state = state;
		p->value0 = value;
		p->value1 = value;
	}

	class lwRenderStateAtomSet : public lwIRenderStateAtomSet {
		LW_STD_DECLARATION()

	private:
		lwRenderStateAtom* _rsa_seq;
		DWORD _rsa_num;

	public:
		lwRenderStateAtomSet();
		~lwRenderStateAtomSet();

		LW_RESULT Allocate(DWORD size);
		LW_RESULT Clear();
		LW_RESULT Clone(lwIRenderStateAtomSet** obj);
		LW_RESULT Load(const lwRenderStateAtom* rsa_seq, DWORD rsa_num);
		LW_RESULT FindState(DWORD* id, DWORD state);
		LW_RESULT ResetStateValue(DWORD state, DWORD value, DWORD* old_value);
		LW_INLINE LW_RESULT SetValue(DWORD id, DWORD value);
		LW_INLINE LW_RESULT SetStateValue(DWORD id, DWORD state, DWORD value);
		LW_INLINE LW_RESULT SetStateValue(DWORD id, lwRenderStateAtom* buf, DWORD num);
		LW_INLINE LW_RESULT BeginRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num);
		LW_INLINE LW_RESULT BeginTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage);
		LW_INLINE LW_RESULT BeginSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage);
		LW_INLINE LW_RESULT EndRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num);
		LW_INLINE LW_RESULT EndTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage);
		LW_INLINE LW_RESULT EndSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage);

		DWORD GetStateNum() const {
			return _rsa_num;
		}

		LW_RESULT GetStateAtom(lwRenderStateAtom** rsa, DWORD id);

		lwRenderStateAtom* GetStateSeq() {
			return _rsa_seq;
		}
	};

	LW_INLINE LW_RESULT lwRenderStateAtomSet::SetValue(DWORD id, DWORD value) {
		if (id >= _rsa_num)
			return LW_RET_FAILED;

		_rsa_seq[id].value0 = value;
		_rsa_seq[id].value1 = value;

		return LW_RET_OK;
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::SetStateValue(DWORD id, DWORD state, DWORD value) {
		if (id >= _rsa_num)
			return LW_RET_FAILED;

		_rsa_seq[id].state = state;
		_rsa_seq[id].value0 = value;
		_rsa_seq[id].value1 = value;

		return LW_RET_OK;
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::SetStateValue(DWORD id, lwRenderStateAtom* buf, DWORD num) {
		if (id + num > _rsa_num)
			return LW_RET_FAILED;

		memcpy(&_rsa_seq[id], buf, sizeof(lwRenderStateAtom) * num);

		return LW_RET_OK;
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::BeginRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		return lwRenderStateAtomBeginSetRS(dev_obj, &_rsa_seq[start_id], n);
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::BeginTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id,
																	 DWORD num, DWORD stage) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		return lwRenderStateAtomBeginSetTSS(stage, dev_obj, &_rsa_seq[start_id], n);
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::BeginSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num,
																DWORD stage) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		//return lwRenderStateAtomBeginSetSS(stage, dev_obj, &_rsa_seq[start_id], n);
		return -1L;
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::EndRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		return lwRenderStateAtomEndSetRS(dev_obj, &_rsa_seq[start_id], n);
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::EndTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num,
																   DWORD stage) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		return lwRenderStateAtomEndSetTSS(stage, dev_obj, &_rsa_seq[start_id], n);
	}

	LW_INLINE LW_RESULT lwRenderStateAtomSet::EndSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num,
															  DWORD stage) {
		DWORD n = (num == LW_INVALID_INDEX) ? (_rsa_num - start_id) : num;
		//return lwRenderStateAtomEndSetSS(stage, dev_obj, &_rsa_seq[start_id], n);
		return -1L;
	}


	class lwBuffer : public lwIBuffer {
		LW_STD_DECLARATION();

	private:
		BYTE* _data;
		DWORD _size;

	public:
		lwBuffer();
		~lwBuffer();

		LW_RESULT Alloc(DWORD size);
		LW_RESULT Realloc(DWORD size);
		LW_RESULT Free();
		LW_RESULT SetSizeArbitrary(DWORD size);

		BYTE* GetData() {
			return _data;
		}

		DWORD GetSize() {
			return _size;
		}
	};

	class lwByteSet : public lwIByteSet {
		LW_STD_DECLARATION();

	private:
		BYTE* _buf;
		DWORD _size;

	public:
		lwByteSet()
			: _buf(0), _size(0) {
		}

		~lwByteSet() {
			Free();
		}

		LW_RESULT Alloc(DWORD size) {
			_buf = LW_NEW(BYTE[size]);
			memset(_buf, 0, sizeof(BYTE) * size);
			_size = size;
			return _buf ? LW_RET_OK : LW_RET_FAILED;
		}

		LW_RESULT Free() {
			LW_SAFE_DELETE_A(_buf);
			_size = 0;
			return LW_RET_OK;
		}

		LW_RESULT SetValueSeq(DWORD start, BYTE* buf, DWORD num);

		void SetValue(DWORD type, BYTE value) {
			_buf[type] = value;
		}

		BYTE GetValue(DWORD type) {
			return _buf[type];
		}
	};

	LW_RESULT lwIAnimCtrlObj_PlayPose(lwIAnimCtrlObj* ctrl_obj, const lwPlayPoseInfo* info);
	lwIPoseCtrl* lwIAnimCtrlObj_GetPoseCtrl(lwIAnimCtrlObj* ctrl_obj);
	lwPlayPoseInfo* lwIAnimCtrlObj_GetPlayPoseInfo(lwIAnimCtrlObj* ctrl_obj);
	DWORD lwGetBlendWeightNum(DWORD fvf);
	LW_RESULT LoadFileInMemory(lwIBuffer* buf, std::string_view file, const char* load_flag);
	LW_RESULT LoadFileInMemory(BYTE** data_seq, DWORD* data_size, std::string_view file, const char* load_flag);
	int lwHexStrToInt(const char* str);

LW_END
