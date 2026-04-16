//
#include "stdafx.h"
#include "lwPoseCtrl.h"
#include "lwErrorCode.h"
#include <string>

LW_BEGIN
	////////////////////////////

	LW_STD_IMPLEMENTATION(lwPoseCtrl)

	LW_RESULT lwPoseCtrl::Load(FILE* fp) {
		size_t t = fread(&_pose_num, sizeof(_pose_num), 1, fp);

		_pose_seq = LW_NEW(lwPoseInfo[_pose_num]);
		t = fread(&_pose_seq[0], sizeof(lwPoseInfo), _pose_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::Save(FILE* fp) const {
		fwrite(&_pose_num, sizeof(_pose_num), 1, fp);
		fwrite(&_pose_seq[0], sizeof(lwPoseInfo), _pose_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::Load(const char* file) {
		FILE* fp = fopen(file, "rb");

		if (fp == NULL)
			return ERR_INVALID_PARAM;

		DWORD version;

		fread(&version, sizeof(DWORD), 1, fp);
		assert(version == 1 && "error version code");

		Load(fp);

		fclose(fp);

		return 1;
	}

	LW_RESULT lwPoseCtrl::Save(const char* file) const {
		FILE* fp = fopen(file, "wb");
		if (fp == 0)
			return ERR_INVALID_PARAM;

		DWORD version = 1;
		fwrite(&version, sizeof(DWORD), 1, fp);

		Save(fp);

		fclose(fp);

		return 1;
	}

	LW_RESULT lwPoseCtrl::Clone(lwIPoseCtrl** obj) {
		lwPoseCtrl* o = LW_NEW(lwPoseCtrl);

		if (_pose_num > 0) {
			o->InsertPose(0, _pose_seq, _pose_num);
		}

		*obj = o;

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::Copy(const lwPoseCtrl* src) {
		_pose_num = src->_pose_num;
		_pose_seq = LW_NEW(lwPoseInfo[_pose_num]);
		memcpy(&_pose_seq[0], &src->_pose_seq[0], sizeof(lwPoseInfo) * _pose_num);

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::GetPoseInfoBuffer(lwPoseInfo** buf) {
		if (buf) {
			*buf = _pose_seq;
		}
		return LW_RET_OK;
	}

	DWORD lwPoseCtrl::GetDataSize() const {
		DWORD size = 0;

		size += sizeof(_pose_num);
		size += sizeof(lwPoseInfo) * _pose_num;

		return size;
	}

	LW_RESULT lwPoseCtrl::_Update(DWORD pose, DWORD play_type, float velocity, float* io_frame, float* o_ret_frame) {
		float l;
		float cur_frame;
		float next_frame;

		cur_frame = *io_frame;

		lwPoseInfo* pi = &_pose_seq[pose];

		l = (float)(pi->end - pi->start);
		assert(l > 0);

		if (play_type == PLAY_ONCE) {
			if (cur_frame >= l) {
				cur_frame = l;
			}
			else if (cur_frame < 0.0f) {
				cur_frame = 0.0f;
			}
		}
		else if (play_type == PLAY_LOOP) {
			if (cur_frame > l) {
				cur_frame = fmodf(cur_frame, l + 1);
			}
			else if (cur_frame < 0.0f) {
				while (cur_frame < 0.0f) {
					cur_frame += l;
				}
			}
		}

		if ((play_type == PLAY_ONCE) || (play_type == PLAY_LOOP)) {
			next_frame = cur_frame + velocity;
		}
		else if (play_type == PLAY_FRAME) {
			next_frame = cur_frame;
		}


		*o_ret_frame = cur_frame + pi->start;
		*io_frame = next_frame;

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::PlayPose(lwPlayPoseInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_pose_num == 0)
			goto __ret;

		if (info->type == PLAY_INVALID)
			goto __ret;

		if (LW_FAILED(_Update(info->pose, info->type, info->velocity, &info->frame, &info->ret_frame)))
			goto __ret;

		if (info->blend_info.op_state) {
			if (LW_FAILED(
				_Update(info->blend_info.blend_pose, info->blend_info.blend_type, info->velocity, &info->blend_info.
					blend_frame, &info->blend_info.blend_ret_frame)))
				goto __ret;

			info->blend_info.weight += (info->blend_info.factor * info->velocity); // Mdr

			info->blend_info.op_frame_length -= info->velocity; // Mdr

			if (info->blend_info.op_frame_length <= 0.0f) {
				info->blend_info.op_state = 0;
			}
		}

#if 0
		float l;
		float cur_frame;
		float next_frame;

		cur_frame = info->frame;

		lwPoseInfo* pi = &_pose_seq[info->pose];

		l = (float)(pi->end - pi->start);
		assert(l > 0);

		if (info->type == PLAY_ONCE) {
			if (cur_frame >= l) {
				cur_frame = l;
			}
			else if (cur_frame < 0.0f) {
				cur_frame = 0.0f;
			}
		}
		else if (info->type == PLAY_LOOP) {
			if (cur_frame > l) {
				cur_frame = fmodf(cur_frame, l + 1);
			}
			else if (cur_frame < 0.0f) {
				while (cur_frame < 0.0f) {
					cur_frame += l;
				}
			}
		}

		if ((info->type == PLAY_ONCE) || (info->type == PLAY_LOOP)) {
			next_frame = cur_frame + info->velocity;
		}
		else if (info->type == PLAY_FRAME) {
			next_frame = cur_frame;
		}


		info->ret_frame = cur_frame + pi->start;
		info->frame = next_frame;
#endif

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPoseCtrl::CallBack(const lwPlayPoseInfo* info) {
		if (_pose_num == 0)
			return LW_RET_FAILED_3;

		if (info->type == PLAY_INVALID)
			return LW_RET_FAILED_2;

		if (info->proc == 0)
			return LW_RET_OK;

		lwPoseInfo* pi = &_pose_seq[info->pose];

		float l = (float)(pi->end - pi->start);
		assert(l > 0);

		float cur_frame;
		float next_frame;

		next_frame = info->frame;
		cur_frame = info->frame;
		if (info->type == PLAY_ONCE) {
			cur_frame -= info->velocity;
			assert(cur_frame >= 0.0f);
		}
		else if (info->type == PLAY_LOOP) {
			cur_frame -= info->velocity;
			if (cur_frame < 0.0f) {
				cur_frame += l;
			}
		}


		static int kk = 0;

		const float eps = 1e-4f;

		DWORD key_type = KEYFRAME_TYPE_INVALID;

		if (info->type == PLAY_ONCE) {
			if ((cur_frame + eps) >= l) {
				kk--;
				key_type = KEYFRAME_TYPE_END;
			}
			else if ((cur_frame - eps) <= 0.0f) {
				kk++;
				key_type = KEYFRAME_TYPE_BEGIN;
			}
		}
		else if (info->type == PLAY_LOOP) {
			if ((cur_frame > (l - info->velocity)) && (cur_frame <= l)) {
				key_type = KEYFRAME_TYPE_END;
			}
			else if ((cur_frame >= 0.0f) && (cur_frame < info->velocity)) {
				key_type = KEYFRAME_TYPE_BEGIN;
			}
		}

		if (key_type != KEYFRAME_TYPE_INVALID) {
			info->proc(key_type, info->pose, 0, (DWORD)cur_frame, info->proc_param);
		}

		// check key frame pose
		if (next_frame < cur_frame) {
			next_frame += l;
		}

		float k;
		for (DWORD i = 0; i < pi->key_frame_num; i++) {
			k = (float)(pi->key_frame_seq[i] - pi->start);

			if ((k >= cur_frame) && (k < next_frame)) {
				info->proc(KEYFRAME_TYPE_KEY, info->pose, i, pi->key_frame_seq[i], info->proc_param);
			}
		}


		return LW_RET_OK;
	}

	int lwPoseCtrl::IsPosePlaying(const lwPlayPoseInfo* info) const {
		int state = 0;

		if (info->type == PLAY_ONCE) {
			state = info->frame < (_pose_seq[info->pose].end - _pose_seq[info->pose].start);
		}
		else if (info->type == PLAY_LOOP) {
			state = 1;
		}

		return state;
	}

	LW_RESULT lwPoseCtrl::InsertPose(DWORD id, const lwPoseInfo* pi, int num) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD i;
		if (pi == 0 || num <= 0)
			goto __ret;

		if (id > _pose_num)
			goto __ret;

		{
			for (i = 0; i < num; i++) {
				if ((pi[i].start > pi[i].end) || (pi[i].end >= _frame_num)) {
					std::string_view reason = (pi[i].start > pi[i].end)
						? "start > end"
						: "end >= animation length";
					ToLogService("errors", LogLevel::Error,
								 "lwPoseCtrl::InsertPose: некорректная запись character_actions "
								 "({}). actionNo={}, start={}, end={}, animLength={}",
								 reason, i + 1, pi[i].start, pi[i].end, _frame_num);
					goto __ret;
				}
			}
			// it is valid for null pose_seq
			lwPoseInfo* buf = LW_NEW(lwPoseInfo[_pose_num + num]);

			memcpy(buf, _pose_seq, sizeof(lwPoseInfo) * id);
			memcpy(&buf[id], pi, sizeof(lwPoseInfo) * num);
			memcpy(&buf[id + num], &_pose_seq[id], sizeof(lwPoseInfo) * (_pose_num - id));

			LW_SAFE_DELETE_A(_pose_seq);

			_pose_seq = buf;
			_pose_num += num;
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPoseCtrl::ReplacePose(DWORD id, const lwPoseInfo* pi) {
		LW_RESULT ret = LW_RET_FAILED;

		if ((id >= (DWORD)_pose_num) || (pi == 0))
			goto __ret;

		if (pi->start < pi->end || pi->end >= _frame_num)
			goto __ret;

		_pose_seq[id] = *pi;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPoseCtrl::RemovePose(DWORD id) {
		if (id >= (DWORD)_pose_num)
			return ERR_INVALID_PARAM;

		lwPoseInfo* buf = LW_NEW(lwPoseInfo[_pose_num - 1]);

		memcpy(buf, _pose_seq, sizeof(lwPoseInfo) * id);
		memcpy(&buf[id], &_pose_seq[id + 1], sizeof(lwPoseInfo) * (_pose_num - id - 1));


		LW_SAFE_DELETE_A(_pose_seq);

		_pose_seq = buf;
		_pose_num -= 1;

		return LW_RET_OK;
	}

	LW_RESULT lwPoseCtrl::RemoveAll() {
		if (_pose_num) {
			LW_DELETE_A(_pose_seq);
			_pose_seq = 0;
			_pose_num = 0;
		}

		return LW_RET_OK;
	}


	LW_RESULT lwPlayPoseSmooth(lwPlayPoseInfo* dst, const lwPlayPoseInfo* src, lwIPoseCtrl* ctrl) {
		LW_RESULT ret = LW_RET_FAILED;

		if (src->bit_mask & PPI_MASK_POSE) {
			if (src->pose >= ctrl->GetPoseNum())
				goto __ret;
		}
		{
			if (src->bit_mask & PPI_MASK_VELOCITY) {
				dst->velocity = src->velocity;
			}
			if (src->bit_mask & PPI_MASK_PROC) {
				dst->proc = src->proc;
				dst->proc_param = src->proc_param;
			}
			if (src->bit_mask & PPI_MASK_DATA) {
				dst->data = src->data;
			}

			// checked before type
			if ((src->bit_mask & PPI_MASK_BLENDINFO) && (dst->type == PLAY_LOOP || dst->type == PLAY_ONCE)) {
				lwPoseInfo* src_pi = ctrl->GetPoseInfo(dst->pose);
				lwPoseInfo* dst_pi = ctrl->GetPoseInfo(src->pose);
				float src_frame_num = (src_pi->end - src_pi->start) - dst->frame;
				float dst_frame_num = (dst_pi->end - dst_pi->start) - src->frame;

				// loop animation used full frame size
				if (dst->type == PLAY_LOOP) {
					src_frame_num = (float)(src_pi->end - src_pi->start);
				}

				if (src->bit_mask & PPI_MASK_BLENDINFO_SRCFRAMENUM) {
					if (src_frame_num > src->blend_info.src_frame_num)
						src_frame_num = src->blend_info.src_frame_num;
				}
				if (src->bit_mask & PPI_MASK_BLENDINFO_DSTFRAMENUM) {
					if (dst_frame_num > src->blend_info.dst_frame_num)
						dst_frame_num = src->blend_info.dst_frame_num;
				}

				float op_frame_len = min(src_frame_num, dst_frame_num);

				if (op_frame_len > 0.0f) {
					dst->blend_info.op_state = 1;
					dst->blend_info.op_frame_length = op_frame_len;
					dst->blend_info.weight = 0.0f;
					dst->blend_info.factor = 1.0f / dst->blend_info.op_frame_length;

					dst->blend_info.blend_pose = dst->pose;
					dst->blend_info.blend_type = dst->type;
					dst->blend_info.blend_frame = dst->frame;
				}
				else {
					dst->blend_info.op_state = 0;
				}
			}

			int smooth_flag = 0;

			if (src->bit_mask & PPI_MASK_TYPE) {
				switch (src->type) {
				case PLAY_ONCE_SMOOTH:
					dst->type = PLAY_ONCE;
					smooth_flag = 1;
					break;
				case PLAY_LOOP_SMOOTH:
					dst->type = PLAY_LOOP;
					smooth_flag = 1;
					break;
				case PLAY_PAUSE:
					dst->data = dst->type;
					dst->type = PLAY_FRAME;
					break;
				case PLAY_CONTINUE:
					dst->type = dst->data;
					break;
				default:
					dst->type = src->type;
				}
			}

			if (smooth_flag == 1) {
				if (src->bit_mask & PPI_MASK_POSE) {
					if (dst->pose != src->pose) {
						dst->pose = src->pose;

						if (src->bit_mask & PPI_MASK_FRAME) {
							dst->frame = src->frame;
						}
					}
					else if (dst->ret_frame >= ctrl->GetPoseInfo(dst->pose)->end) {
						if (src->bit_mask & PPI_MASK_FRAME) {
							dst->frame = src->frame;
						}
						else {
							dst->frame = 0;
						}
					}
				}
				// smooth type cannot modify frame variable arbitrarily
				//else
				//{
				//    if(src->bit_mask & PPI_MASK_FRAME)
				//    {
				//        dst->frame = src->frame;
				//    }
				//}
			}
			else {
				if (src->bit_mask & PPI_MASK_POSE) {
					dst->pose = src->pose;
				}
				if (src->bit_mask & PPI_MASK_FRAME) {
					dst->frame = src->frame;
				}
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

LW_END
