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
 * \file image_types.hpp
 * \brief File containing image basic types.
 * \author tkornuta/krocki
 * \date Nov/09/2015
 */

#ifndef __COLOR_H__
#define __COLOR_H__

namespace mic {
namespace types {

/**
 * \brief Possible image types.
 * \author krocki
 */
typedef enum {
	RGBA, /**< Four channel RGBA image  - each pixel color is described by four values in range <0-255>*///!< RGBA
	GRAYSCALE, /**< Grayscale image - a single channel image with values of pixels in range <0-255> */   //!< GRAYSCALE
	BINARY /**< Binary image - a single channel image with values of pixels in range <0-1> */            //!< BINARY
} image_type;

/**
 * \brief Possible types of image channels.
 * \author krocki
 */
typedef enum {
	RED, /**< Red channel */
	GREEN, /**< Green channel */
	BLUE, /**< Blue channel */
	ALPHA, /**< Alpha channel */
	GRAY /**< Gray channel */
} color_channel;

/**
 * \brief Class for storing single a pixel of four channel (RGBA) image.
 * \author krocki/tkornuta
 */
class color_rgba {
public:
	/// Value of R component.
	uint8_t r;
	/// Value of G component.
	uint8_t g;
	/// Value of B component.
	uint8_t b;
	/// Value of A component.
	uint8_t a;

	/*!
	 * Default constructor, zeros components.
	 */
	color_rgba ( )
	{
		r = g = b = a = 0;
	}

	/*!
	 * Constructor setting up values of components to the one passed as parameters.
	 */
	color_rgba (uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) :
		r(r_), g(g_), b(b_), a(a_)
	{}
};


}//: namespace types

}//: namespace mic

#endif
