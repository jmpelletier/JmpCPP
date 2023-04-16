/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>
#include <fstream>

#include "endians.hpp"

namespace JMP
{
	// This is a static class that encapsulates functionality for writing WAV files. 
	class WaveFile {
	public:
		enum class AudioFormat {
			PCM = 0x0001,
			FLOAT = 0x0003,
			ALAW = 0x0006,
			MULAW = 0x0007
		};
	private:
		using RiffID = BigEndian<uint32_t>;
		using RiffVal_16 = LittleEndian<uint16_t>;
		using RiffVal_32 = LittleEndian<uint32_t>;

		#pragma pack(push)
		#pragma pack (2)
		struct Header {
			RiffID riff_id = 'RIFF';
			RiffVal_32 riff_chunk_size = 0;
			RiffID wave_id = 'WAVE';
			RiffID fmt_id = 'fmt ';
			RiffVal_32 fmt_size = 16;
			RiffVal_16 audio_format = static_cast<uint16_t>(AudioFormat::PCM);
			RiffVal_16 num_channels = 1;
			RiffVal_32 sample_rate = 44100;
			RiffVal_32 byte_rate = 44100 * 2;
			RiffVal_16 block_align = 2;
			RiffVal_16 bits_per_sample = 16;
			RiffID data_id = 'data';
			RiffVal_32 data_chunk_size = 0;

			Header() = delete;

			Header(uint16_t channels, uint32_t sr, uint16_t bit_depth, AudioFormat format, uint32_t sample_count)
			{
				num_channels = channels;
				sample_rate = sr;
				bits_per_sample = bit_depth;
				audio_format = static_cast<uint16_t>(format);
				const uint16_t bytes_per_sample = bits_per_sample >> 3;
				block_align = num_channels * bytes_per_sample;
				byte_rate = sample_rate * block_align;
				data_chunk_size = sample_count * bytes_per_sample;
				riff_chunk_size = sizeof(Header) + data_chunk_size - 8;
			}
		};
		#pragma pack(pop)

		template <typename T>
		static inline bool _write_file(std::string const& filename, Header const& header, std::vector<T> const& samples)
		{
			std::ofstream out_stream(filename, std::ios::binary);
			if (out_stream) {
				out_stream.write(reinterpret_cast<char const*>(&header), static_cast<std::streamsize>(sizeof(header)));
				
				const std::streamsize sample_bytes = static_cast<std::streamsize>(sizeof(T)) * static_cast<std::streamsize>(samples.size());
				out_stream.write(reinterpret_cast<char const*>(&samples.front()), sample_bytes);
				out_stream.close();
				return true;
			}
			else {
				return false;
			}
		}

	public:
		WaveFile() = delete;

		// Output samples to a WAV file.
		static bool write(std::string const & filename, uint16_t channels, uint32_t sample_rate, std::vector<uint8_t> const& samples) noexcept 
		{
			const Header header(channels, sample_rate, 8, AudioFormat::PCM, samples.size());
			return _write_file(filename, header, samples);
		}

		// Output samples to a WAV file.
		static bool write(std::string const& filename, uint16_t channels, uint32_t sample_rate, std::vector<int16_t> const& samples) noexcept 
		{
			const Header header(channels, sample_rate, 16, AudioFormat::PCM, samples.size());
			return _write_file(filename, header, samples);
		}

		// Output samples to a WAV file.
		static bool write(std::string const& filename, uint16_t channels, uint32_t sample_rate, std::vector<float> const& samples) noexcept 
		{
			const Header header(channels, sample_rate, 32, AudioFormat::FLOAT, samples.size());
			return _write_file(filename, header, samples);
		}
	};
}