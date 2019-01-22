/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file image_types.cpp
 * \brief File containing definitions of functions constituting basic operations on images.
 * \author tkornuta/krocki
 * \date Nov/09/2015
 */

// Dependency to system_utils.
#include <logger/Log.hpp>

#include <types/image_types.hpp>

namespace mic {
namespace types {

//TODO: combine set_color & set_color_float? (MACRO?)

void set_color(image* dst, color_channel channel, unsigned row, unsigned col, uint8_t value) {

	CHECK(dst != NULL);
	CHECK(dst->image_data != NULL);

	// CHECK IF IMAGE DATA TYPE IS uint8_t

	if (dst->image_data) {

		unsigned long position = get_image_linear_position(dst, channel, row, col);

		((uint8_t*)dst->image_data)[position] = value;

	}

}

void set_color_float(image* dst, color_channel channel, unsigned row, unsigned col, float value) {

	CHECK(dst != NULL);
	CHECK(dst->image_data != NULL);

	// CHECK IF IMAGE DATA TYPE IS float

	if (dst->image_data) {

		unsigned long position = get_image_linear_position(dst, channel, row, col);

		((float*)dst->image_data)[position] = value;

	}

}

uint8_t get_color(image* src, color_channel channel, unsigned row, unsigned col) {

	CHECK(src != NULL);
	CHECK(src->image_data != NULL);

	uint8_t out = 0;

	// CHECK IF IMAGE DATA TYPE IS uint8_t

	if (src->image_data) {

		unsigned long position = get_image_linear_position(src, channel, row, col);

		out = ((uint8_t*)src->image_data)[position];
	}

	return out;
}

float get_color_float(image* src, color_channel channel, unsigned row, unsigned col) {

	CHECK(src != NULL);
	CHECK(src->image_data != NULL);

	// CHECK IF IMAGE DATA TYPE IS float

	float out = 0;

	if (src->image_data) {

		unsigned long position = get_image_linear_position(src, channel, row, col);

		out = ((float*)src->image_data)[position];
	}

	return out;
}

float get_color_float_linear_address(image* src, unsigned position) {

	CHECK(src != NULL);
	CHECK(src->image_data != NULL);

	// CHECK IF IMAGE DATA TYPE IS float

	float out = 0;

	if (src->image_data) {

		out = ((float*)src->image_data)[position];
	}

	return out;

}
unsigned long get_image_linear_position(image* img, color_channel channel, unsigned row, unsigned col) {

	unsigned long position = 0;

	CHECK(img != NULL);

	if (row < img->height && col < img->width) {

		if (img->type == RGBA) {

			position = (row * img->width + col) * 4;

			if (channel == RED) {

				// offset = 0

			}

			else if (channel == GREEN) {

				position += 1;

			}

			else if (channel == BLUE) {

				position += 2;

			}

			else if (channel == ALPHA) {

				position += 3;

			} else {

				// something's wrong
				CHECK(channel == ALPHA || channel == RED || channel == GREEN || channel == BLUE);

			}

		} else if (img->type == GRAYSCALE || img->type == BINARY) {

			if (channel == GRAY) {

				position = row * img->width + col;

			} else {

				CHECK(channel == GRAY);
				// something's wrong
			}
		}

	} else {

		//CHECK(row < img->height && col < img->width);
	}

	return position;

}


}//: namespace types
}//: namespace mic


