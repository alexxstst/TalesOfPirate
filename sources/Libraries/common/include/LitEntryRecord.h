#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Запись lit-эффекта (одна строка из lit.tx).
//
// Значимые поля зависят от obj_type:
//   0 (character): anim_type, file, mask, textures
//   1 (scene),
//   2 (item):     anim_type, file, sub_id, color_op, textures
struct LitEntryRecord {
	int32_t _obj_type{0};                  // 0=character, 1=scene, 2=item
	int32_t _anim_type{0};                 // 0..4 — режим анимации
	std::string _file{};                   // имя .lgo (ключ поиска)
	std::string _mask{};                   // только для obj_type=0
	int32_t _sub_id{0};                    // только для obj_type=1,2
	int32_t _color_op{0};                  // D3DTOP_*, только для obj_type=1,2
	std::vector<std::string> _textures{};  // имена текстур (до 8)
};
