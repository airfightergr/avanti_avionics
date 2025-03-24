// height 1030
// width 771
//
#include "datarefs.h"
#include "plt_pfd_device.h"
#include "XPLMUtilities.h"
#include "acfutils/core.h"
#include "acfutils/geom.h"
#include "acfutils/glew.h"
#include "cairo-ft.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "acfutils/mt_cairo_render.h"
#include "acfutils/log.h"
#include "acfutils/dr.h"
#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "cairo.h"
#include "pfd.h"
#include <stdio.h>
#include <string.h>

XPLMCommandRef plt_pfd_pop = NULL;
int plt_pfd_pop_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon);

#define PLT_PFD_DISP_W 488
#define PLT_PFD_DISP_H 654
#define PLT_PFD_BEZEL_W 653
#define PLT_PFD_BEZEL_H 800

FT_Library ft_library;
FT_Face ft_face;
cairo_font_face_t* custom_font_face = NULL;

char bezel_image[2048];
char avanti_font[2048];

XPLMAvionicsID plt_pfd_handle;
static mt_cairo_render_t* plt_pfd_render;
static mt_cairo_render_t* plt_pfd_bezel_render;

void load_custom_font(const char* font_path) {
        // Load custom font
        // Implement font loading logic here
        if (FT_Init_FreeType(&ft_library)) {
                logMsg("Error: Failed to initialize FreeType library");
                return;
        }

        if (FT_New_Face(ft_library, font_path, 0, &ft_face)) {
                logMsg("Error: Failed to load font: %s", font_path);
                // FT_Done_FreeType(library);
                return;
        }

        custom_font_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
        logMsg("Custom font loaded: %s", font_path);
}

void load_resources(void) {
        // load images
        char* bezel_png = "pfd_bezel.png";
        strncpy(bezel_image, dir_path, sizeof(bezel_image));
        strncat(bezel_image, bezel_png, sizeof(bezel_image) - strlen(bezel_image) - 1);
        logMsg("Bezel Image: %s", bezel_image);

        // load fonts
        char* font_path = "AvantiP180.ttf";
        strncpy(avanti_font, dir_path, sizeof(avanti_font));
        strncat(avanti_font, font_path, sizeof(avanti_font) - strlen(avanti_font) - 1);
        logMsg("Avanti Font: %s", avanti_font);

        load_custom_font(avanti_font);
}


void draw_text_with_custom_font(cairo_t* cr, const char* text, double x, double y) {
        if (!custom_font_face) {
            logMsg("Error: Custom font not loaded");
            return;
        }

        cairo_set_font_face(cr, custom_font_face);
        cairo_set_font_size(cr, 22);
        cairo_move_to(cr, x, y);
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_show_text(cr, text);
        cairo_stroke(cr);
}


void plt_pfd_bezel_cb(cairo_t* cr, unsigned w, unsigned h, void* data) {
        UNUSED(data);

        cairo_surface_t *image;
        // cairo_surface_t *surface;

        image = cairo_image_surface_create_from_png(bezel_image);
        if (cairo_surface_status(image) != CAIRO_STATUS_SUCCESS) {
                logMsg("Error: Failed to load image: %s", bezel_image);
                return;
                printf("CAIRO_STATUS: %d\n", cairo_surface_status(image));
        }

        int width = cairo_image_surface_get_width(image);
        int height = cairo_image_surface_get_height(image);

        // Scale image to fit the screen
        double scale_x = (double)w / width;
        double scale_y = (double)h / height;
        cairo_save(cr);
        cairo_scale(cr, scale_x, scale_y);
        cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
        cairo_set_source_surface(cr, image, 0, 0);
        cairo_paint(cr);
        cairo_restore(cr);

        // Cleanup
        cairo_surface_destroy(image);
}

int plt_pfd_bezel_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);

        int outLeft, outTop, outRight, outBottom;

        XPLMGetAvionicsGeometry(plt_pfd_handle, &outLeft, &outTop, &outRight, &outBottom);

        vect2_t bezel_loc = VECT2(0, 0);
        mt_cairo_render_draw(plt_pfd_bezel_render, bezel_loc, VECT2(PLT_PFD_BEZEL_W, PLT_PFD_BEZEL_H));
        return 1;
}


