/*
MIT License

Copyright (c) 2023 Jean-Marc Pelletier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <cstdint>
#include <limits>
#include <random>

namespace JMP
{
	namespace Audio {

#ifdef JMP_64_BIT_AUDIO
		using sample_t = double;
#else
		using sample_t = float;
#endif

		const sample_t SPEED_OF_SOUND = 343;

		// Convert float samples to integer PCM (without dithering).
		template <typename T>
		T convert(float sample) = delete;

		template <>
		int16_t convert(float sample)
		{
			sample *= std::numeric_limits<int16_t>::max();

			if (sample > std::numeric_limits<int16_t>::max()) {
				sample = std::numeric_limits<int16_t>::max();
			} else if (sample < std::numeric_limits<int16_t>::min()) {
				sample = std::numeric_limits<int16_t>::min();
			}

			return (int16_t)sample;
		}

		template <>
		uint8_t convert(float sample)
		{
			sample = sample * 0.5f + 0.5f;
			sample *= std::numeric_limits<uint8_t>::max();
				
			if (sample > std::numeric_limits<uint8_t>::max()) {
				sample = std::numeric_limits<uint8_t>::max();
			}
			else if (sample < 0) {
				sample = 0;
			}

			return (uint8_t)sample;
		}
	}
}