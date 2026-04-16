#pragma once

// Тонкий std-only фасад над CryptoPP.
// Цель: спрятать тяжёлые заголовки CryptoPP в .cpp, чтобы потребителям не приходилось
// тянуть через препроцессор cryptlib.h/aes.h/gcm.h/... (десятки тысяч строк на TU).
//
// Параметры зафиксированы под текущие сценарии:
//   AES-GCM: 16-байтный ключ, 16-байтный IV, 12-байтный тег.
//   BLAKE2s: стандартная длина дайджеста, вывод в hex.

#include <cstdint>
#include <string>
#include <string_view>

namespace crypto {

	// AES-GCM шифрование бинарного буфера.
	// Возвращает cipher+tag одной строкой (как StringSink CryptoPP).
	// Исключения CryptoPP пробрасываются наружу.
	std::string AesGcmEncrypt(const uint8_t* data, size_t size,
							  const uint8_t* key16, const uint8_t* iv16);

	// AES-GCM расшифровка бинарного буфера (cipher+tag).
	// Возвращает plaintext; при неверной аутентификации бросает исключение.
	std::string AesGcmDecrypt(const uint8_t* data, size_t size,
							  const uint8_t* key16, const uint8_t* iv16);

	// AES-GCM файл → файл. Возвращает true при успехе (включая успешную аутентификацию).
	// Логи/сообщения об ошибках оставлены на потребителе.
	bool AesGcmDecryptFile(const std::string& srcPath, const std::string& dstPath,
						   const uint8_t* key16, const uint8_t* iv16);

	// BLAKE2s хеш UTF-8 строки, результат — hex (uppercase).
	std::string Blake2sHex(std::string_view input);

} // namespace crypto
