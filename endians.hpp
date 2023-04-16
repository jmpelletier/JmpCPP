/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <type_traits>
#include <cstdint>


namespace JMP
{
	// Runtime check of machine endianness.
	bool is_big_endian() noexcept {
		const union {
			uint32_t i;
			uint8_t c[4];
		} u = { 0x11223344 };

		return u.c[0] == 0x11;
	}

	// Runtime check of machine endianness.
	bool is_little_endian() noexcept {
		const union {
			uint32_t i;
			uint8_t c[4];
		} u = { 0x11223344 };

		return u.c[0] == 0x44;
	}

	// Swapping the bytes of an 8-bit integer is a no-op.
	template <typename T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 1, bool> = true>
		constexpr T byteswap(T i) {
		return i;
	}

	// Swaps the bytes of a 16-bit integer.
	template <typename T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 2, bool> = true>	
	constexpr T byteswap(T i) {
		return
			((i & 0xFF00) >> 8) |
			((i & 0x00FF) << 8);
	}

	// Swaps the bytes of a 32-bit integer.
	template <typename T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 4, bool> = true>
	constexpr T byteswap(T i) {
		return
			((i & 0xFF000000) >> 24) |
			((i & 0x00FF0000) >> 8) |
			((i & 0x0000FF00) << 8) |
			((i & 0x000000FF) << 24);
	}

	// Swaps the bytes of a 64-bit integer.
	template <typename T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 8, bool> = true>
	constexpr T byteswap(T i) {
		return
			((i & 0xFF00000000000000) >> 56) |
			((i & 0x00FF000000000000) >> 40) |
			((i & 0x0000FF0000000000) >> 24) |
			((i & 0x000000FF00000000) >> 8) |
			((i & 0x00000000FF000000) << 8) |
			((i & 0x0000000000FF0000) << 24) |
			((i & 0x000000000000FF00) << 40) |
			((i & 0x00000000000000FF) << 56);
	}

	// Stores an integer value in big-endian format.
	template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
	class BigEndian
	{
	private:
		T _val;
	public:
		BigEndian() : _val(0) {}
		BigEndian(T const& i) : _val(is_little_endian() ? byteswap<T>(i) : i) {}

		operator T() const {
			return is_little_endian() ? byteswap<T>(_val) : _val;
		}

		// Returns a big-endian ordered integer.
		T bytes() const {
			return _val;
		}
		
		// Returns a pointer to the raw bytes of the big-endian integer.
		char const* byte_ptr() const {
			return static_cast<char const*>(&_val);
		}
	};

	// Stores an integer value in little-endian format.
	template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
	class LittleEndian
	{
	private:
		T _val;
	public:
		LittleEndian() : _val(0) {}
		LittleEndian(T const& i) : _val(is_big_endian() ? byteswap<T>(i) : i) {}

		operator T() const {
			return is_big_endian() ? byteswap<T>(_val) : _val;
		}

		// Returns a little-endian ordered integer.
		T bytes() const {
			return _val;
		}

		// Returns a pointer to the raw bytes of the little-endian integer.
		char const* byte_ptr() const {
			return static_cast<char const*>(&_val);
		}
	};
}
