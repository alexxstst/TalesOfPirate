#include "IniFile.h"
#include "excp.h"
#include <fstream>
#include <filesystem>
#include <charconv>

namespace dbc {

static std::string_view TrimView(std::string_view sv) {
	while (!sv.empty() && (sv.front() == ' ' || sv.front() == '\t' || sv.front() == '\r' || sv.front() == '\n'))
		sv.remove_prefix(1);
	while (!sv.empty() && (sv.back() == ' ' || sv.back() == '\t' || sv.back() == '\r' || sv.back() == '\n'))
		sv.remove_suffix(1);
	return sv;
}

// ============================================================================
// IniFile
// ============================================================================

IniFile::IniFile(std::string_view filename) {
	if (!filename.empty())
		ReadFile(filename);
}

void IniFile::ReadFile(std::string_view filename) {
	m_filename = filename;

	std::ifstream file(m_filename);
	if (!file.is_open())
		THROW_EXCP(excpFile, "Не удалось открыть: " + m_filename);

	std::string line;
	int lineNo = 0;
	IniSection* currentSection = nullptr;

	while (std::getline(file, line)) {
		lineNo++;
		auto trimmed = TrimView(line);

		if (trimmed.empty() || trimmed.front() == ';' || trimmed.front() == '#' || trimmed.front() == '/')
			continue;

		if (trimmed.front() == '[') {
			auto closing = trimmed.find(']');
			if (closing == std::string_view::npos)
				THROW_EXCP(excpFile, "INI:" + std::to_string(lineNo) + " — нет закрывающей ]");

			auto sectName = TrimView(trimmed.substr(1, closing - 1));
			currentSection = &(*this)[sectName];
		}
		else {
			auto eqPos = trimmed.find('=');
			if (eqPos == std::string_view::npos)
				THROW_EXCP(excpFile, "INI:" + std::to_string(lineNo) + " — ошибка формата");

			// Ключи до первой секции попадают в дефолтную секцию ""
			if (!currentSection)
				currentSection = &(*this)[""];

			auto key = TrimView(trimmed.substr(0, eqPos));
			auto val = trimmed.substr(eqPos + 1);

			if (auto commentPos = val.find("//"); commentPos != std::string_view::npos)
				val = val.substr(0, commentPos);

			val = TrimView(val);

			if (key.empty())
				THROW_EXCP(excpFile, "INI:" + std::to_string(lineNo) + " — пустой ключ");

			currentSection->SetString(key, val);
		}
	}
}

IniSection& IniFile::operator[](std::string_view sectname) {
	for (auto& sect : m_sections)
		if (sect.m_name == sectname)
			return sect;

	auto& sect = m_sections.emplace_back();
	sect.m_name = sectname;
	return sect;
}

void IniFile::Save(std::string_view filename) const {
	auto path = filename.empty() ? m_filename : std::string(filename);
	if (path.empty())
		THROW_EXCP(excpFile, "IniFile::Save — путь не указан");

	if (auto parent = std::filesystem::path(path).parent_path(); !parent.empty())
		std::filesystem::create_directories(parent);

	std::ofstream file(path);
	if (!file.is_open())
		THROW_EXCP(excpFile, "Не удалось записать: " + path);

	for (const auto& sect : m_sections) {
		file << '[' << sect.m_name << "]\n";
		for (const auto& item : sect.m_items)
			file << item.name << " = " << item.value << '\n';
		file << '\n';
	}
}

// ============================================================================
// IniSection
// ============================================================================

IniItem* IniSection::FindItem(std::string_view key) {
	for (auto& item : m_items)
		if (item.name == key)
			return &item;
	return nullptr;
}

const IniItem* IniSection::FindItem(std::string_view key) const {
	for (auto& item : m_items)
		if (item.name == key)
			return &item;
	return nullptr;
}

std::string IniSection::GetString(std::string_view key, std::string_view defaultValue) const {
	if (auto* item = FindItem(key))
		return item->value;
	return std::string(defaultValue);
}

int64_t IniSection::GetInt64(std::string_view key, int64_t defaultValue) const {
	auto* item = FindItem(key);
	if (!item)
		return defaultValue;

	int64_t result = defaultValue;
	std::from_chars(item->value.data(), item->value.data() + item->value.size(), result);
	return result;
}

void IniSection::SetString(std::string_view key, std::string_view value) {
	if (auto* item = FindItem(key)) {
		item->value = value;
		return;
	}
	m_items.push_back({std::string(key), std::string(value)});
}

void IniSection::SetInt64(std::string_view key, int64_t value) {
	SetString(key, std::to_string(value));
}

} // namespace dbc
