#include "stdafx.h"
#include "CommFunc.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

// Сравнение двух stNetChangeChaPart (только поля, сериализуемые LookData2String)
static bool CompareLook(const stNetChangeChaPart &a, const stNetChangeChaPart &b)
{
	if (a.sTypeID != b.sTypeID) { std::cerr << "sTypeID mismatch\n"; return false; }
	if (a.sHairID != b.sHairID) { std::cerr << "sHairID mismatch\n"; return false; }

	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		const auto &ai = a.SLink[i];
		const auto &bi = b.SLink[i];

		if (ai.expiration != bi.expiration) { std::cerr << "slot " << i << " expiration mismatch\n"; return false; }
		if (ai.bItemTradable != bi.bItemTradable) { std::cerr << "slot " << i << " bItemTradable mismatch\n"; return false; }
		if (ai.bIsLock != bi.bIsLock) { std::cerr << "slot " << i << " bIsLock mismatch\n"; return false; }
		if (ai.sNeedLv != bi.sNeedLv) { std::cerr << "slot " << i << " sNeedLv mismatch\n"; return false; }
		if (ai.dwDBID != bi.dwDBID) { std::cerr << "slot " << i << " dwDBID mismatch\n"; return false; }
		if (ai.sID != bi.sID) { std::cerr << "slot " << i << " sID mismatch\n"; return false; }
		if (ai.sNum != bi.sNum) { std::cerr << "slot " << i << " sNum mismatch\n"; return false; }
		if (ai.sEndure[0] != bi.sEndure[0] || ai.sEndure[1] != bi.sEndure[1]) { std::cerr << "slot " << i << " sEndure mismatch\n"; return false; }
		if (ai.sEnergy[0] != bi.sEnergy[0] || ai.sEnergy[1] != bi.sEnergy[1]) { std::cerr << "slot " << i << " sEnergy mismatch\n"; return false; }
		if (ai.chForgeLv != bi.chForgeLv) { std::cerr << "slot " << i << " chForgeLv mismatch\n"; return false; }

		for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
		{
			if (ai.GetDBParam(m) != bi.GetDBParam(m)) { std::cerr << "slot " << i << " dbParam[" << m << "] mismatch\n"; return false; }
		}

		if (ai.IsInstAttrValid() != bi.IsInstAttrValid()) { std::cerr << "slot " << i << " instAttr valid mismatch\n"; return false; }
		if (ai.IsInstAttrValid())
		{
			for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
			{
				if (ai.sInstAttr[k][0] != bi.sInstAttr[k][0] || ai.sInstAttr[k][1] != bi.sInstAttr[k][1])
				{
					std::cerr << "slot " << i << " instAttr[" << k << "] mismatch\n"; return false;
				}
			}
		}
	}
	return true;
}

// Тест 1: roundtrip — пустой look (все нули)
static bool TestEmptyRoundtrip()
{
	stNetChangeChaPart original;
	memset(&original, 0, sizeof(original));
	original.sTypeID = 1;
	original.sHairID = 2000;

	std::string serialized;
	if (!LookData2String(original, serialized))
	{
		std::cerr << "TestEmptyRoundtrip: LookData2String failed\n";
		return false;
	}

	stNetChangeChaPart parsed;
	memset(&parsed, 0, sizeof(parsed));
	if (!String2LookData(parsed, serialized))
	{
		std::cerr << "TestEmptyRoundtrip: String2LookData failed\n";
		return false;
	}

	if (!CompareLook(original, parsed))
	{
		std::cerr << "TestEmptyRoundtrip: data mismatch after roundtrip\n";
		return false;
	}

	// Второй roundtrip: serialized2 должна совпасть с serialized
	std::string serialized2;
	if (!LookData2String(parsed, serialized2))
	{
		std::cerr << "TestEmptyRoundtrip: second LookData2String failed\n";
		return false;
	}
	if (serialized != serialized2)
	{
		std::cerr << "TestEmptyRoundtrip: string mismatch\n";
		std::cerr << "  first:  " << serialized.substr(0, 200) << "...\n";
		std::cerr << "  second: " << serialized2.substr(0, 200) << "...\n";
		return false;
	}

	std::cout << "TestEmptyRoundtrip: PASSED\n";
	return true;
}