void plt_pfd_display_cb(cairo_t* cr, unsigned w, unsigned h, void* ref_data) {
     UNUSED(w);
     UNUSED(h);
     UNUSED(ref_data);

     xp_data_t sim_data;
     drefs_get(&sim_data);

     int acf_hdg_int = (int)sim_data.ahrs_hdg;
     char heading_str[16];
     snprintf(heading_str, sizeof(heading_str), "%d", acf_hdg_int);

     cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
     cairo_paint(cr);
     cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

     cairo_set_line_width(cr, 4);

     // AP header
     cairo_set_source_rgb(cr, 0.9, 0.1, 0.1);
     cairo_rectangle(cr, 115.0, 10.0, 262, 37);
     cairo_fill(cr);

     // Speed tape
     cairo_set_source_rgb(cr, 0.1, 0.9, 0.1);
     cairo_rectangle(cr, 45.0, 69.0, 73, 203);
     cairo_fill(cr);

     cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
     cairo_arc(cr, 246.0, 518.0, 96.0, 0, 2 * M_PI);
     cairo_stroke(cr);

     double angle1 = -26.0  * (M_PI/180.0);  /* angles are specified */
     double angle2 = 206.0 * (M_PI/180.0);  /* in radians           */

     cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
     cairo_arc_negative(cr, 246.0, 518.0, 191.0, angle1, angle2);
     cairo_stroke(cr);
     // cairo_close_path(cr);

     // TEXT
     draw_text_with_custom_font(cr, heading_str, 328, 314);
}

int plt_pfd_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);
        vect2_t panel_loc = VECT2(0, 0);
        mt_cairo_render_draw(plt_pfd_render, panel_loc, VECT2(PLT_PFD_DISP_W, PLT_PFD_DISP_H));
        return 1;
}

void plf_pfd_draw_enable(void) {
        plt_pfd_render = mt_cairo_render_init(
                PLT_PFD_DISP_W, PLT_PFD_DISP_H,
                20, NULL, plt_pfd_display_cb,
                NULL, NULL);

        plt_pfd_bezel_render = mt_cairo_render_init(
                PLT_PFD_BEZEL_W, PLT_PFD_BEZEL_H,
                20, NULL, plt_pfd_bezel_cb,
                NULL, NULL);
}

void plt_pfd_draw_disable(void) {
        mt_cairo_render_fini(plt_pfd_render);
        plt_pfd_render = NULL;

        mt_cairo_render_fini(plt_pfd_bezel_render);
        plt_pfd_bezel_render = NULL;

        // Clear fonts
        if (ft_face) {
                FT_Done_Face(ft_face);
                ft_face = NULL;
        }

        if (ft_library) {
                FT_Done_FreeType(ft_library);
                ft_library = NULL;
        }
}

void plt_pfd_bezel_draw(float inAmbiantR, float inAmbiantG, float inAmbiantB, void *inRefcon) {

        plt_pfd_bezel_draw_loop(NULL, NULL, NULL);

}

void plt_pfd_screen_draw(void *inRefcon) {
        plt_pfd_draw_loop(NULL, NULL, NULL);
}

int plt_pfd_bezel_click_f(int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {

        return 1;
}


void plt_pfd_init(void) {

        char deviceID[64];
            snprintf(deviceID, 64, "PLT_PFD");

            char deviceName[256];
            snprintf(deviceName, 256, "Pilot PFD");

            XPLMCreateAvionics_t plt_pfd;
            plt_pfd.structSize = sizeof(plt_pfd);
            plt_pfd.screenWidth = 488;
            plt_pfd.screenHeight = 654;
            plt_pfd.bezelWidth = 653;
            plt_pfd.bezelHeight = 800;
            plt_pfd.screenOffsetX = 83;
            plt_pfd.screenOffsetY = 63;
            plt_pfd.drawOnDemand = 0;
            plt_pfd.bezelDrawCallback = plt_pfd_bezel_draw;
            plt_pfd.drawCallback = plt_pfd_screen_draw;
            plt_pfd.bezelClickCallback = &plt_pfd_bezel_click_f;
            plt_pfd.bezelRightClickCallback = NULL;
            plt_pfd.bezelScrollCallback = NULL;
            plt_pfd.bezelCursorCallback = NULL;
            plt_pfd.screenTouchCallback = NULL;
            plt_pfd.screenRightTouchCallback = NULL;
            plt_pfd.screenScrollCallback = NULL;
            plt_pfd.screenCursorCallback = NULL;
            plt_pfd.keyboardCallback = NULL;
            plt_pfd.brightnessCallback = NULL;
            plt_pfd.deviceID = deviceID;
            plt_pfd.deviceName = deviceName;
            plt_pfd_handle = XPLMCreateAvionicsEx(&plt_pfd);

            plt_pfd_pop = XPLMCreateCommand("avanti/avionics/plf_pfd/pop", "Pop PLT PFD");
            XPLMRegisterCommandHandler(plt_pfd_pop, plt_pfd_pop_handler, 1, (void *)0);

}

int plt_pfd_pop_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {

        if (XPLMIsAvionicsPopupVisible(plt_pfd_handle) == 0) {
                if (inPhase == 0) {
                        XPLMSetAvionicsPopupVisible(plt_pfd_handle, 1);
                }
        } else {
                if (inPhase == 0) {
                        XPLMSetAvionicsPopupVisible(plt_pfd_handle, 0);
                }
        }
        return 0;
}

void plt_pfd_destroy(void) {
        XPLMDestroyAvionics(plt_pfd_handle);
}
