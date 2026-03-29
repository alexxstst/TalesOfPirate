#include "stdafx.h"
#include "CryptoUtils.h"

#include "blake2.h"
#include "hex.h"
#include "filters.h"

std::string HashPassword(const std::string& password)
{
    std::string digest;
    std::string hexencoded;
    CryptoPP::BLAKE2s d;
    CryptoPP::StringSource(password, true, new CryptoPP::HashFilter(d, new CryptoPP::StringSink(digest)));
    CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexencoded)));
    return hexencoded;
}