// Тест 2: roundtrip с заполненными данными
static bool TestFilledRoundtrip()
{
	stNetChangeChaPart original;
	memset(&original, 0, sizeof(original));
	original.sTypeID = 3;
	original.sHairID = 2125;

	// Заполняем несколько слотов
	for (int i = 0; i < 5; i++)
	{
		auto &item = original.SLink[i];
		item.sID = static_cast<short>(1000 + i);
		item.sNum = 1;
		item.sEndure[0] = 100;
		item.sEndure[1] = 200;
		item.sEnergy[0] = 50;
		item.sEnergy[1] = 60;
		item.chForgeLv = static_cast<char>(i);
		item.expiration = 1000000L + i;
		item.bItemTradable = (i % 2 == 0);
		item.bIsLock = (i % 3 == 0);
		item.sNeedLv = static_cast<short>(10 * i);
		item.dwDBID = 5000 + i;
		item.SetDBParam(0, 42 + i);
		item.SetDBParam(1, 99 + i);
	}

	// Слот с instance-атрибутами
	auto &slotWithAttr = original.SLink[2];
	slotWithAttr.sInstAttr[0][0] = 10;
	slotWithAttr.sInstAttr[0][1] = 20;
	slotWithAttr.sInstAttr[1][0] = 30;
	slotWithAttr.sInstAttr[1][1] = 40;

	std::string serialized;
	if (!LookData2String(original, serialized))
	{
		std::cerr << "TestFilledRoundtrip: LookData2String failed\n";
		return false;
	}

	stNetChangeChaPart parsed;
	memset(&parsed, 0, sizeof(parsed));
	if (!String2LookData(parsed, serialized))
	{
		std::cerr << "TestFilledRoundtrip: String2LookData failed\n";
		return false;
	}

	if (!CompareLook(original, parsed))
	{
		std::cerr << "TestFilledRoundtrip: data mismatch\n";
		return false;
	}

	// Двойной roundtrip
	std::string serialized2;
	LookData2String(parsed, serialized2);
	if (serialized != serialized2)
	{
		std::cerr << "TestFilledRoundtrip: string mismatch on double roundtrip\n";
		return false;
	}

	std::cout << "TestFilledRoundtrip: PASSED\n";
	return true;
}

// Тест 3: проверка формата версии (начинается с "112#")
static bool TestVersionFormat()
{
	stNetChangeChaPart look{};
	look.sTypeID = 2;
	look.sHairID = 2063;

	std::string serialized;
	LookData2String(look, serialized);

	if (serialized.substr(0, 4) != "112#")
	{
		std::cerr << "TestVersionFormat: expected '112#' prefix, got: " << serialized.substr(0, 10) << "\n";
		return false;
	}

	// Проверяем, что после "112#" идёт "typeID,hairID;..."
	auto afterVer = serialized.substr(4);
	if (afterVer.substr(0, 6) != "2,2063")
	{
		std::cerr << "TestVersionFormat: expected '2,2063' after version, got: " << afterVer.substr(0, 20) << "\n";
		return false;
	}

	std::cout << "TestVersionFormat: PASSED\n";
	return true;
}

