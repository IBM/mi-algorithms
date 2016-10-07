// Dependency on data_utils.
#include <data_utils/interpolation.hpp>
#include <data_utils/colormap.hpp>

using namespace mic::data_utils;


#include <stdio.h>
#include <math.h>

namespace mic {
namespace data_utils {

float interpolate_jet( float val, float y0, float x0, float y1, float x1 ) {

    return (val - x0) * (y1 - y0) / (x1 - x0) + y0;
}

float jet_base (float val ) {

    if ( val <= -0.75f ) return 0;
    else if ( val <= -0.25f ) return interpolate_jet( val, 0.0f, -0.75f, 1.0f, -0.25f );
    else if ( val <= 0.25f ) return 1.0f;
    else if ( val <= 0.75f ) return interpolate_jet( val, 1.0f, 0.25f, 0.0f, 0.75f );
    else return 0.0f;

}

float jet_red( float value ) {

    return jet_base( value - 0.6f );
}

float jet_green( float value ) {

    return jet_base( value );
}

float jet_blue( float value ) {

    return jet_base( value + 0.6f );
}

color_rgba map_value_to_color(float val, float min, float max, COLORMAP_TYPE t) {

    float range = max - min;

    float value = (val - min)/(range);

    if (value <= 0.0f)
        value = 0.0f;
    if (value > 1.0f)
        value = 1.0f;

    switch (t) {

        case COLORMAP_JET: { return jet_colormap(value); }
        case COLORMAP_HAXBY: { return haxby_colormap(value); }
        case COLORMAP_SEISMIC: { return seismic_colormap(value); }
        case COLORMAP_GRAY: { return gray_colormap(value); }

    }
    //default
    //printf("map_value_to_color: unknown switch\n");
    //return jet_colormap(value);
#ifdef _WIN32
    return(jet_colormap(value)); // have to have return value
#endif
    return color_rgba(255, 255, 255, 255);
}

color_rgba jet_colormap(float value) {

    value *= 2.0f;
    value -= 1.0f;

    color_rgba color;

    //MATLAB's jet palette
    float r = jet_red(value);
    float g = jet_green(value);
    float b = jet_blue(value);

    color.r = (uint8_t)(r * 255.0f);
    color.g = (uint8_t)(g * 255.0f);
    color.b = (uint8_t)(b * 255.0f);
    color.a = 255;
    
    return color;
}

color_rgba gray_colormap(float value) {

    color_rgba color;

    float r = value;
    float g = value;
    float b = value;

    color.r = (uint8_t)(r * 255.0f);
    color.g = (uint8_t)(g * 255.0f);
    color.b = (uint8_t)(b * 255.0f);
    color.a = 255;

    return color;
}

color_rgba haxby_colormap(float value) {

    color_rgba color;

    unsigned low = (unsigned)(float)floor(value * (haxby_colors - 1));
    unsigned high = (unsigned)(float)ceil(value * (haxby_colors - 1));

    float v = (float)(haxby_colors - 1) * (value - (float)floor(value * (haxby_colors - 1))/(haxby_colors - 1));

    float r = (float)linear_interpolation( v, haxby[low][0], haxby[high][0] );
    float g = (float)linear_interpolation( v, haxby[low][1], haxby[high][1] );
    float b = (float)linear_interpolation( v, haxby[low][2], haxby[high][2] );

    color.r = (uint8_t)(r);
    color.g = (uint8_t)(g);
    color.b = (uint8_t)(b);
    color.a = 255;

    return color;

}

color_rgba seismic_colormap(float value) {

    color_rgba color;

    unsigned low = (unsigned)(float)floor(value * (seismic_colors - 1));
    unsigned high = (unsigned)(float)ceil(value * (seismic_colors - 1));

    float v = (float)(seismic_colors - 1) * (value - (float)floor(value * (seismic_colors - 1))/(seismic_colors - 1));

    float r = (float)linear_interpolation( v, seismic[low][0], seismic[high][0] );
    float g = (float)linear_interpolation( v, seismic[low][1], seismic[high][1] );
    float b = (float)linear_interpolation( v, seismic[low][2], seismic[high][2] );

    color.r = (uint8_t)(r);
    color.g = (uint8_t)(g);
    color.b = (uint8_t)(b);
    color.a = 255;

    return color;

}

}//: namespace data_utils
}//: namespace mic

