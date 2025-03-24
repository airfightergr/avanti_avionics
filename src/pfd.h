//
// Created by ilias on 6/25/23.
//

#ifndef AVANTI_PFD_H
#define AVANTI_PFD_H

#include <XPLMUtilities.h>
#include <XPLMGraphics.h>
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>

#include <acfutils/mt_cairo_render.h>


void drawing_init(void);
void drawing_enable(void);
void drawing_disable(void);
void paths(void);
static void display_render_cb(cairo_t *cr, unsigned w, unsigned h, void *data);
static int draw_loop(XPLMDrawingPhase phase, int is_before, void *refcon);
void drawing_receiveMessage(void);

extern char acf_dir_path[1024];
extern char dir_path[1024];
extern char resources_path[1024];

#endif //AVANTI_PFD_H
