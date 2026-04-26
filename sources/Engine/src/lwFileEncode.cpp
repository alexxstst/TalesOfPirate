//
#include "stdafx.h"
#include <memory.h>
#include "lwFileEncode.h"

LW_BEGIN
	LW_RESULT lwTexEncode::_Encode0(lwIBuffer* buf) {
		const DWORD i = 44;
		BYTE tmp[i];

		BYTE* p;
		DWORD s;

		p = buf->GetData();
		s = buf->GetSize();

		if (s > i) {
			memcpy(tmp, p, i);
			memcpy(p, &p[s - i], i);
			memcpy(&p[s - i], tmp, i);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwTexEncode::_Decode0(lwIBuffer* buf) {
		return _Encode0(buf);
	}

	LW_RESULT lwTexEncode::_Encode1(lwIBuffer* buf) {
		BYTE* p;
		DWORD s;

		lwDwordByte4* sp;
		lwDwordByte4 e;
		e.b[0] = 'm';
		e.b[1] = 'p';
		e.b[2] = '.';
		e.b[3] = 'x';


		p = buf->GetData();
		s = buf->GetSize();

		sp = (lwDwordByte4*)&p[s - 4];

		if (sp->d == e.d)
			return LW_RET_OK;

		_Encode0(buf);

		buf->Realloc(s + 4);

		p = buf->GetData();
		s = buf->GetSize();

		memcpy(&p[s - 4], &e, sizeof(e));

		return LW_RET_OK;
	}

	LW_RESULT lwTexEncode::_Decode1(lwIBuffer* buf) {
		BYTE* p;
		DWORD s;

		lwDwordByte4* sp;
		lwDwordByte4 e;
		e.b[0] = 'm';
		e.b[1] = 'p';
		e.b[2] = '.';
		e.b[3] = 'x';

		p = buf->GetData();
		s = buf->GetSize();

		sp = (lwDwordByte4*)&p[s - 4];

		if (sp->d != e.d)
			return LW_RET_OK;

		buf->SetSizeArbitrary(s - 4);

		_Decode0(buf);


		return LW_RET_OK;
	}

	LW_RESULT lwTexEncode::Encode(lwIBuffer* buf) {
		return _Encode1(buf);
	}

	LW_RESULT lwTexEncode::Decode(lwIBuffer* buf) {
		return _Decode1(buf);
	}


LW_END
