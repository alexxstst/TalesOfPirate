#pragma once

#include <string>
#include <cstdint>
#include <windows.h>

// Запись таблицы объектов сцены
class CSceneObjInfo {
public:
	int32_t _id{};
	std::string _dataName{};
	std::string _name{};
	int32_t _type{};
	uint8_t _pointColor[3]{};
	uint8_t _envColor[3]{};
	uint8_t _fogColor[3]{};
	int32_t _range{};
	float _attenuation{};
	int32_t _animCtrlId{};

	int32_t _style{};
	int32_t _attachEffectId{};
	bool _enablePointLight{false};
	bool _enableEnvLight{false};
	int32_t _flag{};
	int32_t _sizeFlag{};

	std::string _envSound{};
	int32_t _envSoundDis{};
	int32_t _photoTexId{};
	bool _shadeFlag{false};
	bool _isReallyBig{false};

	int32_t _fadeObjNum{};
	int32_t _fadeObjSeq[16]{};
	float _fadeCoefficient{};
};
