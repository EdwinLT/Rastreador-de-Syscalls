#pragma once

#include <gdk/gdk.h>

// Convert a color in HSV format to RGB
// h: 0-360
// s: 0-1
// v: 0-1
GdkRGBA hsv_to_rgb(gdouble h, gdouble s, gdouble b);

void create_n_hues(guint n, gboolean shuffle, gdouble *output);