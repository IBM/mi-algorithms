#ifndef __IMAGE_TOOLS_H__
#define __IMAGE_TOOLS_H__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Dependencies on core_types
#include <types/image_types.hpp>
#include <types/vector_types.hpp>

namespace mic {

/**
 * \namespace mic::data_utils
 * \brief Namespace containing functions useful for image processing, memory allocation/deallocation etc.
 * \author tkornuta
 */
namespace data_utils {


using namespace mic::types;


typedef enum { NONE, AREA, BILINEAR} interpolation_type;

typedef enum { THRESHOLDING, ADAPTIVE_THRESHOLDING, PROBABILITY} binarization_type;


void alloc_image(image* img, image_type type, unsigned width, unsigned height);
void free_image(image* img);
void copy_image(const image* src, image* dst);

void convert_image_type(image* src, image* dst);
void resize_image(image* src, image* dst, interpolation_type i_type);
void resize_image_channel(image* src, image* dst, color_channel channel, interpolation_type);
void resize_image_channel_float(image* src, image* dst, color_channel channel, interpolation_type i_type);

v_4i subimage(image* src, image* dst, unsigned x, unsigned y);
void subimage_channel(image* src, image* dst, color_channel channel, unsigned x, unsigned y);
void copy_subimage(image* src, image* dst, unsigned x, unsigned y);
void copy_subimage_channel(image* src, image* dst, color_channel in, color_channel out, unsigned x, unsigned y);
v_4i random_subimage(image* src, image* dst);
v_4i random_subimage_center_with_radius(image* src, image* dst, unsigned x, unsigned y, unsigned radius);

double square_overlap_value(coord a_ul, coord a_lr, coord b_ul, coord b_lr);
pixel_bounds pixel_coords(unsigned i, unsigned j, double pixel_h, double pixel_w);

uint8_t convert_pixel_to_grayscale_tmp(const color_rgba pix);


void print_image_data(image* img);
void clear_image(image* img);

color_rgba pixel_linear_interpolation(float value, color_rgba l, color_rgba r);
color_rgba pixel_bilinear_interpolation(float value_x, float value_y, color_rgba ul, color_rgba ur, color_rgba ll, color_rgba lr);

void binarize_channel(image* src, image* dst, color_channel channel, binarization_type);
void set_alpha(image* img, uint8_t alpha_value);
void multiply_image_with_color(image* img, float red, float green, float blue, float alpha);
void add_noise(image *img, color_channel c, float prob);
void remove_signal(image *img, color_channel c, float prob);
void remove_signal_float(image *img, color_channel c, float prob);
void add_nonneg_noise(image *img, color_channel c, float prob);
float image_mean(image *img, color_channel c);
void invert_image(image* img, color_channel c);


}//: namespace data_utils
}//: namespace mic


#endif
