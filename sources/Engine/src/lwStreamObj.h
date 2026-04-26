//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwExpObj.h"
#include "lwSlotMap.h"
#include "lwInterfaceExt.h"

LW_BEGIN
#define LW_INVALID_STREAM_ID        0xffff
#define LW_INVALID_STREAMENTRY_ID   0xffff

	enum {
		STREAMENTITY_STATE_INVALID = 0,
		STREAMENTITY_STATE_INIT = 1,
		STREAMENTITY_STATE_BIND = 2,
	};

	class lwStreamEntity {
	public:
		DWORD state;
		DWORD size;
		DWORD offset; // vertices offset from begin in vb
		DWORD stride;
		DWORD stream_id;
		void* data;

	public:
		lwStreamEntity()
			: size(0), offset(0), stride(0), data(0),
			  stream_id(LW_INVALID_INDEX), state(STREAMENTITY_STATE_INVALID) {
		}

		~lwStreamEntity() {
		}

		void Reset() {
			size = 0;
			offset = 0;
			stride = 0;
			data = 0;
			stream_id = LW_INVALID_INDEX;
			state = STREAMENTITY_STATE_INVALID;
		}
	};

	class lwStreamObject {
	protected:
	public:
		DWORD _total_size;
		DWORD _free_size;
		DWORD _locked_size;
		DWORD _unused_size;
		DWORD _lock_addr;

	public:
		lwStreamObject()
			: _total_size(0), _free_size(0), _locked_size(0), _unused_size(0), _lock_addr(0) {
		}

		DWORD GetLockAddr() {
			return _lock_addr;
		}

		DWORD GetTotalSize() {
			return _total_size;
		}

		DWORD GetLockedSize() {
			return _locked_size;
		}

		DWORD GetUnusedSize() {
			return _unused_size;
		}

		DWORD GetFreeSize() {
			return _free_size;
		}

		DWORD GetReservedSize() {
			return (GetTotalSize() - GetUnusedSize() - GetLockAddr());
		}

		LW_RESULT ApplyRoom(DWORD* out_off_addr, DWORD size, DWORD stride);
		LW_RESULT ReserveRoom(DWORD size, DWORD stride);
		LW_RESULT BindData(DWORD* out_off_addr, DWORD size, DWORD stride);
		LW_RESULT UnbindData(DWORD size, DWORD stride);
		LW_RESULT Init(DWORD buffer_size);

		LW_RESULT Reset() {
			return Init(_total_size);
		}
	};

	class lwStreamObjVB : public lwStreamObject {
	public:
		DWORD _fvf;
		lwIDeviceObject* _dev_obj;
		IDirect3DVertexBufferX* _buf;

	public:
		lwStreamObjVB(lwIDeviceObject* dev_obj);
		~lwStreamObjVB();

		LW_RESULT BindData(DWORD* out_off_addr, void* data, DWORD size, DWORD stride);
		LW_RESULT Create(DWORD size, DWORD fvf);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	class lwStreamObjIB : public lwStreamObject {
	public:
		DWORD _format;
		lwIDeviceObject* _dev_obj;
		IDirect3DIndexBufferX* _buf;

	public:
		lwStreamObjIB(lwIDeviceObject* dev_obj);
		~lwStreamObjIB();

		LW_RESULT BindData(DWORD* out_off_addr, void* data, DWORD size, DWORD stride);
		LW_RESULT Create(DWORD size, DWORD format);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	class lwStaticStreamMgr : public lwIStaticStreamMgr {
		LW_STD_DECLARATION()

		enum { MAX_STREAM_SEQ_SIZE = 32 };

	private:
		lwIDeviceObject* _dev_obj;

		lwStreamEntity* _entity_vb_seq;
		lwStreamEntity* _entity_ib_seq;

		lwStreamObjVB* _stream_vb_seq[MAX_STREAM_SEQ_SIZE];
		lwStreamObjIB* _stream_ib_seq[MAX_STREAM_SEQ_SIZE];
		DWORD _index_entry_offset;
		DWORD _vertex_entry_offset; // dx9 SetStreamSource

		DWORD _entity_vb_num;
		DWORD _entity_vb_seq_size;
		DWORD _entity_ib_num;
		DWORD _entity_ib_seq_size;
		DWORD _next_entity_vb_id;
		DWORD _next_entity_ib_id;

		DWORD _stream_vb_num;
		DWORD _stream_ib_num;

	public:
		lwStaticStreamMgr(lwIDeviceObject* dev_obj);
		~lwStaticStreamMgr();

		LW_RESULT Destroy();
		LW_RESULT CreateStreamEntitySeq(DWORD entity_vb_num, DWORD entity_ib_num);
		LW_RESULT CreateVertexBufferStream(DWORD stream_id, DWORD stream_size);
		LW_RESULT CreateIndexBufferStream(DWORD stream_id, DWORD stream_size);

		LW_RESULT RegisterVertexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride);
		LW_RESULT RegisterIndexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride);
		LW_RESULT UnregisterVertexBuffer(LW_HANDLE handle);
		LW_RESULT UnregisterIndexBuffer(LW_HANDLE handle);
		LW_RESULT BindVertexBuffer(LW_HANDLE handle, UINT channel);
		LW_RESULT BindIndexBuffer(LW_HANDLE handle);

		DWORD GetVertexEntityOffset() {
			return _vertex_entry_offset;
		}

		DWORD GetIndexEntityOffset() {
			return _index_entry_offset;
		}

		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
		LW_RESULT GetDebugInfo(lwStaticStreamMgrDebugInfo* info);
	};

	// dynamic stream object
	class lwDynamicStream {
	protected:
		DWORD _total_size;
		DWORD _free_size;
		DWORD _free_addr;
		DWORD _base_index;

		DWORD _fvf;

	public:
		lwDynamicStream();
		~lwDynamicStream();

		DWORD GetFreeAddr() const {
			return _free_addr;
		}

		DWORD GetTotalSize() const {
			return _total_size;
		}

		DWORD GetFreeSize() const {
			return _free_size;
		}

		DWORD GetValidSize() const {
			return (_total_size - _free_addr);
		}

		DWORD GetBaseIndex() const {
			return _base_index;
		}

		//  fail_branch (out, опционально) сообщает, какая именно проверка
		//  вернула FAILED:
		//    0 — успех (offset заполнен),
		//    1 — ветка A: GetValidSize() < size (учётно нет места),
		//    2 — ветка B: (o + size) >= GetTotalSize() (упёрлись в конец кольца — wrap).
		LW_RESULT GetEntryOffset(DWORD* offset, DWORD size, DWORD stride, int* fail_branch = nullptr);
		LW_RESULT ResetFreeSize(DWORD size, DWORD offset);
	};

	class lwDynamicStreamVB : public lwDynamicStream {
	private:
		lwIDeviceObject* _dev_obj;
		IDirect3DVertexBufferX* _buf;

	public:
		lwDynamicStreamVB(lwIDeviceObject* dev_obj);
		~lwDynamicStreamVB();

		LW_RESULT Create(DWORD buf_size, D3DFORMAT fmt);
		LW_RESULT Bind(DWORD channel, const void* data, DWORD size, DWORD stride);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	class lwDynamicStreamIB : public lwDynamicStream {
	private:
		lwIDeviceObject* _dev_obj;
		IDirect3DIndexBufferX* _buf;

	public:
		lwDynamicStreamIB(lwIDeviceObject* dev_obj);
		~lwDynamicStreamIB();

		LW_RESULT Create(DWORD buf_size, D3DFORMAT fmt);
		LW_RESULT Bind(DWORD vert_index, const void* data, DWORD size, DWORD stride);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	class lwDynamicStreamMgr : public lwIDynamicStreamMgr {
		LW_STD_DECLARATION()

	private:
		lwIDeviceObject* _dev_obj;
		lwDynamicStreamVB _vb;
		lwDynamicStreamIB _ib;

	private:
		DWORD _GetVerticesSize(D3DPRIMITIVETYPE pt_type, DWORD count, DWORD stride);
		DWORD _GetIndicesSize(D3DPRIMITIVETYPE pt_type, DWORD count, DWORD stride);

	public:
		lwDynamicStreamMgr(lwIDeviceObject* dev_obj)
			: _dev_obj(dev_obj), _vb(dev_obj), _ib(dev_obj) {
		}

		LW_RESULT Create(DWORD vb_size, DWORD ib_size);
		LW_RESULT DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT count, const void* vert_data, UINT vert_stride,
								D3DFORMAT fvf);
		LW_RESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, UINT min_vert_index, UINT num_vert_indices, UINT count,
									   const void* index_data, D3DFORMAT index_format, const void* vertex_data,
									   UINT vert_stride, D3DFORMAT fvf);

		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();

		LW_RESULT BindDataVB(DWORD channel, const void* data, DWORD size, UINT stride);
		LW_RESULT BindDataIB(const void* data, DWORD size, DWORD stride);
		LW_RESULT DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT start_vert, UINT count);
		LW_RESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, INT base_vert_index, UINT min_index, UINT num_vert,
									   UINT start_index, UINT count);
	};

	// lockable stream
	class lwLockableStream {
	protected:
		DWORD _size;
		DWORD _usage;
		DWORD _fvf;
		BYTE* _data;
		UINT _lock_offset;
		UINT _lock_size;
		DWORD _lock_flag;
		DWORD _lock_cnt;

	public:
		lwLockableStream()
			: _size(0), _usage(0), _fvf(0), _data(0)
			  , _lock_offset(0), _lock_size(0), _lock_flag(0), _lock_cnt(0) {
		}

		inline void Reset() {
			_size = 0;
			_usage = 0;
			_fvf = 0;
			_data = 0;
			_lock_offset = 0;
			_lock_size = 0;
			_lock_flag = 0;
			_lock_cnt = 0;
		}
	};

	class lwLockableStreamVB : public lwLockableStream, public lwILockableStreamVB {
		LW_STD_DECLARATION()

	private:
		IDirect3DVertexBufferX* _buf;
		lwIDeviceObject* _dev_obj;

	public:
		lwLockableStreamVB(lwIDeviceObject* dev_obj)
			: _dev_obj(dev_obj), _buf(0) {
		}

		~lwLockableStreamVB() {
			if (_buf)
				LoseDevice();
		}

		LW_RESULT Create(void* data, DWORD size, DWORD usage, DWORD fvf);
		LW_RESULT Destroy();
		LW_RESULT Lock(UINT offset, UINT size, void** data, DWORD flag);
		LW_RESULT Unlock();
		LW_RESULT Bind(UINT channel, UINT offset_byte, UINT stride);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	class lwLockableStreamIB : public lwLockableStream, public lwILockableStreamIB {
		LW_STD_DECLARATION()

	private:
		IDirect3DIndexBufferX* _buf;
		lwIDeviceObject* _dev_obj;

	public:
		lwLockableStreamIB(lwIDeviceObject* dev_obj)
			: _dev_obj(dev_obj), _buf(0) {
		}

		~lwLockableStreamIB() {
			LoseDevice();
		}

		LW_RESULT Create(void* data, DWORD size, DWORD usage, DWORD fvf);
		LW_RESULT Destroy();
		LW_RESULT Lock(UINT offset, UINT size, void** data, DWORD flag);
		LW_RESULT Unlock();
		LW_RESULT Bind(UINT base_vert_index);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};

	// lwLockableStreamMgr
	class lwLockableStreamMgr : public lwILockableStreamMgr {
		LW_STD_DECLARATION()

		typedef lwSlotMapVoidPtr1024 lwPoolStreamVB;
		typedef lwSlotMapVoidPtr1024 lwPoolStreamIB;
		typedef void* lwPoolEntity;

	private:
		lwIResourceMgr* _res_mgr;
		lwPoolStreamVB _pool_vb;
		lwPoolStreamIB _pool_ib;

	public:
		lwLockableStreamMgr(lwIResourceMgr* res_mgr);
		virtual ~lwLockableStreamMgr();

		lwILockableStreamVB* GetStreamVB(LW_HANDLE handle);
		lwILockableStreamIB* GetStreamIB(LW_HANDLE handle);

		LW_RESULT RegisterVertexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf);
		LW_RESULT RegisterIndexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf);
		LW_RESULT UnregisterVertexBuffer(LW_HANDLE handle);
		LW_RESULT UnregisterIndexBuffer(LW_HANDLE handle);
		LW_RESULT BindVertexBuffer(LW_HANDLE handle, UINT channel, UINT offset_byte, UINT stride);
		LW_RESULT BindIndexBuffer(LW_HANDLE handle, UINT base_vert_index);
		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();
	};


	class lwSurfaceStream : public lwISurfaceStream {
		LW_STD_DECLARATION()

	private:
		DWORD _type;
		DWORD _width;
		DWORD _height;
		DWORD _format;

		union {
			DWORD _lockable;
			DWORD _discard;
		};

		D3DMULTISAMPLE_TYPE _multi_sample;
		DWORD _multi_sample_quality;
		IDirect3DSurfaceX* _surface;
		lwISurfaceStreamMgr* _ss_mgr;
		lwIDeviceObject* _dev_obj;
		DWORD _reg_id;

	public:
		lwSurfaceStream(lwISurfaceStreamMgr* ss_mgr);
		~lwSurfaceStream();

		LW_RESULT CreateRenderTarget(UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample,
									 DWORD multi_sample_quality, BOOL lockable, HANDLE* handle);
		LW_RESULT CreateDepthStencilSurface(UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample,
											DWORD multi_sample_quality, BOOL discard, HANDLE* handle);

		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();

		void SetRegisterID(DWORD id) {
			_reg_id = id;
		}

		DWORD GetRegisterID() const {
			return _reg_id;
		}

		IDirect3DSurfaceX* GetSurface() {
			return _surface;
		}
	};

	class lwSurfaceStreamMgr : public lwISurfaceStreamMgr {
		LW_STD_DECLARATION()

		typedef lwSlotMapVoidPtr1024 lwPoolSurface;

	private:
		lwIResourceMgr* _res_mgr;
		lwPoolSurface _pool_surface;

	public:
		lwSurfaceStreamMgr(lwIResourceMgr* res_mgr);
		~lwSurfaceStreamMgr();

		LW_RESULT CreateRenderTarget(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format,
									 D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL lockable,
									 HANDLE* handle);
		LW_RESULT CreateDepthStencilSurface(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format,
											D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL discard,
											HANDLE* handle);
		LW_RESULT UnregisterSurface(LW_HANDLE handle);

		LW_RESULT LoseDevice();
		LW_RESULT ResetDevice();

		lwIResourceMgr* GetResourceMgr() {
			return _res_mgr;
		}

		lwISurfaceStream* GetSurfaceStream(LW_HANDLE handle);
	};


	// individual resource
	class lwVertexBuffer : public lwIVertexBuffer {
		LW_STD_DECLARATION();
		friend class lwDeviceObject;

	private:
		lwIDeviceObject* _dev_obj;
		IDirect3DVertexBufferX* _buf;
		lwVertexBufferInfo _buf_info;
		DWORD _dlock_pos;
		lwVertexBuffer* _next;
		lwVertexBuffer* _prev;

	private:
		inline void _RemoveThisFromList() {
			if (_next) {
				_next->_prev = _prev;
			}
			if (_prev) {
				_prev->_next = _next;
			}
		}

	public:
		lwVertexBuffer(lwIDeviceObject* dev_obj);
		~lwVertexBuffer();

		LW_RESULT Create(UINT length, DWORD usage, DWORD fvf, D3DPOOL pool, DWORD stride, HANDLE* handle);
		LW_RESULT Destroy();

		LW_RESULT Lock(UINT offset, UINT size, D3DLOCK_TYPE** out_data, DWORD flag) {
			return _buf->Lock(offset, size, out_data, flag);
		}

		LW_RESULT Unlock() {
			return _buf->Unlock();
		}

		LW_RESULT LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag);
		LW_RESULT LoadDataDynamic(const void* data_seq, DWORD data_size);

		LW_RESULT BindDevice(DWORD stream_id, UINT offset_byte) {
			return _dev_obj->SetStreamSource(stream_id, _buf, offset_byte, _buf_info.stride);
		}

		LW_RESULT SetDeviceFVF() {
			return _dev_obj->SetFVF(_buf_info.fvf);
		}

		lwVertexBufferInfo* GetBufferInfo() {
			return &_buf_info;
		}

		IDirect3DVertexBufferX* GetStreamBuffer() {
			return _buf;
		}
	};

	class lwIndexBuffer : public lwIIndexBuffer {
		LW_STD_DECLARATION();
		friend class lwDeviceObject;

	private:
		lwIDeviceObject* _dev_obj;
		IDirect3DIndexBufferX* _buf;
		lwIndexBufferInfo _buf_info;
		DWORD _dlock_pos;
		lwIndexBuffer* _next;
		lwIndexBuffer* _prev;

	private:
		inline void _RemoveThisFromList() {
			if (_next) {
				_next->_prev = _prev;
			}
			if (_prev) {
				_prev->_next = _next;
			}
		}

	public:
		lwIndexBuffer(lwIDeviceObject* dev_obj);
		~lwIndexBuffer();

		LW_RESULT Create(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, HANDLE* handle);
		LW_RESULT Destroy();

		LW_RESULT Lock(UINT offset, UINT size, D3DLOCK_TYPE** out_data, DWORD flag) {
			return _buf->Lock(offset, size, out_data, flag);
		}

		LW_RESULT Unlock() {
			return _buf->Unlock();
		}

		LW_RESULT LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag);
		LW_RESULT LoadDataDynamic(const void* data_seq, DWORD data_size);

		LW_RESULT BindDevice() {
			return _dev_obj->SetIndices(_buf, 0);
		}

		lwIndexBufferInfo* GetBufferInfo() {
			return &_buf_info;
		}

		IDirect3DIndexBufferX* GetStreamBuffer() {
			return _buf;
		}
	};


LW_END
