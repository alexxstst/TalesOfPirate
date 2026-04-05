#include "Stdafx.h"

#include "CharacterAction.h"
#include <PoseDataStore.h>
#include <AssetDatabase.h>

// Данные анимаций в Engine-формате, заполняются из PoseDataStore
static std::vector<SCharacterAction> s_charActions;

static void LoadPoseDataFromStore() {
	auto* store = PoseDataStore::Instance();
	int maxType = store->GetMaxCharType();
	if (maxType < 1) {
		return;
	}

	s_charActions.clear();
	s_charActions.resize(maxType);

	for (int charType = 1; charType <= maxType; charType++) {
		int count = 0;
		const PoseDataRecord* records = store->GetActions(charType, &count);
		if (!records || count == 0) {
			continue;
		}

		auto& ct = s_charActions[charType - 1];
		ct.m_iCharacterType = static_cast<short>(charType);

		// Определяем maxActionId для размера вектора
		int maxActionId = 0;
		for (int i = 0; i < count; i++) {
			if (records[i].actionId > maxActionId) {
				maxActionId = records[i].actionId;
			}
		}

		ct.m_actions.resize(maxActionId);

		for (int i = 0; i < count; i++) {
			const auto& rec = records[i];
			if (rec.actionId < 1 || rec.actionId > maxActionId) {
				continue;
			}

			auto& action = ct.m_actions[rec.actionId - 1];
			action.m_sActionNO = static_cast<short>(rec.actionId);
			action.info.charType = 0;
			action.info.start = static_cast<DWORD>(rec.startFrame);
			action.info.end = static_cast<DWORD>(rec.endFrame);
			action.info.key_frame_num = static_cast<DWORD>(rec.keyframeCount);
			for (int k = 0; k < rec.keyframeCount && k < MAX_KEY_FRAME_NUM; k++) {
				action.info.key_frame_seq[k] = static_cast<DWORD>(rec.keyframes[k]);
			}
		}
	}
}

void InitPoseData() {
	LoadPoseDataFromStore();
}

const SCharacterAction* GetCharacterAction(int nTypeID) {
	if (nTypeID < 1 || nTypeID > static_cast<int>(s_charActions.size())) {
		return nullptr;
	}
	auto* ct = &s_charActions[nTypeID - 1];
	if (ct->m_iCharacterType < 1) {
		return nullptr;
	}
	return ct;
}
