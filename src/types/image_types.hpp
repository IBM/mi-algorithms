/**
 * \file image_types.hpp
 * \brief File containing image basic types.
 * \author tkornuta/krocki
 * \date Nov/09/2015
 */

#ifndef __IMAGE_TYPES_H__
#define __IMAGE_TYPES_H__

#include <memory>

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

/**
 * \brief Structure representing image and storing all related data.
 * \author krocki
 */
typedef struct {
	/// Label of the image.
	unsigned int label_id;
	/// Image width.
	unsigned int width;
	/// Image height,
	unsigned int height;
	/// Image type.
	image_type type;
	/// Size of image data structure.
	size_t data_size;
	/// Pointer to a memory block storing image data (values of pixels).
	void* image_data;
} image;


/*!
 * \brief Shared pointer to image type.
 * \author tkornuta
 */
typedef std::shared_ptr<image> image_ptr_t;

/*!
 * \brief Shared pointer to const image type.
 * \author tkornuta
 */
typedef std::shared_ptr<const image> const_image_ptr_t;

/**
 * \brief Structure storing X-Y coordinate.
 * \author krocki
 */
typedef struct {
	double x;
	double y;

} coord;


/**
 * \brief Structure storing image ROI (Region of Interest) boundary.
 * \author krocki
 */
typedef struct {
	/// Upper left corner of ROI.
	coord ul;
	/// Lower right corner of ROI.
	coord lr;
} pixel_bounds;


#ifdef __cplusplus 
extern "C" {
#endif


/**
 * \brief Sets integer value (!) of a given channel of a given image pixel.
 * @param dst Destination image.
 * @param channel Channel of the image.
 * @param row Pixel row.
 * @param col Pixel column.
 * @param value New value to be set (an integer).
 */
void set_color(image* dst, color_channel channel, unsigned row, unsigned col, uint8_t value);


/**
 * \brief Sets float value (!) of a given channel of a given image pixel.
 * @param dst Destination image.
 * @param channel Channel of the image.
 * @param row Pixel row.
 * @param col Pixel column.
 * @param value New value to be set (a float).
 */
void set_color_float(image* dst, color_channel channel, unsigned row, unsigned col, float value);


/**
 * \brief Returns (integer) value of a given channel of a given image pixel.
 * @param src Source image.
 * @param channel Channel of the image.
 * @param row Pixel row.
 * @param col Pixel column.
 * @return Value of a given image channel.
 */
uint8_t get_color(image* src, color_channel channel, unsigned row, unsigned col);


/**
 * \brief Returns (float) value of a given channel of a given image pixel.
 * @param src Source image.
 * @param channel Channel of the image.
 * @param row Pixel row.
 * @param col Pixel column.
 * @return Value of a given image channel.
 */
float get_color_float(image* src, color_channel channel, unsigned row, unsigned col);


/**
 * \brief Computes address of a given pixel in (float) image.
 * @param src Source image.
 * @param position Desired position.
 * @return Address in image data table.
 */
float get_color_float_linear_address(image* src, unsigned position);


/**
 * \brief Computes position of a given pixel in image.
 * @param img Source image.
 * @param channel Image channel.
 * @param row Desired row.
 * @param col Desired column.
 * @return Position.
 */
unsigned long get_image_linear_position(image* img, color_channel channel, unsigned row, unsigned col);

#ifdef __cplusplus
}
#endif


}//: namespace types

}//: namespace mic

#endif
