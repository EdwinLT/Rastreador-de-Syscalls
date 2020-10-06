#include "colors.h"

#include <stdlib.h>
#include <math.h>

static gdouble wrap_hue(gdouble h) {
    gdouble result = fmod(h, 360);
    if (h < 0.0) h += 360.0;
    return result;
}

GdkRGBA hsv_to_rgb(gdouble h, gdouble s, gdouble v) {
    h = wrap_hue(h);
    s = CLAMP(s, 0.0, 1.0);
    v = CLAMP(v, 0.0, 1.0);

    gdouble c = v * s;
    gdouble x = c * (1 - fabs(fmod(h / 60, 2) - 1));
    gdouble m = v - c;

    switch ((int) floor(h / 60.0)) {
        case 0: return (GdkRGBA) {red: c+m, green: x+m, blue: m,   alpha: 1.0};
        case 1: return (GdkRGBA) {red: x+m, green: c+m, blue: m,   alpha: 1.0};
        case 2: return (GdkRGBA) {red: m,   green: c+m, blue: x+m, alpha: 1.0};
        case 3: return (GdkRGBA) {red: m,   green: x+m, blue: c+m, alpha: 1.0};
        case 4: return (GdkRGBA) {red: x+m, green: m,   blue: c+m, alpha: 1.0};
        case 5: return (GdkRGBA) {red: c+m, green: m,   blue: x+m, alpha: 1.0};
        default:
            return (GdkRGBA) {red: 0, green: 0, blue: 0, alpha: 0};
    }
}

void create_n_hues(guint n, gboolean shuffle, gdouble *output) {
    for (guint i = 0; i < n; i++) {
        output[i] = 360.0 * ((gdouble) i / n);
    }

    if (shuffle && n > 1) {
        for (guint i = n - 1; i > 0; i--) {
            guint j = (guint) (rand() % (i + 1));
            gdouble temp = output[i];
            output[i] = output[j];
            output[j] = temp;
        }
    }
}