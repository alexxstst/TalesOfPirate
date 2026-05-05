#include "stdafx.h"
#include "CryptoUtils.h"

#include "Blake2s.h"

std::string HashPassword(const std::string& password) {
	return Corsairs::Common::Crypto::Blake2sHex(password);
}
