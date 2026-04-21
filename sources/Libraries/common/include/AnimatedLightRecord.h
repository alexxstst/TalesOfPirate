#pragma once

#include "TableData.h"

// Один ключевой кадр анимации источника света.
// Набор кадров, объединённый полем _lightNo, описывает одну анимацию света (AnimCtrlLight).
// _frameId — время/кадр внутри анимации (ключ для интерполяции в AnimCtrlLight::UpdateObject).
// _r/_g/_b хранятся как 0..255 (нормализация в 0.0..1.0 — в потребителе).
class CAnimatedLightInfo : public EntityData
{
public:
	short _lightNo{};
	short _keyNo{};
	int _frameId{};
	int _type{};
	int _r{};
	int _g{};
	int _b{};
	float _range{};
	float _attenuation0{};
	float _attenuation1{};
	float _attenuation2{};
};
