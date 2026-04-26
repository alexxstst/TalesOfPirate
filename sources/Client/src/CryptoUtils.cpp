#include "stdafx.h"
#include "CryptoUtils.h"

#include "crypto_facade.h"

std::string HashPassword(const std::string& password) {
	return crypto::Blake2sHex(password);
}
