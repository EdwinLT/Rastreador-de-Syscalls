#pragma once

#include <gdk/gdk.h>

// Convert a color in HSV format to RGB
// h: 0-360
// s: 0-1
// v: 0-1
GdkRGBA hsv_to_rgb(double h, double s, double b);

double *create_n_hues(size_t n, gboolean shuffle);