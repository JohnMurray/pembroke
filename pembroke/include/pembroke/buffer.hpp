#pragma once

#include <cstddef>
#include <string_view>

#include "pembroke/internal/forward_decls.hpp"

namespace pembroke {

    class ByteSlice;

    /**
     * @brief
     * Buffer is a simple wrapper over libevent's `evbuffer` that is needed when reading
     * or writing to/from a network or file-system. 
     * 
     * You may find that not all methods or utilities are are possible with an `evbuffer`
     * are exposed as only the methods essential to this libraries functionalities are
     * included.
     * 
     * @note
     * While the underlying C-style API mostly represents contents as strings, or bytes via
     * `unsigned char`, you can also decide to work purely with `std::byte` methods. To read
     * `std::bytes` (and avoid copies), you'll have to work with ByteSlice, which is a poor
     * man's `std:span` in a pre-C++20 world.
     * 
     * @see ByteSlice
     */
    class Buffer {
    private:
        evbuffer *m_underlying;

    public:
        /** @brief Construct a new, empty buffer object */
        Buffer() noexcept;

        /** @brief Move-construct a buffer-object. The underlying `evbuffer` will be transferred
         *         into the newly constructed class. */
        Buffer(Buffer &&) noexcept;

        ~Buffer() noexcept;

        /** @brief Move-assign a buffer-object. The underlying `evbuffer` will be transferred
         *         into the newly constructed class. */
        Buffer &operator=(Buffer &&) noexcept;

        /* Remove implicit buffer copies. Copying a buffer is often _not_
         * what a user wants to do. Instead make this explicit by forcing
         * them to call `copy()` */
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;


        // ---
        // Write Functions
        // ---

        /**
         * @brief Write a string-view of data into the buffer. The data being pointed to will
         *        be copied into the buffer.
         * 
         * @note Useful for writing `char` literals or `std::string` and avoid unnecessary
         *       copyig of the data.
         * @param str_view View of data to write to buffer
         */
        void add(std::string_view str_view) noexcept;

        /**
         * @brief
         * Write a c-style string for given number of characters. The sub-section of data
         * will be copied into the buffer.
         *
         * @pre
         * @p n_chars should not be larger than the length of @p c_str as this method does not
         * check for null-terminating characters.
         * 
         * @param c_str    The C-style string to add to the buffer
         * @param n_chars  The number of characters to write, from the start of the string
         */
        void add(const char *c_str, size_t n_chars) noexcept;

        /**
         * @brief
         * Write an array of bytes, the length of @p n_bytes. The data will be copied into
         * the buffer.
         * 
         * @pre
         * @p n_bytes should not be larger than the length of @p bytes.
         * 
         * @see Buffer::add(const ByteSlice &)
         * 
         * @param bytes    Pointer to head of `std::byte` array
         * @param n_bytes  The number of `std::byte`s to write to he buffer, starting from
         *                 @p bytes.
         */
        void add(const std::byte *bytes, size_t n_bytes) noexcept;

        /**
         * @brief
         * Write an array of bytes. The data will be copied into the buffer.
         * 
         * Very similar to `Buffer::add(const std::byte *, size_t)`, but using the ByteSlice.
         * The two methods are identical in functionality, but with this method adding parity
         * to the read method: `Buffer::bytes()`.
         * 
         * @see Buffer::add(const std::byte *, size_t)
         * 
         * @param byte_slice  Slice of `std::byte`s to append to the buffer.
         */
        void add(const ByteSlice &byte_slice) noexcept;


        // ---
        // Read Functions
        // ---

        /**
         * @brief Return the length of the data currently stored in the buffer. Same as Buffer::size().
         */
        [[nodiscard]]
        size_t length() noexcept;

        /**
         * @brief Return the length of the data currently stored in the buffer. Same as Buffer::length().
         */
        [[nodiscard]]
        size_t size() noexcept;

        /**
         * @brief Return a string-view into the buffer.
         */
        [[nodiscard]]
        std::string_view view_str() noexcept;

        /**
         * @brief Return a `std::string` of the buffer contents. Note that this @b will perform
         *        a copy of the underlying buffer data. If no copies are desired, see Buffer::view_str()
         *        or Buffer::bytes() methods.
         */
        [[nodiscard]]
        std::string str() noexcept;

        /**
         * @brief Return a ByteSlice of the buffer. No copies of the underlying buffer-data are performed.
         */
        [[nodiscard]]
        ByteSlice bytes() const noexcept;
    };

    /**
     * @brief
     * ByteSlice is a non-owning view into an array of `std::byte` objects. It exists to
     * facilitate zero-copy reading of Buffer data.
     * 
     * This struct is similar in intention to C++20's `std::span`, but without any other
     * useful utility features.
     */
    struct ByteSlice {
        std::byte *bytes;  /**< Pointer for first `std::byte` in the array */
        size_t len;        /**< The number of elements in the array/slice */
    };

}  // namespace pembroke