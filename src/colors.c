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
        case 0: return (GdkRGBA) {red: c+m, green: x+m, blue: m};
        case 1: return (GdkRGBA) {red: x+m, green: c+m, blue: m};
        case 2: return (GdkRGBA) {red: m,   green: c+m, blue: x+m};
        case 3: return (GdkRGBA) {red: m,   green: x+m, blue: c+m};
        case 4: return (GdkRGBA) {red: x+m, green: m,   blue: c+m};
        case 5: return (GdkRGBA) {red: c+m, green: m,   blue: x+m};
        default:
            return (GdkRGBA) {red: 0, green: 0, blue: 0};
    }
}

double *create_n_hues(guint n, gboolean shuffle) {
    double *hues = malloc(n * sizeof(double));

    for (guint i = 0; i < n; i++) {
        hues[i] = 360.0 * ((double) i / n);
    }

    if (shuffle && n > 1) {
        for (guint i = n - 1; i > 0; i--) {
            guint j = (guint) (rand() % (i + 1));
            double temp = hues[i];
            hues[i] = hues[j];
            hues[j] = temp;
        }
    }

    return hues;
}