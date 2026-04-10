// KitbagMsgpack.cpp — Msgpack сериализация китбагов (v115)

#include "Kitbag.h"
#include "algo.h"
#include "mpack.h"
#include <vector>
#include <format>

std::string KitbagData2Msgpack(CKitbag* pKitbag) {
	if (!pKitbag) return {};

	char* data = nullptr;
	size_t size = 0;
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, &data, &size);

	mpack_start_map(&writer, 4);
	mpack_write_cstr(&writer, "v"); mpack_write_i32(&writer, KITBAG_VERSION_MSGPACK);
	mpack_write_cstr(&writer, "cap"); mpack_write_i32(&writer, pKitbag->GetCapacity());

	__int64 checksum = 0;

	mpack_write_cstr(&writer, "items");
	mpack_start_array(&writer, enumKBITEM_TYPE_NUM);

	for (int type = 0; type < enumKBITEM_TYPE_NUM; type++) {
		short usedNum = pKitbag->GetUseGridNum(type);
		mpack_start_map(&writer, 2);
		mpack_write_cstr(&writer, "u"); mpack_write_i32(&writer, usedNum);
		mpack_write_cstr(&writer, "s");
		mpack_start_array(&writer, usedNum);

		for (int j = 0; j < pKitbag->GetCapacity(); j++) {
			SItemGrid* g = pKitbag->GetGridContByID(j, type);
			if (!g) continue;

			bool hasAttr = g->IsInstAttrValid();
			mpack_start_map(&writer, hasAttr ? 13 : 12);

			mpack_write_cstr(&writer, "p"); mpack_write_i32(&writer, j);
			mpack_write_cstr(&writer, "i"); mpack_write_i32(&writer, g->sID);
			mpack_write_cstr(&writer, "n"); mpack_write_i32(&writer, g->sNum);
			mpack_write_cstr(&writer, "e");
			mpack_start_array(&writer, 2); mpack_write_i32(&writer, g->sEndure[0]); mpack_write_i32(&writer, g->sEndure[1]); mpack_finish_array(&writer);
			mpack_write_cstr(&writer, "g");
			mpack_start_array(&writer, 2); mpack_write_i32(&writer, g->sEnergy[0]); mpack_write_i32(&writer, g->sEnergy[1]); mpack_finish_array(&writer);
			mpack_write_cstr(&writer, "f"); mpack_write_i32(&writer, g->chForgeLv);
			mpack_write_cstr(&writer, "d"); mpack_write_u32(&writer, g->dwDBID);
			mpack_write_cstr(&writer, "l"); mpack_write_i32(&writer, g->sNeedLv);
			mpack_write_cstr(&writer, "k"); mpack_write_bool(&writer, g->bIsLock);
			mpack_write_cstr(&writer, "t"); mpack_write_bool(&writer, g->bItemTradable);
			mpack_write_cstr(&writer, "x"); mpack_write_i32(&writer, g->expiration);
			mpack_write_cstr(&writer, "b");
			mpack_start_array(&writer, enumITEMDBP_MAXNUM);
			for (int m = 0; m < enumITEMDBP_MAXNUM; m++) {
				mpack_write_i32(&writer, g->GetDBParam(m));
				checksum += g->GetDBParam(m);
			}
			mpack_finish_array(&writer);

			checksum += g->sID + g->sNum + g->sEndure[0] + g->sEndure[1]
				+ g->sEnergy[0] + g->sEnergy[1] + g->chForgeLv
				+ g->dwDBID + g->sNeedLv + g->bIsLock + g->bItemTradable;

			if (hasAttr) {
				mpack_write_cstr(&writer, "a");
				mpack_start_array(&writer, defITEM_INSTANCE_ATTR_NUM);
				for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++) {
					mpack_start_array(&writer, 2);
					mpack_write_i32(&writer, g->sInstAttr[k][0]); mpack_write_i32(&writer, g->sInstAttr[k][1]);
					mpack_finish_array(&writer);
					checksum += g->sInstAttr[k][0] + g->sInstAttr[k][1];
				}
				mpack_finish_array(&writer);
			}
			mpack_finish_map(&writer);
		}
		mpack_finish_array(&writer);
		mpack_finish_map(&writer);
	}
	mpack_finish_array(&writer);

	mpack_write_cstr(&writer, "chk"); mpack_write_i64(&writer, checksum);
	mpack_finish_map(&writer);

	if (mpack_writer_destroy(&writer) != mpack_ok) { free(data); return {}; }

	unsigned int b64Len = 0;
	std::vector<char> b64Buf(size * 2 + 4);
	base64(data, static_cast<unsigned int>(size), b64Buf.data(), static_cast<unsigned int>(b64Buf.size()), &b64Len);
	free(data);

	return std::format("{}@{}#{}", pKitbag->GetCapacity(), KITBAG_VERSION_MSGPACK, std::string_view(b64Buf.data(), b64Len));
}

