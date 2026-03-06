#include "MPTextureSet.h"

#include <cryptlib.h>
#include "aes.h"
#include "filters.h"
#include "files.h"
#include "gcm.h"

void MPTexSet::CryptImage(bool encrypt, char* filename, char* sinkbuffer) {
	const unsigned char imgTableKey[] = { 0x48, 0x73, 0x29, 0xCA, 0xBB, 0x54, 0xCF, 0xB0, 0xF4, 0xBF, 0x70, 0xA0, 0xAA, 0x4B, 0x12, 0xF5 };
	const unsigned char imgTableIV[] = { 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	try {
		if (encrypt) {
			std::string encFilename = filename;
			encFilename.replace(encFilename.length() - 3, 3, "wsd");
			CryptoPP::GCM<CryptoPP::AES>::Encryption e;
			e.SetKeyWithIV(imgTableKey, 16, imgTableIV, 16);
			CryptoPP::FileSource(filename, true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::FileSink(encFilename.c_str())));
			return;
		}
		else {
			CryptoPP::GCM<CryptoPP::AES>::Decryption d;
			d.SetKeyWithIV(imgTableKey, 16, imgTableIV, 16);
			std::string buffer;
			CryptoPP::FileSource(filename, true, new CryptoPP::AuthenticatedDecryptionFilter(d, new CryptoPP::StringSink(buffer)));
			memcpy(sinkbuffer, buffer.data(), buffer.size());
			return;
		}
	}
	catch (...) {}
}
