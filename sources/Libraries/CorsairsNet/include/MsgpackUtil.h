#pragma once

// MsgpackUtil.h — вспомогательные функции для работы с msgpack payload.
// Портировано из Kraken-Next/server/Kraken.Util/Source/Commands.cpp.

#include "mpack.h"
#include <cstdint>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

namespace net {

// Пропустить один msgpack-элемент (data после tag).
inline void mpack_skip_element_data(mpack_reader_t& reader, mpack_tag_t tag) {
    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:
    case mpack_type_bool:
    case mpack_type_int:
    case mpack_type_uint:
    case mpack_type_double:
    case mpack_type_float:
        break;

    case mpack_type_bin: {
        auto length = mpack_tag_bin_length(&tag);
        mpack_read_bytes_inplace(&reader, length);
        mpack_done_bin(&reader);
    } break;

    case mpack_type_str: {
        auto length = mpack_tag_str_length(&tag);
        mpack_read_bytes_inplace(&reader, length);
        mpack_done_str(&reader);
    } break;

    case mpack_type_array:
        mpack_done_array(&reader);
        break;
    case mpack_type_map:
        mpack_done_map(&reader);
        break;

    default:
        throw std::runtime_error(
            "mpack_skip_element_data: unsupported type " +
            std::to_string(static_cast<int>(mpack_tag_type(&tag))));
    }
}

// Подсчёт количества msgpack-элементов верхнего уровня в буфере.
inline uint32_t mpack_sequence_length(const char* buffer, uint32_t length) {
    uint32_t result = 0;
    if (buffer && length > 0) {
        mpack_reader_t reader{};
        mpack_reader_init_data(&reader, buffer, length);

        while (true) {
            mpack_tag_t tag = mpack_read_tag(&reader);
            if (mpack_reader_error(&reader) != mpack_ok)
                break;
            ++result;
            mpack_skip_element_data(reader, tag);
        }

        mpack_reader_destroy(&reader);
    }
    return result;
}

// Оставить первые (total - count) элементов, обнулить остальные.
// Возвращает новую длину данных (байт).
inline uint32_t mpack_discard_last(char* buffer, uint32_t length, uint32_t count) {
    auto realCount = mpack_sequence_length(buffer, length);
    if (realCount < count) {
        throw std::runtime_error("mpack_discard_last: count > total elements");
    }

    mpack_reader_t reader{};
    mpack_reader_init_data(&reader, buffer, length);

    uint32_t keep = realCount - count;
    for (uint32_t i = 0; i < keep; ++i) {
        mpack_tag_t tag = mpack_read_tag(&reader);
        mpack_skip_element_data(reader, tag);
    }

    uint32_t result = static_cast<uint32_t>(reader.data - buffer);
    std::memset(buffer + result, 0, length - result);
    mpack_reader_destroy(&reader);

    return result;
}

// Пропустить paramCount элементов, вернуть указатель на следующий.
inline char* mpack_skeep_params(const char* buffer, uint32_t length, uint32_t paramCount) {
    mpack_reader_t reader{};
    mpack_reader_init_data(&reader, buffer, length);

    for (uint32_t i = 0; i < paramCount; ++i) {
        mpack_tag_t tag = mpack_read_tag(&reader);
        if (mpack_reader_error(&reader) != mpack_ok) {
            mpack_reader_destroy(&reader);
            throw std::runtime_error(
                "mpack_skeep_params: can't parse element at index " + std::to_string(i));
        }
        mpack_skip_element_data(reader, tag);
    }

    char* elementPtr = const_cast<char*>(reader.data);
    mpack_reader_destroy(&reader);
    return elementPtr;
}

// Представить msgpack payload в виде строки: |int_value|str(len)data|bin(len)|...
inline std::string mpack_sequence_to_string(const char* buffer, uint32_t length) {
    if (!buffer || length == 0) return {};

    std::stringstream stream{};
    mpack_reader_t reader{};

    mpack_reader_init_data(&reader, buffer, length);

    for (auto tag = mpack_read_tag(&reader);
         mpack_reader_error(&reader) == mpack_ok;
         tag = mpack_read_tag(&reader)) {
        switch (mpack_tag_type(&tag)) {
        case mpack_type_nil:
            stream << "|nil";
            break;

        case mpack_type_bool:
            stream << "|" << mpack_tag_bool_value(&tag);
            break;

        case mpack_type_int:
            stream << "|" << mpack_tag_int_value(&tag);
            break;

        case mpack_type_uint:
            stream << "|" << mpack_tag_uint_value(&tag);
            break;

        case mpack_type_double:
            stream << "|" << mpack_tag_double_value(&tag);
            break;

        case mpack_type_float:
            stream << "|" << mpack_tag_float_value(&tag);
            break;

        case mpack_type_bin: {
            auto len = mpack_tag_bin_length(&tag);
            stream << "|bin(" << len << ")";
            mpack_read_bytes_inplace(&reader, len);
            mpack_done_bin(&reader);
        } break;

        case mpack_type_str: {
            auto len = mpack_tag_str_length(&tag);
            auto data = mpack_read_bytes_inplace(&reader, len);
            if (data) {
                // Строки содержат null-terminator — выводим без него
                std::string value(data, len > 0 ? len - 1 : 0);
                stream << "|str(" << len << ")" << value;
            } else {
                stream << "|str(" << len << ")null!!";
            }
            mpack_done_str(&reader);
        } break;

        default:
            mpack_reader_destroy(&reader);
            throw std::runtime_error(
                "mpack_sequence_to_string: unsupported type " +
                std::to_string(static_cast<int>(mpack_tag_type(&tag))));
        }
    }

    mpack_reader_destroy(&reader);
    return stream.str();
}

} // namespace net