bool Msgpack2KitbagData(CKitbag* pKitbag, const char* b64Data, size_t b64Len) {
	if (!pKitbag || !b64Data || b64Len == 0) return false;

	unsigned int binLen = 0;
	std::vector<char> binBuf(b64Len);
	ibase64(b64Data, static_cast<unsigned int>(b64Len), binBuf.data(), &binLen);

	mpack_reader_t reader;
	mpack_reader_init_data(&reader, binBuf.data(), binLen);

	uint32_t rootCount = mpack_expect_map(&reader);
	for (uint32_t r = 0; r < rootCount; r++) {
		char key[8]{}; mpack_expect_cstr(&reader, key, sizeof(key));

		if (!strcmp(key, "v")) { mpack_expect_i32(&reader); }
		else if (!strcmp(key, "cap")) { pKitbag->SetCapacity(static_cast<short>(mpack_expect_i32(&reader))); }
		else if (!strcmp(key, "items")) {
			uint32_t typeCount = mpack_expect_array(&reader);
			for (uint32_t type = 0; type < typeCount; type++) {
				uint32_t tm = mpack_expect_map(&reader);
				for (uint32_t ti = 0; ti < tm; ti++) {
					char tkey[4]{}; mpack_expect_cstr(&reader, tkey, sizeof(tkey));
					if (!strcmp(tkey, "u")) { mpack_expect_i32(&reader); }
					else if (!strcmp(tkey, "s")) {
						uint32_t slotCount = mpack_expect_array(&reader);
						for (uint32_t s = 0; s < slotCount; s++) {
							SItemGrid grid{}; int pos = 0;
							uint32_t fc = mpack_expect_map(&reader);
							for (uint32_t f = 0; f < fc; f++) {
								char fk[4]{}; mpack_expect_cstr(&reader, fk, sizeof(fk));
								if (!strcmp(fk, "p")) pos = mpack_expect_i32(&reader);
								else if (!strcmp(fk, "i")) grid.sID = static_cast<short>(mpack_expect_i32(&reader));
								else if (!strcmp(fk, "n")) grid.sNum = static_cast<short>(mpack_expect_i32(&reader));
								else if (!strcmp(fk, "e")) {
									mpack_expect_array_match(&reader, 2);
									grid.sEndure[0] = static_cast<short>(mpack_expect_i32(&reader));
									grid.sEndure[1] = static_cast<short>(mpack_expect_i32(&reader));
									mpack_done_array(&reader);
								}
								else if (!strcmp(fk, "g")) {
									mpack_expect_array_match(&reader, 2);
									grid.sEnergy[0] = static_cast<short>(mpack_expect_i32(&reader));
									grid.sEnergy[1] = static_cast<short>(mpack_expect_i32(&reader));
									mpack_done_array(&reader);
								}
								else if (!strcmp(fk, "f")) grid.chForgeLv = static_cast<char>(mpack_expect_i32(&reader));
								else if (!strcmp(fk, "d")) grid.dwDBID = mpack_expect_u32(&reader);
								else if (!strcmp(fk, "l")) grid.sNeedLv = static_cast<short>(mpack_expect_i32(&reader));
								else if (!strcmp(fk, "k")) grid.bIsLock = mpack_expect_bool(&reader);
								else if (!strcmp(fk, "t")) grid.bItemTradable = mpack_expect_bool(&reader);
								else if (!strcmp(fk, "x")) grid.expiration = mpack_expect_i32(&reader);
								else if (!strcmp(fk, "b")) {
									uint32_t dp = mpack_expect_array(&reader);
									for (uint32_t d = 0; d < dp && d < enumITEMDBP_MAXNUM; d++) grid.SetDBParam(d, mpack_expect_i32(&reader));
									mpack_done_array(&reader);
								}
								else if (!strcmp(fk, "a")) {
									uint32_t ac = mpack_expect_array(&reader);
									for (uint32_t a = 0; a < ac && a < defITEM_INSTANCE_ATTR_NUM; a++) {
										mpack_expect_array_match(&reader, 2);
										grid.sInstAttr[a][0] = static_cast<short>(mpack_expect_i32(&reader));
										grid.sInstAttr[a][1] = static_cast<short>(mpack_expect_i32(&reader));
										mpack_done_array(&reader);
									}
									mpack_done_array(&reader);
								}
								else { mpack_discard(&reader); }
							}
							mpack_done_map(&reader);
							grid.SetValid(true);
							short sPos = static_cast<short>(pos);
							pKitbag->Push(&grid, sPos, type);
						}
						mpack_done_array(&reader);
					} else { mpack_discard(&reader); }
				}
				mpack_done_map(&reader);
			}
			mpack_done_array(&reader);
		}
		else if (!strcmp(key, "chk")) { mpack_expect_i64(&reader); }
		else { mpack_discard(&reader); }
	}
	mpack_done_map(&reader);

	return mpack_reader_destroy(&reader) == mpack_ok;
}
