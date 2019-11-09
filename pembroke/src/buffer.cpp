#include "pembroke/buffer.hpp"

#include <string>

extern "C" {
    #include "event2/buffer.h"
}

namespace pembroke {
    // ---
    // Basic Constructor & Destructor
    // ---
    Buffer::Buffer() noexcept
        : m_underlying(evbuffer_new()) {}

    Buffer::~Buffer() noexcept {
        if (m_underlying != nullptr) {
            evbuffer_free(m_underlying);
            m_underlying = nullptr;
        }
    }

    // ---
    // Move Constructor / Asignment
    // ---
    Buffer::Buffer(Buffer &&b) noexcept {
        m_underlying = b.m_underlying;
        b.m_underlying = nullptr;
    }

    Buffer &Buffer::operator=(Buffer &&b) noexcept {
        if (m_underlying != nullptr) {
            // make sure to free any existing resources
            evbuffer_free(m_underlying);
        }

        m_underlying = b.m_underlying;
        b.m_underlying = nullptr;

        return *this;
    }

    // ---
    // Buffer Write Methods
    // ---

    void Buffer::add(std::string_view sv) noexcept {
        if (m_underlying == nullptr) {
            return;
        }
        evbuffer_add(m_underlying, sv.data(), sv.length());
    }

    void Buffer::add(const char *c_str, size_t n_chars) noexcept {
        if (m_underlying == nullptr) {
            return;
        }
        evbuffer_add(m_underlying, c_str, n_chars);
    }

    void Buffer::add(const std::byte *bytes, size_t n_bytes) noexcept {
        if (m_underlying == nullptr) {
            return;
        }
        evbuffer_add(m_underlying, bytes, n_bytes);
    }

    void Buffer::add(const ByteSlice &byte_slice) noexcept {
        add(byte_slice.bytes, byte_slice.len);
    }


    // ---
    // Buffer Read Methods
    // ---

    size_t Buffer::length() noexcept {
        if (m_underlying == nullptr) {
            return 0;
        }
        return evbuffer_get_length(m_underlying);
    }

    size_t Buffer::size() noexcept {
        return length();
    }

    std::string_view Buffer::view_str() noexcept {
        if (m_underlying == nullptr) {
            return std::string_view();
        }

        size_t len = evbuffer_get_length(m_underlying);
        unsigned char *data = evbuffer_pullup(m_underlying, len);

        return std::string_view(reinterpret_cast<char *>(data), len);
    }

    std::string Buffer::str() noexcept {
        if (m_underlying == nullptr) {
            return std::string();
        }

        size_t len = evbuffer_get_length(m_underlying);
        unsigned char *data = evbuffer_pullup(m_underlying, len);

        return std::string(reinterpret_cast<char *>(data), len);
    }

    ByteSlice Buffer::bytes() const noexcept {
        if (m_underlying == nullptr) {
            return ByteSlice{nullptr, 0};
        }
        size_t len = evbuffer_get_length(m_underlying);
        return ByteSlice {
            /* because std::byte is defined by the standard as an unsigned char,
             * this should be safe */
            .bytes = reinterpret_cast<std::byte *>(evbuffer_pullup(m_underlying, len)),
            .len = len,
        };
    }
}