#include "colors.h"

#include <stdlib.h>
#include <math.h>

GdkRGBA hsv_to_rgb(double h, double s, double v) {
    h = fmod(h, 360);
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60, 2) - 1));
    double m = v - c;

    int region = (int) floor(h / 60.0);
    switch (region) {
        case 0: return (GdkRGBA) {red: c, green: x, blue: 0};
        case 1: return (GdkRGBA) {red: x, green: c, blue: 0};
        case 2: return (GdkRGBA) {red: 0, green: c, blue: x};
        case 3: return (GdkRGBA) {red: 0, green: x, blue: c};
        case 4: return (GdkRGBA) {red: x, green: 0, blue: c};
        case 5: return (GdkRGBA) {red: c, green: 0, blue: x};
        default:
            return (GdkRGBA) {red: 0, green: 0, blue: 0};
    }
}