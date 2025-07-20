#include "fx_lin_multi2.h"
#include <acfutils/assert.h>
#include <acfutils/math.h>
#include <math.h>

double
fx_lin_multi2(double x, const struct vect2_s *points, size_t n_points,
    bool_t extrapolate)
{
    ASSERT(points != NULL);
    ASSERT3U(n_points, >=, 2);

    for (;;) {
        vect2_t p1 = points[0], p2 = points[1];

        ASSERT3F(p1.x, <, p2.x);

        if (x < p1.x) {
            /* X outside of range to the left */
            if (extrapolate)
                return (fx_lin(x, p1.x, p1.y, p2.x, p2.y));
            break;
        }
        /* X in range of current segment */
        if (x <= p2.x)
            return (fx_lin(x, p1.x, p1.y, p2.x, p2.y));
        /* X outside of range to the right */
        if (n_points == 2) {
            if (extrapolate)
                return (fx_lin(x, p1.x, p1.y, p2.x, p2.y));
            break;
        }

        points++;
        n_points--;
    }

    return (NAN);
}
