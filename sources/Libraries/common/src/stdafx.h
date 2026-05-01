#pragma once

// PCH для Common.vcxproj — собирает тяжёлые заголовки, которые
// транзитивно включаются в десятки TU через TableData.h и GameRecordset.h.

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <tchar.h>

// C runtime
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

// STL
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <array>
#include <memory>
#include <algorithm>
#include <functional>
#include <tuple>
#include <optional>
#include <chrono>
#include <source_location>
#include <format>
#include <charconv>
#include <sstream>
#include <fstream>

// Project — Util
#include "util.h"
#include "util2.h"
#include "logutil.h"

// Project — Common core (включается в 43-44 заголовка Common)
#include "SqliteDatabase.h"
#include "GameRecordset.h"
#include "TableData.h"
