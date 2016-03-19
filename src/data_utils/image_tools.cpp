#include <logger/Log.hpp>

// Dependencies on data utils.
#include <data_utils/image_tools.hpp>
#include <data_utils/interpolation.hpp>

#include <algorithm>
#include "rand.hpp"

namespace mic {
namespace data_utils {


void alloc_image(image* img, image_type type, unsigned width, unsigned height) {

	unsigned pixel_size;

	img->width = width;
	img->height = height;
	img->type = type;

	CHECK(img->type == BINARY || img->type == GRAYSCALE || img->type == RGBA);

	if (img->type == RGBA) {

		pixel_size = sizeof(color_rgba);

	} else if (img->type == GRAYSCALE) {

		pixel_size = sizeof(float);
	}

	else {

		pixel_size = sizeof(uint8_t);

	} 

	img->image_data = (void*) malloc(pixel_size * img->width * img->height);

	img->data_size = pixel_size * img->width * img->height;

	CHECK(img->image_data != NULL);

	if (!img->image_data) {

		fprintf(stderr, "alloc_image: out of memory! (Requested %u B)\n", pixel_size * img->width * img->height);
		exit(0);

	}

}

void free_image(image* img) {

	CHECK(img != NULL);

	img->width = 0;
	img->height = 0;

	free(img->image_data);

}

void copy_image(const image* src, image* dst) {

	if (src && dst) {

		if (src->image_data && dst->image_data) {

			if (dst->data_size >= src->data_size) {

				memcpy(dst->image_data, src->image_data, src->data_size);

				dst->label_id = src->label_id;
				dst->width = src->width;
				dst->height = src->height;
				dst->data_size = src->data_size;
				dst->type = src->type;

			} else {

				CHECK(dst->data_size >= src->data_size);

				LOG(LERROR) <<"dst->data_siz e=" << dst->data_size << " src->data_size = " << src->data_size;
			}
			

		} else {

			CHECK(src->image_data != NULL);
			CHECK(dst->image_data != NULL);		
		}

	} else {
		
		CHECK(src != NULL);
		CHECK(dst != NULL);

	}

}
void print_image_data(image* img) {

	if (img) {

		printf("-----Image-----\n");
		printf("Type: %d\n", img->type);
#ifdef _WIN32    
		printf("Data length: %Iu\n", img->data_size); // %zu not recognized
#else    
		printf("Data length: %zu\n", img->data_size);
#endif
		
		for (unsigned i = 0; i < img->data_size; i++) {

			printf("%x", ((uint8_t*) img->image_data)[i]);

		}

		printf("\n------End------\n");
	}
}

void binarize_channel(image* src, image* dst, color_channel channel, binarization_type b_type) {

	if (src && src->image_data && dst && dst->image_data) {

		dst->label_id = src->label_id;

		if (src->width == dst->width && src->height == dst->height) {

			if ((src->type == GRAYSCALE && channel == GRAY) || 
				(src->type == RGBA && (channel == RED || channel == BLUE || channel == GREEN)) ||
				src->type == BINARY) {

				if (dst->type == BINARY) {

					for (unsigned i = 0; i < src->height; i++) {

						for (unsigned j = 0; j < src->width; j++) {

							uint8_t value;

							if (src->type == GRAYSCALE) {

								value = (uint8_t)(float)round(get_color_float(src, channel, i, j) * 255.0f);
							
							} else {

								value = get_color(src, channel, i, j);

							}

							uint8_t out = value;

							if (b_type == THRESHOLDING) {

								if (out < 100) 
									out = 0;
								else 
									out = 255;

							} else if (b_type == PROBABILITY) {

								double r = (rand_real01() * 255.0);

								if (r < value)
									out = 255;
								else
									out = 0;

							}

							
							set_color(dst, channel, i, j, out);
						}

					}

				} else {

					CHECK(dst->type == BINARY);

				}

			} else {

				CHECK(((src->type == GRAYSCALE || src->type == BINARY) && channel == GRAY) || 
					(src->type == RGBA && (channel == RED || channel == BLUE || channel == GREEN)));
			}

		} else {

			CHECK(src->width == dst->width);
			CHECK(src->height == dst->height);

		}

	} else {

		CHECK(src != NULL);
		CHECK(dst != NULL);
		CHECK(src->image_data != NULL);
		CHECK(dst->image_data != NULL);

	}
}


uint8_t convert_pixel_to_grayscale(const color_rgba pix_) {

	// luminosity method
	return (uint8_t)((float)round(((0.21f * (float)pix_.r + 0.71f * (float)pix_.g + 0.07f * (float)pix_.b))));

}

void convert_image_type(image* src, image* dst) {

	if (src && src->image_data && dst && dst->image_data) {

		dst->label_id = src->label_id;

		if (src->width == dst->width && src->height == dst->height) {

			if (src->type == GRAYSCALE && dst->type == RGBA) {

				for (unsigned i = 0; i < src->height; i++) {

					for (unsigned j = 0; j < src->width; j++) {

						uint8_t value = get_color(src, GRAY, i, j);
						set_color(dst, RED, i, j, value);
						set_color(dst, GREEN, i, j, value);
						set_color(dst, BLUE, i, j, value);

					}
				}
			
			} else if (src->type == RGBA && dst->type == GRAYSCALE) {

				for (unsigned i = 0; i < src->height; i++) {

					for (unsigned j = 0; j < src->width; j++) {

						color_rgba color;
						color.r = get_color(src, RED, i, j);
						color.g = get_color(src, GREEN, i, j);
						color.b = get_color(src, BLUE, i, j);
						set_color_float(dst, GRAY, i, j, convert_pixel_to_grayscale(color)/255.0f);

					}
				}

			} else {

				CHECK((src->type == RGBA && dst->type == GRAYSCALE) || (src->type == GRAYSCALE && dst->type == RGBA));
			}

		} else {

			CHECK(src->width == dst->width);
			CHECK(src->height == dst->height);

		}

	} else {

		CHECK(src != NULL);
		CHECK(dst != NULL);
		CHECK(src->image_data != NULL);
		CHECK(dst->image_data != NULL);

	}

}



void resize_image(image* src, image* dst, interpolation_type i_type) {

	if (src && src->image_data && dst && dst->image_data) {

		dst->label_id = src->label_id;

		if (src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0) {

			if (src->type == dst->type) {

				if (src->type == BINARY) {

					resize_image_channel(src, dst, GRAY, i_type);

				} else if (src->type == RGBA) {

					resize_image_channel(src, dst, RED, i_type);
					resize_image_channel(src, dst, GREEN, i_type);
					resize_image_channel(src, dst, BLUE, i_type);
					resize_image_channel(src, dst, ALPHA, i_type);

				} else if (src->type == GRAYSCALE) {

					resize_image_channel_float(src, dst, GRAY, i_type);

				} else {

					CHECK(src->type == GRAYSCALE || src->type == BINARY || src->type == RGBA);
				}	

			} else {

				CHECK(src->type == dst->type);

			}

		}  else {

			CHECK(src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0);

		}

	} else {

		CHECK(src != NULL);
		CHECK(dst != NULL);
		CHECK(src->image_data != NULL);
		CHECK(dst->image_data != NULL);

	}
}

void resize_image_channel(image* src, image* dst, color_channel channel, interpolation_type i_type) {

	if (src && src->image_data && dst && dst->image_data) {

		dst->label_id = src->label_id;

		if (src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0) {

			float scale_y = (float)dst->width/(float)src->width;
			float scale_x = (float)dst->height/(float)src->height;

     		float src_i_f, src_j_f;
     		float src_i_r, src_j_r;
			unsigned int src_i, src_j;

			float value_a, value_b, value_c, value_d;

 			for (unsigned i = 0; i < dst->height; i++) {

     			for (unsigned  j = 0; j < dst->width; j++) {

     				uint8_t value;

     				src_i_f = ((float)i / scale_x);
     				src_j_f = ((float)j / scale_y);

     				src_i = (unsigned int)(float)(src_i_f);
     				src_j = (unsigned int)(float)(src_j_f);

     				src_i_r = (float)fabs(src_i_f - (float)src_i);
     				src_j_r = (float)fabs(src_j_f - (float)src_j);

					value_a = get_color(src, channel, src_i, src_j);
					value_b = value_a;
					value_c = value_a;
					value_d = value_a;		

					if (i_type == AREA) {

						float src_pixel_width = 1.0f/(float)src->width;
						float src_pixel_height = 1.0f/(float)src->height;
						float dst_pixel_width = 1.0f/(float)dst->width;
						float dst_pixel_height = 1.0f/(float)dst->height;

						pixel_bounds dst_bounds = pixel_coords(i, j, dst_pixel_width, dst_pixel_height);

						unsigned src_ul_x = (unsigned)(dst_bounds.ul.x / src_pixel_width);
						unsigned src_lr_x = (unsigned)(dst_bounds.lr.x / src_pixel_width + 1);
						unsigned src_ul_y = (unsigned)(dst_bounds.ul.y / src_pixel_height);
						unsigned src_lr_y = (unsigned)(dst_bounds.lr.y / src_pixel_height + 1);
						
						value = 0;

						for (unsigned x = src_ul_x; x < src_lr_x; x++) {

							for (unsigned y = src_ul_y; y < src_lr_y; y++) {

								pixel_bounds src_bounds = pixel_coords(x, y, src_pixel_width, src_pixel_height);
					
								uint8_t src_pix_value = get_color(src, channel, x, y);
								value += (uint8_t)(int)((float)src_pix_value * square_overlap_value(dst_bounds.ul, dst_bounds.lr, src_bounds.ul, src_bounds.lr)/(dst_pixel_height * dst_pixel_width));

							}
						}

					}

					else if (i_type == BILINEAR) {

						// value_a = get_color(src, channel, (unsigned)(float)floor(src_i_f), (unsigned)(float)floor(src_j_f));
						// value_b = get_color(src, channel, (unsigned)(float)ceil(src_i_f), (unsigned)(float)floor(src_j_f));
						// value_c = get_color(src, channel, (unsigned)(float)floor(src_i_f), (unsigned)(float)ceil(src_j_f));
						// value_d = get_color(src, channel, (unsigned)(float)ceil(src_i_f), (unsigned)(float)ceil(src_j_f));
						
						value = (uint8_t)(int)(float)round(bilinear_interpolation((float)src_i_r, (float)src_j_r, (float)value_a, (float)value_b, (float)value_c, (float)value_d));

					}

					else
						value = (uint8_t)(int)(float)value_a;

					set_color(dst, channel, i, j, value);

     			}

			} 
			
		} else {

			CHECK(src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0);

		}

	} else {

		CHECK(src != NULL);
		CHECK(dst != NULL);
		CHECK(src->image_data != NULL);
		CHECK(dst->image_data != NULL);

	}

}

// temporary, TODO: merge uint and float functions
void resize_image_channel_float(image* src, image* dst, color_channel channel, interpolation_type i_type) {

	if (src && src->image_data && dst && dst->image_data) {

		dst->label_id = src->label_id;

		if (src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0) {

			float scale_y = (float)dst->width/(float)src->width;
			float scale_x = (float)dst->height/(float)src->height;

     		float src_i_f, src_j_f;
     		float src_i_r, src_j_r;
			unsigned int src_i, src_j;

			float value_a, value_b, value_c, value_d;

 			for (unsigned i = 0; i < dst->height; i++) {

     			for (unsigned  j = 0; j < dst->width; j++) {

     				float value;

     				src_i_f = ((float)i / scale_x);
     				src_j_f = ((float)j / scale_y);

     				src_i = (unsigned int)(float)(src_i_f);
     				src_j = (unsigned int)(float)(src_j_f);

     				src_i_r = (float)fabs(src_i_f - (float)src_i);
     				src_j_r = (float)fabs(src_j_f - (float)src_j);

					value_a = get_color_float(src, channel, src_i, src_j);
					value_b = value_a;
					value_c = value_a;
					value_d = value_a;		

					if (i_type == AREA) {

						float src_pixel_width = 1.0f/(float)src->width;
						float src_pixel_height = 1.0f/(float)src->height;
						float dst_pixel_width = 1.0f/(float)dst->width;
						float dst_pixel_height = 1.0f/(float)dst->height;

						pixel_bounds dst_bounds = pixel_coords(i, j, dst_pixel_width, dst_pixel_height);

						unsigned src_ul_x = (unsigned)(dst_bounds.ul.x / src_pixel_width);
						unsigned src_lr_x = (unsigned)(dst_bounds.lr.x / src_pixel_width + 1);
						unsigned src_ul_y = (unsigned)(dst_bounds.ul.y / src_pixel_height);
						unsigned src_lr_y = (unsigned)(dst_bounds.lr.y / src_pixel_height + 1);
						
						value = 0;

						for (unsigned x = src_ul_x; x < src_lr_x; x++) {

							for (unsigned y = src_ul_y; y < src_lr_y; y++) {

								pixel_bounds src_bounds = pixel_coords(x, y, src_pixel_width, src_pixel_height);
					
								float src_pix_value = get_color_float(src, channel, x, y);
								value += (float)(int)((float)src_pix_value * square_overlap_value(dst_bounds.ul, dst_bounds.lr, src_bounds.ul, src_bounds.lr)/(dst_pixel_height * dst_pixel_width));

							}
						}

					}

					else if (i_type == BILINEAR) {

						// value_a = get_color(src, channel, (unsigned)(float)floor(src_i_f), (unsigned)(float)floor(src_j_f));
						// value_b = get_color(src, channel, (unsigned)(float)ceil(src_i_f), (unsigned)(float)floor(src_j_f));
						// value_c = get_color(src, channel, (unsigned)(float)floor(src_i_f), (unsigned)(float)ceil(src_j_f));
						// value_d = get_color(src, channel, (unsigned)(float)ceil(src_i_f), (unsigned)(float)ceil(src_j_f));
						
						value = (float)(int)(float)round(bilinear_interpolation((float)src_i_r, (float)src_j_r, (float)value_a, (float)value_b, (float)value_c, (float)value_d));

					}

					else
						value = (float)(int)(float)value_a;

					set_color_float(dst, channel, i, j, value);

     			}

			} 
			
		} else {

			CHECK(src->width > 0 && src->height > 0 && dst->width > 0 && dst->height > 0);

		}

	} else {

		CHECK(src != NULL);
		CHECK(dst != NULL);
		CHECK(src->image_data != NULL);
		CHECK(dst->image_data != NULL);

	}

}

pixel_bounds pixel_coords(unsigned i, unsigned j, double pixel_h, double pixel_w) {

	pixel_bounds out;

	out.ul.x = (double)(i) * (double)pixel_w;
	out.lr.x = (double)(i + 1) * (double)pixel_w;

	out.ul.y = (double)(j) * (double)pixel_h;
	out.lr.y = (double)(j + 1) * (double)pixel_h;

	return out;
}

color_rgba pixel_linear_interpolation(float value, color_rgba l, color_rgba r) {

	color_rgba out;

	out.r = (uint8_t)(int)(float)round(linear_interpolation(value, l.r, r.r));
	out.g = (uint8_t)(int)(float)round(linear_interpolation(value, l.g, r.g));
	out.b = (uint8_t)(int)(float)round(linear_interpolation(value, l.b, r.b));
	out.a = (uint8_t)(int)(float)round(linear_interpolation(value, l.a, r.a));

	return out;
}

color_rgba pixel_bilinear_interpolation(float value_x, float value_y, color_rgba ul, color_rgba ur, color_rgba ll, color_rgba lr) {

	color_rgba out;

	out.r = (uint8_t)(int)(float)round(bilinear_interpolation(value_x, value_y, ul.r, ur.r, ll.r, lr.r));
	out.g = (uint8_t)(int)(float)round(bilinear_interpolation(value_x, value_y, ul.g, ur.g, ll.g, lr.g));
	out.b = (uint8_t)(int)(float)round(bilinear_interpolation(value_x, value_y, ul.b, ur.b, ll.b, lr.b));
	out.a = (uint8_t)(int)(float)round(bilinear_interpolation(value_x, value_y, ul.a, ur.a, ll.a, lr.a));

	return out;

}

double square_overlap_value(coord a_ul, coord a_lr, coord b_ul, coord b_lr) {

	coord UL;
	coord LR;

	//find tighter upper left corner
	UL.x = std::max(a_ul.x, b_ul.x);
	UL.y = std::max(a_ul.y, b_ul.y);

	//find tighter lower right corner
	LR.x = std::min(a_lr.x, b_lr.x);
	LR.y = std::min(a_lr.y, b_lr.y);

	//calculate UL, LR area
	double x_length = std::max(LR.x - UL.x, 0.0);
	double y_length = std::max(LR.y - UL.y, 0.0);

	return x_length * y_length;

}

v_4i subimage(image* src, image* dst, unsigned x, unsigned y) {

	v_4i out;

	if (src && dst) {

		out.x = (uint8_t) y;
 		out.y = (uint8_t) x;
 		out.z = (uint8_t) (y + dst->width); 
 		out.w = (uint8_t) (x + dst->height);

		if (src->type == dst->type && (src->type == GRAYSCALE || src->type == BINARY))
			subimage_channel(src, dst, GRAY, x, y);

	} else {

		CHECK(src && dst);

	}

	return out;

}

void subimage_channel(image* src, image* dst, color_channel c_type, unsigned x, unsigned y) {

	if (src && dst) {

		dst->label_id = src->label_id;

		unsigned win_w = dst->width;
		unsigned win_h = dst->height;

		if ((x + win_w) <= src->width && (y + win_h) <= src->height) {

			for (unsigned row = 0; row < win_h; row++) {

				for (unsigned col = 0; col < win_w; col++) {

					if (src->type == BINARY) {
						set_color(dst, c_type, row, col, get_color(src, c_type, row + x, col + y));
					}
					
					else if (src->type == GRAYSCALE) {
						set_color_float(dst, c_type, row, col, get_color_float(src, c_type, row + x, col + y));
					}
				}
			}

		} else {

			CHECK(x < dst->width && y < dst->height);
			CHECK((x + win_w) <= src->width && (y + win_h) <= src->height);

		}

	} else {

		CHECK(src && dst);

	}

}

v_4i random_subimage(image* src, image* dst) {

	v_4i out;

	if (src && dst) {

	 	unsigned x_w = src->width - dst->width;
	 	unsigned y_w = src->height - dst->height;

	 	if (x_w > 0 && y_w > 0) {

	 		unsigned x = rand_int() % (x_w);
	 		unsigned y = rand_int() % (y_w);

	 		out = subimage(src, dst, x, y);

	 	} else {

	 		CHECK(x_w > 0 && y_w > 0);

	 	}

	} else {

		CHECK(src && dst);

	}

	return out;
}

v_4i random_subimage_center_with_radius(image* src, image* dst, unsigned x, unsigned y, unsigned radius) {

	v_4i out;

	if (src && dst) {

		unsigned xx = rand_int() % (2 * radius) - radius + x;
		unsigned yy = rand_int() % (2 * radius) - radius + y;

		out = subimage(src, dst, xx, yy);

		} else {

			CHECK(src && dst);

		}

	return out;
}

void copy_subimage_channel(image* src, image* dst, color_channel in, color_channel out, unsigned x, unsigned y) {

	if (src && dst) {

		if (src->height + y <= dst->height && src->width + x <= dst->width) {

			for (unsigned i = 0; i < src->height; i++) {

				for (unsigned j = 0; j < src->width; j++) {

					set_color(dst, out, i + x, j + y, std::max(get_color(dst, out, i + x, j + y), get_color(src, in, i, j)));

				}
			}

		} else {

			CHECK(src->height + y <= dst->height && src->width + x <= dst->width);
		}

	} else {

		CHECK(src && dst);

	}

}

void copy_subimage(image* src, image* dst, unsigned x, unsigned y) {

	if (src && dst) {

		if ((src->type == BINARY || src->type == GRAYSCALE) && (dst->type == src->type)) {

			copy_subimage_channel(src, dst, GRAY, GRAY, x, y);
			set_alpha(dst, 255);

		} else if (src->type == BINARY || src->type == GRAYSCALE) {

			copy_subimage_channel(src, dst, GRAY, RED, x, y);
			copy_subimage_channel(src, dst, GRAY, GREEN, x, y);
			copy_subimage_channel(src, dst, GRAY, BLUE, x, y);

		} else {

			copy_subimage_channel(src, dst, RED, RED, x, y);
			copy_subimage_channel(src, dst, GREEN, GREEN, x, y);
			copy_subimage_channel(src, dst, BLUE, BLUE, x, y);
			copy_subimage_channel(src, dst, ALPHA, ALPHA, x, y);

		}


	} else {

		CHECK(src && dst);

	}

}

void clear_image(image* img) {

	if (img && img->image_data) {

		memset(img->image_data, '\0', img->data_size);

	} else {

		CHECK(img && img->image_data);

	}
}

void set_alpha(image* img, uint8_t alpha_value) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				set_color(img, ALPHA, i, j, alpha_value);
			}
		}

	} else {

		CHECK(img && img->image_data);

	}
}

