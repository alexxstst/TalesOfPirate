#pragma once

#include "LanguageRecordStore.h"

#define RES_STRING(a) LanguageRecordStore::Instance()->GetKeyString("" #a "").c_str()

// RES_FORMAT_STRING удалён — использовал ICU CFormatParameter.
// Оставшиеся 5 call sites в GameServer (Weather, CharacterCmd, CharStall, CharTrade)
// нужно переписать на std::format + GetKeyString.

const char* ConvertResString(const char* str);
