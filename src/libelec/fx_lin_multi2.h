#ifndef _FX_LIN_MULTI2_H_
#define _FX_LIN_MULTI2_H_

#include <acfutils/geom.h>
#include <acfutils/types.h>

#ifdef __cplusplus
extern "C" {
#endif

double fx_lin_multi2(double x, const struct vect2_s *points,
    size_t n_points, bool_t extrapolate);

#ifdef __cplusplus
}
#endif

#endif /* _FX_LIN_MULTI2_H_ */
