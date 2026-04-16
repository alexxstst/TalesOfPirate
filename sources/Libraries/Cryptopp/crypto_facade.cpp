#include "pch.h"
#include "crypto_facade.h"

#include "aes.h"
#include "blake2.h"
#include "cryptlib.h"
#include "files.h"
#include "filters.h"
#include "gcm.h"
#include "hex.h"

namespace {
	constexpr size_t kKeyBytes = 16;
	constexpr size_t kIvBytes = 16;
	constexpr int kTagBytes = 12;
} // namespace

namespace crypto {

	std::string AesGcmEncrypt(const uint8_t* data, size_t size,
							  const uint8_t* key16, const uint8_t* iv16) {
		CryptoPP::GCM<CryptoPP::AES>::Encryption e;
		e.SetKeyWithIV(key16, kKeyBytes, iv16, kIvBytes);

		std::string out;
		CryptoPP::StringSource ss(
			data, size, true,
			new CryptoPP::AuthenticatedEncryptionFilter(
				e, new CryptoPP::StringSink(out), false, kTagBytes));
		return out;
	}

	std::string AesGcmDecrypt(const uint8_t* data, size_t size,
							  const uint8_t* key16, const uint8_t* iv16) {
		CryptoPP::GCM<CryptoPP::AES>::Decryption d;
		d.SetKeyWithIV(key16, kKeyBytes, iv16, kIvBytes);

		std::string out;
		CryptoPP::AuthenticatedDecryptionFilter df(
			d, new CryptoPP::StringSink(out),
			CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
			kTagBytes);
		CryptoPP::StringSource ss(data, size, true, new CryptoPP::Redirector(df));
		return out;
	}

	bool AesGcmDecryptFile(const std::string& srcPath, const std::string& dstPath,
						   const uint8_t* key16, const uint8_t* iv16) {
		try {
			CryptoPP::GCM<CryptoPP::AES>::Decryption d;
			d.SetKeyWithIV(key16, kKeyBytes, iv16, kIvBytes);
			CryptoPP::FileSource(
				srcPath.c_str(), true,
				new CryptoPP::AuthenticatedDecryptionFilter(
					d, new CryptoPP::FileSink(dstPath.c_str())));
			return true;
		}
		catch (...) {
			return false;
		}
	}

	std::string Blake2sHex(std::string_view input) {
		CryptoPP::BLAKE2s digest;
		std::string hashed;
		std::string hex;
		CryptoPP::StringSource(
			reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size(), true,
			new CryptoPP::HashFilter(digest, new CryptoPP::StringSink(hashed)));
		CryptoPP::StringSource(
			hashed, true,
			new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex)));
		return hex;
	}

} // namespace crypto
