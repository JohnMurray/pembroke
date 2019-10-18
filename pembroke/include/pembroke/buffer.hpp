#pragma once

#include <string_view>

#include "pembroke/internal/forward_decls.hpp"

namespace pembroke {

    class Buffer {
    private:
        evbuffer *m_underlying;

    public:
        Buffer() noexcept;
        Buffer(Buffer &&) noexcept;

        ~Buffer() noexcept;

        Buffer &operator=(Buffer &&) noexcept;

        /* Remove implicit buffer copies. Copying a buffer is often _not_
         * what a user wants to do. Instead make this explicit by forcing
         * them to call `copy()` */
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;


        // ---
        // Write Functions
        // ---

        void add(std::string_view) noexcept;

        void add(const char *c_str, size_t n_chars) noexcept;


        // ---
        // Read Functions
        // ---

        [[nodiscard]]
        size_t length() noexcept;

        [[nodiscard]]
        size_t size() noexcept;

        [[nodiscard]]
        std::string_view view_str() noexcept;

        [[nodiscard]]
        std::string str() noexcept;
    };

}  // namespace pembroke