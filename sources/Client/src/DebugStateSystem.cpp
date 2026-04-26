#include "stdafx.h"
#include "DebugStateSystem.h"

DebugStateSystem& DebugStateSystem::Instance() {
	static DebugStateSystem instance;
	return instance;
}

DebugStateSystem::DebugStateSystem() = default;

void DebugStateSystem::Set(Category cat, int x, int y, std::string_view text) {
	const auto idx = static_cast<std::size_t>(cat);
	if (idx >= _categories.size()) {
		return;
	}
	auto& state = _categories[idx];
	const int key = MakeKey(x, y);
	auto& entry = state.entries[key];
	entry.x = x;
	entry.y = y;
	entry.text.assign(text);
}

void DebugStateSystem::SetEnabled(Category cat, bool enabled) {
	const auto idx = static_cast<std::size_t>(cat);
	if (idx >= _categories.size()) {
		return;
	}
	_categories[idx].enabled = enabled;
}

bool DebugStateSystem::IsEnabled(Category cat) const {
	const auto idx = static_cast<std::size_t>(cat);
	if (idx >= _categories.size()) {
		return false;
	}
	return _categories[idx].enabled;
}

void DebugStateSystem::Clear(Category cat) {
	const auto idx = static_cast<std::size_t>(cat);
	if (idx >= _categories.size()) {
		return;
	}
	_categories[idx].entries.clear();
}

void DebugStateSystem::ClearAll() {
	for (auto& state : _categories) {
		state.entries.clear();
	}
}

void DebugStateSystem::ForEachVisible(const std::function<void(const Entry&)>& visitor) const {
	if (!visitor) {
		return;
	}
	for (const auto& state : _categories) {
		if (!state.enabled) {
			continue;
		}
		for (const auto& [key, entry] : state.entries) {
			visitor(entry);
		}
	}
}

void DebugStateSystem::ForEach(Category cat, const std::function<void(const Entry&)>& visitor) const {
	const auto idx = static_cast<std::size_t>(cat);
	if (idx >= _categories.size() || !visitor) {
		return;
	}
	for (const auto& [key, entry] : _categories[idx].entries) {
		visitor(entry);
	}
}