// Тест 4: совместимость с F# parseLookMinimal
// F# парсер: version=112 → sID на позиции 5 в каждой equip-секции
static bool TestFSharpCompatibility()
{
	stNetChangeChaPart original{};
	original.sTypeID = 1;
	original.sHairID = 2001;

	// Заполняем первый слот конкретным sID
	original.SLink[0].sID = 777;
	original.SLink[0].expiration = 100;
	original.SLink[0].bItemTradable = 1;
	original.SLink[0].bIsLock = 0;
	original.SLink[0].sNeedLv = 10;
	original.SLink[0].dwDBID = 999;

	std::string serialized;
	LookData2String(original, serialized);

	// Парсим формат как F#: split по ';', section[1] split по ','
	// sID должен быть на позиции 5 (0-indexed) при version=112
	auto hashPos = serialized.find('#');
	auto dataStr = serialized.substr(hashPos + 1);

	// split по ';'
	std::vector<std::string> sections;
	{
		size_t start = 0;
		while (start < dataStr.size())
		{
			auto pos = dataStr.find(';', start);
			if (pos == std::string::npos)
			{
				sections.push_back(dataStr.substr(start));
				break;
			}
			sections.push_back(dataStr.substr(start, pos - start));
			start = pos + 1;
		}
	}

	if (sections.size() < 2)
	{
		std::cerr << "TestFSharpCompatibility: not enough sections\n";
		return false;
	}

	// Section[1] = первый equip slot
	std::vector<std::string> fields;
	{
		size_t start = 0;
		auto &s = sections[1];
		while (start < s.size())
		{
			auto pos = s.find(',', start);
			if (pos == std::string::npos)
			{
				fields.push_back(s.substr(start));
				break;
			}
			fields.push_back(s.substr(start, pos - start));
			start = pos + 1;
		}
	}

	// F#: sIdIndex = if version = 112 then 5 else 2
	// fields[0]=expiration, [1]=bItemTradable, [2]=bIsLock, [3]=sNeedLv, [4]=dwDBID, [5]=sID
	if (fields.size() <= 5)
	{
		std::cerr << "TestFSharpCompatibility: not enough fields in slot\n";
		return false;
	}

	int parsedSID = std::stoi(fields[5]);
	if (parsedSID != 777)
	{
		std::cerr << "TestFSharpCompatibility: sID at index 5 expected 777, got " << parsedSID << "\n";
		return false;
	}

	std::cout << "TestFSharpCompatibility: PASSED\n";
	return true;
}

// Тест 5: F# простой формат "typeID;hairID;faceID"
static bool TestFSharpSimpleFormat()
{
	std::string fsharpLook = "1;2001;2555";

	stNetChangeChaPart parsed;
	memset(&parsed, 0, sizeof(parsed));
	if (!String2LookData(parsed, fsharpLook))
	{
		std::cerr << "TestFSharpSimpleFormat: String2LookData failed\n";
		return false;
	}

	if (parsed.sTypeID != 1)
	{
		std::cerr << "TestFSharpSimpleFormat: sTypeID expected 1, got " << parsed.sTypeID << "\n";
		return false;
	}
	if (parsed.sHairID != 2001)
	{
		std::cerr << "TestFSharpSimpleFormat: sHairID expected 2001, got " << parsed.sHairID << "\n";
		return false;
	}

	// Все слоты должны быть пустыми
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		if (parsed.SLink[i].sID != 0)
		{
			std::cerr << "TestFSharpSimpleFormat: slot " << i << " sID expected 0, got " << parsed.SLink[i].sID << "\n";
			return false;
		}
	}

	std::cout << "TestFSharpSimpleFormat: PASSED\n";
	return true;
}

// Тест 6: контрольная сумма — повреждённые данные не парсятся
static bool TestChecksumValidation()
{
	stNetChangeChaPart original{};
	original.sTypeID = 1;
	original.sHairID = 2000;

	std::string serialized;
	LookData2String(original, serialized);

	// Подменяем последний символ контрольной суммы
	std::string corrupted = serialized;
	corrupted.back() = (corrupted.back() == '0') ? '1' : '0';

	stNetChangeChaPart parsed{};
	if (String2LookData(parsed, corrupted))
	{
		std::cerr << "TestChecksumValidation: expected false for corrupted checksum\n";
		return false;
	}

	std::cout << "TestChecksumValidation: PASSED\n";
	return true;
}

int main()
{
	int passed = 0;
	int failed = 0;

	auto run = [&](bool result) { result ? ++passed : ++failed; };

	run(TestEmptyRoundtrip());
	run(TestFilledRoundtrip());
	run(TestVersionFormat());
	run(TestFSharpCompatibility());
	run(TestFSharpSimpleFormat());
	run(TestChecksumValidation());

	std::cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===\n";
	return failed > 0 ? 1 : 0;
}
