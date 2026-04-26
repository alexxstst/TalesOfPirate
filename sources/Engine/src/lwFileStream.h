//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"

LW_BEGIN
	class lwFileStream : public lwIFileStream {
		LW_STD_DECLARATION()

	private:
		lwIFile* _adapter_file;
		lwFileStreamOpenInfo _fsoi;

	private:

	public:
		lwFileStream();
		~lwFileStream();

		LW_RESULT Open(std::string_view file, const lwFileStreamOpenInfo* info);
		LW_RESULT Close();
		LW_RESULT Read(void* buf, DWORD in_size, DWORD* out_size);
		LW_RESULT Write(const void* buf, DWORD in_size, DWORD* out_size);
		LW_RESULT Seek(DWORD* pos, long offset, DWORD flag);
		LW_RESULT GetSize(DWORD* size);
		LW_RESULT Flush();
		LW_RESULT SetEnd();
	};

LW_END
