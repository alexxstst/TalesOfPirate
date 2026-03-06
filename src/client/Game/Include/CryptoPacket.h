#pragma once

#include "base64.h"
#include "osrng.h"
#include "rsa.h"
#include "gcm.h"

extern CryptoPP::SecByteBlock cliPrivateKey;
extern CryptoPP::RSA::PublicKey   srvPublicKey;
extern CryptoPP::AutoSeededRandomPool rng;

