//
#pragma once


#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"


LW_BEGIN
	// by lsh
	// PlayPoseCallBackPLAY_LOOP

	class lwPoseCtrl : public lwIPoseCtrl {
	private:
		lwPoseInfo* _pose_seq;
		DWORD _pose_num;
		DWORD _frame_num;

		LW_STD_DECLARATION()

	private:
		LW_RESULT _Update(DWORD pose, DWORD play_type, float velocity, float* io_frame, float* o_ret_frame);

	public:
		lwPoseCtrl() : _pose_seq(0), _pose_num(0), _frame_num(0xffffffff) {
		}

		~lwPoseCtrl() {
			LW_SAFE_DELETE_A(_pose_seq);
		}

		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file) const;
		LW_RESULT Load(FILE* fp);
		LW_RESULT Save(FILE* fp) const;

		LW_RESULT Copy(const lwPoseCtrl* src);
		LW_RESULT Clone(lwIPoseCtrl** obj);

		DWORD GetDataSize() const;

		LW_RESULT InsertPose(DWORD id, const lwPoseInfo* pi, int num);
		LW_RESULT ReplacePose(DWORD id, const lwPoseInfo* pi);
		LW_RESULT RemovePose(DWORD id);
		LW_RESULT RemoveAll();
		LW_RESULT GetPoseInfoBuffer(lwPoseInfo** buf);

		void SetFrameNum(int frame) {
			_frame_num = frame;
		}

		DWORD GetPoseNum() const {
			return _pose_num;
		}

		lwPoseInfo* GetPoseInfo(DWORD id) {
			return (id >= _pose_num) ? 0 : &_pose_seq[id];
		}

		DWORD GetPoseFrameNum(DWORD id) const {
			return (id >= _pose_num) ? 0 : (_pose_seq[id].end - _pose_seq[id].start + 1);
		}

		int IsPosePlaying(const lwPlayPoseInfo* info) const;

		LW_RESULT PlayPose(lwPlayPoseInfo* info);
		LW_RESULT CallBack(const lwPlayPoseInfo* info);
	};


	LW_RESULT lwPlayPoseSmooth(lwPlayPoseInfo* dst, const lwPlayPoseInfo* src, lwIPoseCtrl* ctrl);

LW_END
