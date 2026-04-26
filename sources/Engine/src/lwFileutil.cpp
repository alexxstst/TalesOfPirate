#include "stdafx.h"
#include "lwfileutil.h"

std::string lwGetPathFileNameExt(std::string_view path) {
	const auto pos = path.rfind('.');
	if (pos == std::string_view::npos) {
		return {};
	}
	return std::string{path.substr(pos + 1)};
}
