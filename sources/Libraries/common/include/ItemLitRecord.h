#pragma once

#include <cstdint>
#include <string>

// Одна запись пресета свечения предмета по уровню заточки (из item.lit).
// Плоская денормализация: поля ItemDescriptor/ItemFile дублируются на каждый
// уровень того же ItemId (данных мало, ~40 items × несколько уровней).
struct ItemLitRecord {
	int32_t     ItemId{0};           // порядковый номер item в исходном item.lit
	int32_t     LitId{0};            // уровень заточки (0..N-1)
	std::string ItemDescriptor{};    // имя записи (редакторское поле уровня item)
	std::string ItemFile{};          // имя базовой модели (редакторское поле уровня item)
	std::string TexFile{};           // имя текстуры-свечения (PATH_TYPE_TEXTURE_ITEM)
	int32_t     AnimType{0};         // 0..8 — тип UV-анимации, см. __lit_proc[] в ItemLitAnim.cpp
	int32_t     TranspType{0};       // режим прозрачности
	float       Opacity{0.0f};
};
