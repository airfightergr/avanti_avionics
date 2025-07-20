

#include "datarefs.h"
#include "tablet_device.h"
#include "tablet_ui.h"
#include "XPLMUtilities.h"
#include "acfutils/core.h"
#include "acfutils/geom.h"
#include "acfutils/glew.h"
#include "cairo-ft.h"
#include "ft2build.h"
#include <unistd.h>
#include FT_FREETYPE_H
// #include "acfutils/mt_cairo_render.h"
#include "acfutils/log.h"
#include "acfutils/dr.h"
#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
// #include "cairo.h"
#include "pfd.h"
#include <stdio.h>
#include <string.h>

XPLMCommandRef tablet_scr_pop = NULL;
int tablet_scr_pop_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon);

#define TABLET_SCREEN_W 718
#define TABLET_SCREEN_H 448
// static mt_cairo_render_t* tablet_screen_render;
// static mt_cairo_render_t* tablet_bezel_render;
XPLMAvionicsID tablet_screen_handle;

void tablet_bezel_draw_cb(cairo_t* cr, unsigned w, unsigned h, void* ref_data) {
        UNUSED(w);
        UNUSED(h);
        UNUSED(ref_data);

        // cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
        // cairo_paint(cr);
        // cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

        // cairo_set_source_rgb(cr, 0, 0, 0);
        // cairo_rectangle(cr, 0, 0, 718, 448);
        // cairo_fill(cr);
}

int tablet_bezel_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);

        // vect2_t tablet_loc = VECT2(0, 0);
        // mt_cairo_render_draw(tablet_bezel_render, tablet_loc, VECT2(TABLET_SCREEN_W, TABLET_SCREEN_H));
        return 1;
}

void tablet_scr_bezel_draw(float inAmbientR, float inAmbientG, float inAmbientB, void *inRefcon) {
        tablet_bezel_draw_loop(NULL, NULL, NULL);
}

void tablet_screen_draw_cb(cairo_t* cr, unsigned w, unsigned h, void* ref_data) {
        UNUSED(w);
        UNUSED(h);
        UNUSED(ref_data);

        // cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
        // cairo_paint(cr);
        // cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

        // cairo_set_source_rgb(cr, 0.9, 0.1, 0.1);
        // cairo_rectangle(cr, 0, 0, 718, 448);
        // cairo_fill(cr);

        // cairo_set_source_rgb(cr, 0.1, 0.9, 0.1);
        // cairo_rectangle(cr, 10, 10, 698, 428);
        // cairo_fill(cr);

        // // draw a button
        // cairo_set_source_rgb(cr, 0.1, 0.0, 0.9);
        // cairo_rectangle(cr, 100, 100, 100, 50);
        // cairo_fill(cr);

}

int tablet_screen_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);

        // vect2_t tablet_loc = VECT2(0, 0);
        // mt_cairo_render_draw(tablet_screen_render, tablet_loc, VECT2(TABLET_SCREEN_W, TABLET_SCREEN_H));
        return 1;
}

void tablet_draw_enable(void) {
        // tablet_screen_render = mt_cairo_render_init(
        //                         TABLET_SCREEN_W, TABLET_SCREEN_H,
        //                         20, NULL, tablet_screen_draw_cb,
        //                         NULL, NULL);

        // tablet_bezel_render = mt_cairo_render_init(
        //                         TABLET_SCREEN_W, TABLET_SCREEN_H,
        //                         20, NULL, tablet_bezel_draw_cb,
        //                         NULL, NULL);
}

void tablet_draw_disable(void) {
        // mt_cairo_render_fini(tablet_screen_render);
        // tablet_screen_render = NULL;

        // mt_cairo_render_fini(tablet_bezel_render);
        // tablet_bezel_render = NULL;
}

void tablet_scr_screen_draw(void* inRefcon) {
        // tablet_screen_draw_loop(NULL, NULL, NULL);
        TabletUIDraw(0, 0, 718, 448, NULL);
}

int tablet_scr_bezel_click_f(int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {

        return 1;
}

int tablet_scr_touch_f(int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {
        if (x > 100 && x < 200 && y < TABLET_SCREEN_H - 100 && y > TABLET_SCREEN_H - 150){
                if (inMouse == 1) {
                logMsg("Button Pressed! | X=%d, Y=%d", x, y);
                printf("Button Pressed! | X=%d, Y=%d\n", x, y);
                }
        }
        return 1;
}

void tablet_init(void) {

        char deviceID[64];
        snprintf(deviceID, 64, "tablet_screen");

        char deviceName[256];
        snprintf(deviceName, 256, "Tablet Touch Screen");

        XPLMCreateAvionics_t tablet_scr;
        tablet_scr.structSize = sizeof(tablet_scr);
        tablet_scr.screenWidth = 718;
        tablet_scr.screenHeight = 448;
        tablet_scr.bezelWidth = 718;
        tablet_scr.bezelHeight = 448;
        tablet_scr.screenOffsetX = 0;
        tablet_scr.screenOffsetY = 0;
        tablet_scr.drawOnDemand = 0;
        tablet_scr.bezelDrawCallback = tablet_scr_bezel_draw;
        tablet_scr.drawCallback = tablet_scr_screen_draw;
        tablet_scr.bezelClickCallback = NULL;
        tablet_scr.bezelRightClickCallback = NULL;
        tablet_scr.bezelScrollCallback = NULL;
        tablet_scr.bezelCursorCallback = NULL;
        tablet_scr.screenTouchCallback = &tablet_scr_touch_f;
        tablet_scr.screenRightTouchCallback = NULL;
        tablet_scr.screenScrollCallback = NULL;
        tablet_scr.screenCursorCallback = NULL;
        tablet_scr.keyboardCallback = NULL;
        tablet_scr.brightnessCallback = NULL;
        tablet_scr.deviceID = deviceID;
        tablet_scr.deviceName = deviceName;
        tablet_screen_handle = XPLMCreateAvionicsEx(&tablet_scr);

        tablet_scr_pop = XPLMCreateCommand("avanti/avionics/tablet/pop", "Pop tablet");
        XPLMRegisterCommandHandler(tablet_scr_pop, tablet_scr_pop_handler, 1, (void *)0);

}

int tablet_scr_pop_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon){
        if (XPLMIsAvionicsPopupVisible(tablet_screen_handle) == 0) {
                if (inPhase == 0) {
                        XPLMSetAvionicsPopupVisible(tablet_screen_handle, 1);
                }
        } else {
                if (inPhase == 0) {
                        XPLMSetAvionicsPopupVisible(tablet_screen_handle, 0);
                }
        }

        return 0;
}

void tablet_destroy(void) {
        XPLMDestroyAvionics(tablet_screen_handle);
}
