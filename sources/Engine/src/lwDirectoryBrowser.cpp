//
#include "stdafx.h"

#include "lwDirectoryBrowser.h"

LW_BEGIN
	// lwDirectoryBrowser
	LW_STD_IMPLEMENTATION(lwDirectoryBrowser)

	lwDirectoryBrowser::lwDirectoryBrowser()
		: _proc(0), _param(0) {
	}

	LW_RESULT lwDirectoryBrowser::_Go(const char* file, DWORD flag) {
		LW_RESULT ret = LW_RET_OK;

		WIN32_FIND_DATA wfd;

		HANDLE handle = ::FindFirstFile(file, &wfd);

		if (handle == INVALID_HANDLE_VALUE)
			goto __ret;

		{
			std::string file_path{file};
			const auto sep = file_path.rfind('\\');
			if (sep == std::string::npos)
				goto __ret;

			const std::string file_spec = file_path.substr(sep + 1);
			file_path.resize(sep + 1);

			do {
				if (wfd.cFileName[0] == '.') {
					if ((wfd.cFileName[1] == '\0')
						|| (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0')) {
						continue;
					}
				}

				if ((!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (flag & DIR_BROWSE_FILE)) {
					if (LW_RESULT r = (*_proc)(file_path.c_str(), &wfd, _param); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] callback (file) returned failure: file_path={}, name={}, ret={}",
									 __FUNCTION__, file_path, wfd.cFileName, static_cast<long long>(r));
						ret = LW_RET_OK_1;
						goto __ret;
					}
				}
				else if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (flag & DIR_BROWSE_DIRECTORY)) {
					if (LW_RESULT r = (*_proc)(file_path.c_str(), &wfd, _param); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] callback (directory) returned failure: file_path={}, name={}, ret={}",
									 __FUNCTION__, file_path, wfd.cFileName, static_cast<long long>(r));
						ret = LW_RET_OK_1;
						goto __ret;
					}

					const std::string sub_file = std::format("{}{}\\{}", file_path, wfd.cFileName, file_spec);

					if ((ret = _Go(sub_file.c_str(), flag)) == LW_RET_OK_1)
						goto __ret;
				}
			}
			while (::FindNextFile(handle, &wfd));
		}
	__ret:
		::FindClose(handle);

		return ret;
	}

	LW_RESULT lwDirectoryBrowser::Browse(const char* file, DWORD flag) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_proc == 0)
			goto __ret;


		ret = _Go(file, flag);

	__ret:
		return ret;
	}

LW_END
