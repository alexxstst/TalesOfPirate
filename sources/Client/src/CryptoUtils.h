#pragma once

#include <string>

// BLAKE2s hash → hex string (used for password hashing)
std::string HashPassword(const std::string& password);