void multiply_image_with_color(image* img, float red, float green, float blue, float alpha) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				set_color(img, RED, i, j, (uint8_t)(float)round(get_color(img, RED, i, j) * red));
				set_color(img, GREEN, i, j, (uint8_t)(float)round(get_color(img, GREEN, i, j) * green));
				set_color(img, BLUE, i, j, (uint8_t)(float)round(get_color(img, BLUE, i, j) * blue));
				set_color(img, ALPHA, i, j, (uint8_t)(float)round(get_color(img, ALPHA, i, j) * alpha));

			}
		}

	} else {

		CHECK(img && img->image_data);

	}

}

void add_noise(image *img, color_channel c, float prob) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				float r = (float)rand_real01();

				if (r < prob) 
					set_color(img, c, i, j, (uint8_t)(255 - get_color(img, c, i, j)));

			}
		}
	}
}

void remove_signal(image *img, color_channel c, float prob) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				float r = (float)rand_real01();

				if (r < prob) 
					set_color(img, c, i, j, (uint8_t)(0));

			}
		}
	}
}

void remove_signal_float(image *img, color_channel c, float prob) {

	if (img && img->image_data) {

		//unsigned noise = 0;

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				float r = (float)rand_real01();

				

				if (r > prob) {
					set_color_float(img, c, i, j, 0.5f);
					//noise++;
					//printf("%f, %d, %u\n", prob, r > prob, noise);
				} else {
					//printf("%f %f\n", r, prob);
				}


			}
			

		}
	}
}

void add_nonneg_noise(image *img, color_channel c, float prob) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				float r = (float)rand_real01();

				if (r <= prob && get_color(img, c, i, j) == 0)
					set_color(img, c, i, j, (uint8_t)(255));

			}
		}
	}

}

float image_mean(image *img, color_channel c) {

	float mean = 0.0f;

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				mean += get_color(img, c, i, j);

			}
		}	
	}

	return mean/(float)(img->height * img->width);
}

void invert_image(image* img, color_channel c) {

	if (img && img->image_data) {

		for (unsigned i = 0; i < img->height; i++) {

			for (unsigned j = 0; j < img->width; j++) {

				if (img->type == BINARY || img->type == RGBA)
					set_color(img, c, i, j, (uint8_t)(255 - get_color(img, c, i, j)));
				
				else if (img->type == GRAYSCALE) {

					set_color_float(img, c, i, j, (1.0f - get_color_float(img, c, i, j)));
				}

			}
		}	
	}

}

}//: namespace data_utils
}//: namespace mic


