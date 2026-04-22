// RecordScript.h
//---------------------------------------------------------
// Регистрация C++ сторов Item/Character/Skill/SkillState в Lua через LuaBridge.
// Функции:
//   GetItemRecord(id)            -> CItemRecord* | nil
//   GetChaRecord(id)             -> CChaRecord* | nil
//   GetSkillRecord(id)           -> CSkillRecord* | nil
//   GetSkillStateRecord(id)      -> CSkillStateRecord* | nil
//   IterateItemRecords(fn)       -- fn вызывается для каждой записи
//   IterateChaRecords(fn)
//   IterateSkillRecords(fn)
//   IterateSkillStateRecords(fn)
// Используется для миграции Lua-скриптов с парсинга .txt через io.open
// на прямой доступ к сторам (см. lua_txt_column_mapping.md).
//---------------------------------------------------------
#pragma once

#ifndef _RECORDSCRIPT_H_
#define _RECORDSCRIPT_H_

extern bool RegisterRecordScript();

#endif // _RECORDSCRIPT_H_
