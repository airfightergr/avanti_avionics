#ifndef PLT_PFD_DEVICE_H
#define PLT_PFD_DEVICE_H

#include "pfd.h"
#include "acfutils/dr.h"

void plt_pfd_init(void);
void plf_pfd_draw_enable(void);
void plt_pfd_draw_disable(void);
void load_resources(void);

static dr_t heading_ahars;

#endif  // PLT_PFD_DEVICE_H
