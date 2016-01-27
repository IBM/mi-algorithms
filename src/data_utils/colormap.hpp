#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include <types/image_types.hpp>

using namespace mic::types;

namespace mic {
namespace data_utils {

/*!
 * \brief Colormap type.
 * \author krocki
 */
typedef enum {COLORMAP_JET, COLORMAP_HAXBY, COLORMAP_GRAY, COLORMAP_SEISMIC} COLORMAP_TYPE;

const static uint8_t haxby[20][3] = {	{  9,     0,   121},  
					 			 		{ 40,     0,   150},
						  				{  0,     9,   200},
    									{  0,    25,   212},
    									{ 26,   102,   240}, 
    									{ 25,   175,   255},
    									{ 50,   190,   255},
    									{ 97,   225,   240}, 
      									{106,   236,   225},
         								{138,   236,   174},	
					 			 		{205,   255,   162},
										{223,   246,   141},
						  				{248,   215,   104},
    									{255,   189,    87},
    									{244,   117,    75}, 
    									{255,    90,    90},
    									{255,   124,   124},
    									{246,   179,   174}, 
      									{255,   196,   196},
         								{255,   236,   236}		};

const static uint8_t seismic[20][3] = {	{  0, 	 36,   227},
										{  0,    72,   250},
										{  0,   135,   205},
										{  0,   208,   139},
										{ 22,   244,    90},
										{ 63,   250,    54},
										{115,   255,    26},
										{189,   255,    12},
										{255,   255,     0},
										{255,   255,     0},
										{255,   255,     0},
										{255,   221,     0},
										{255,   183,     0},
										{255,   144,     0},
										{255,   106,     0},
										{255,    68,     0},
										{255,    29,     0},
										{247,     0,     0},
										{208,     0,     0},
										{170,     0,     0}		};

const static uint8_t haxby_colors = 20;
const static uint8_t seismic_colors = 20;

float interpolate_jet( float val, float y0, float x0, float y1, float x1);
float interpolate( float val, float y_low, float y_high);
float jet_base( float val ) ;
float jet_red( float gray );
float jet_green( float gray );
float jet_blue( float gray );

color_rgba map_value_to_color(float val, float min, float max, COLORMAP_TYPE t);

color_rgba jet_colormap(float value);
color_rgba haxby_colormap(float value);
color_rgba seismic_colormap(float value);
color_rgba gray_colormap(float value);


}//: namespace data_utils
}//: namespace mic

#endif

