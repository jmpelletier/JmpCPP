/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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