// height 1030
// width 771
//
#include "datarefs.h"
#include "plt_pfd_device.h"
#include "XPLMUtilities.h"
#include "acfutils/core.h"
#include "acfutils/geom.h"
// #include "acfutils/glew.h"
#include "acfutils/helpers.h"
// #include "cairo-ft.h"
#include "ft2build.h"
#include FT_FREETYPE_H
// #include "acfutils/mt_cairo_render.h"
#include "acfutils/log.h"
#include "acfutils/dr.h"
#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"

#if IBM
#include <windows.h>
#include <GL/gl.h>
#endif
#ifdef _WIN32
#include <GL/glext.h>
#endif
#if LIN
#include <GL/gl.h>
#endif
#if __GNUC__ && APL
#include <OpenGL/gl.h>
#endif
#if __GNUC__ && IBM
#include <GL/gl.h>
#endif

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg.h"
#include "nanovg/nanovg_gl.h"

// #include "cairo.h"
#include "pfd.h"
#include <stdio.h>
#include <string.h>

XPLMCommandRef plt_pfd_pop = NULL;
int plt_pfd_pop_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon);

NVGcontext* g_pdf_nanovg_context = NULL;
// Global variable to store the NanoVG image handle for the compass card
static int g_compass_card_image_id = -1; // -1 indicates not loaded

#define PLT_PFD_DISP_W 488
#define PLT_PFD_DISP_H 654
#define PLT_PFD_BEZEL_W 653
#define PLT_PFD_BEZEL_H 800

FT_Library ft_library;
FT_Face ft_face;
// cairo_font_face_t* custom_font_face = NULL;

char bezel_image[2048];
char avanti_font[2048];
char compass_img[1024];

XPLMAvionicsID plt_pfd_handle;
// static mt_cairo_render_t* plt_pfd_render;
// static mt_cairo_render_t* plt_pfd_bezel_render;

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

        // custom_font_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
        // logMsg("Custom font loaded: %s", font_path);
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
        //
        if (nvgCreateFont(g_pdf_nanovg_context, "avanti_pdf_font", avanti_font) == -1) {
            XPLMDebugString("PltPDF: Could not load font 'AvantiP180.ttf' for NanoVG!\n");
            XPLMDebugString("Attempted font path: "); // Add debug info for the path
            XPLMDebugString(avanti_font);             // Print the path that failed
            XPLMDebugString("\n"); // Newline for cleaner log
        } else {
            XPLMDebugString("PltPDF: AvantiP180.ttf font loaded into NanoVG successfully.\n");
        }

        char* compass_img_path = "compass.png";
        strncpy(compass_img, dir_path, sizeof(compass_img));
        strncat(compass_img, compass_img_path, sizeof(compass_img) - strlen(compass_img) - 1);
        XPLMDebugString("Compass image path: ");
        XPLMDebugString(compass_img);
        XPLMDebugString("\n");

        g_compass_card_image_id = nvgCreateImage(g_pdf_nanovg_context, compass_img, 0);
        if (g_compass_card_image_id == -1) {
             XPLMDebugString("PltPDF: Could not load compass_card.png for NanoVG! Check path and file.\n");
             XPLMDebugString(compass_img);
             XPLMDebugString("\n");
         } else {
             XPLMDebugString("PltPDF: compass_card.png loaded into NanoVG successfully.\n");
         }
}


void draw_text_with_custom_font(cairo_t* cr, const char* text, double x, double y) {
        // if (!custom_font_face) {
        //     logMsg("Error: Custom font not loaded");
        //     return;
        // }

        // cairo_set_font_face(cr, custom_font_face);
        // cairo_set_font_size(cr, 22);
        // cairo_move_to(cr, x, y);
        // cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        // cairo_show_text(cr, text);
        // cairo_stroke(cr);
}


void plt_pfd_bezel_cb(cairo_t* cr, unsigned w, unsigned h, void* data) {
        UNUSED(data);

        // cairo_surface_t *image;
        // cairo_surface_t *surface;

        // image = cairo_image_surface_create_from_png(bezel_image);
        // if (cairo_surface_status(image) != CAIRO_STATUS_SUCCESS) {
        //         logMsg("Error: Failed to load image: %s", bezel_image);
        //         return;
        //         printf("CAIRO_STATUS: %d\n", cairo_surface_status(image));
        // }

        // int width = cairo_image_surface_get_width(image);
        // int height = cairo_image_surface_get_height(image);

        // // Scale image to fit the screen
        // double scale_x = (double)w / width;
        // double scale_y = (double)h / height;
        // cairo_save(cr);
        // cairo_scale(cr, scale_x, scale_y);
        // cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
        // cairo_set_source_surface(cr, image, 0, 0);
        // cairo_paint(cr);
        // cairo_restore(cr);

        // // Cleanup
        // cairo_surface_destroy(image);
}

int plt_pfd_bezel_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);

        int outLeft, outTop, outRight, outBottom;

        XPLMGetAvionicsGeometry(plt_pfd_handle, &outLeft, &outTop, &outRight, &outBottom);

        // vect2_t bezel_loc = VECT2(0, 0);
        // mt_cairo_render_draw(plt_pfd_bezel_render, bezel_loc, VECT2(PLT_PFD_BEZEL_W, PLT_PFD_BEZEL_H));
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

     // cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
     // cairo_paint(cr);
     // cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

     // cairo_set_line_width(cr, 4);

     // AP header
     // cairo_set_source_rgb(cr, 0.9, 0.1, 0.1);
     // cairo_rectangle(cr, 115.0, 10.0, 262, 37);
     // cairo_fill(cr);

     // Speed tape
     // cairo_set_source_rgb(cr, 0.1, 0.9, 0.1);
     // cairo_rectangle(cr, 45.0, 69.0, 73, 203);
     // cairo_fill(cr);

     // cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
     // cairo_arc(cr, 246.0, 518.0, 96.0, 0, 2 * M_PI);
     // cairo_stroke(cr);

     double angle1 = -26.0  * (M_PI/180.0);  /* angles are specified */
     double angle2 = 206.0 * (M_PI/180.0);  /* in radians           */

     // cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
     // cairo_arc_negative(cr, 246.0, 518.0, 191.0, angle1, angle2);
     // cairo_stroke(cr);
     // cairo_close_path(cr);

     // TEXT
     // draw_text_with_custom_font(cr, heading_str, 328, 314);
}

int plt_pfd_draw_loop(XPLMDrawingPhase phase, int isBefore, void* refcon) {
        UNUSED(phase);
        UNUSED(isBefore);
        UNUSED(refcon);
        // vect2_t panel_loc = VECT2(0, 0);
        // mt_cairo_render_draw(plt_pfd_render, panel_loc, VECT2(PLT_PFD_DISP_W, PLT_PFD_DISP_H));
        return 1;
}

void plf_pfd_draw_enable(void) {
        // plt_pfd_render = mt_cairo_render_init(
        //         PLT_PFD_DISP_W, PLT_PFD_DISP_H,
        //         20, NULL, plt_pfd_display_cb,
        //         NULL, NULL);

        // plt_pfd_bezel_render = mt_cairo_render_init(
        //         PLT_PFD_BEZEL_W, PLT_PFD_BEZEL_H,
        //         20, NULL, plt_pfd_bezel_cb,
        //         NULL, NULL);
}

void plt_pfd_draw_disable(void) {
        // mt_cairo_render_fini(plt_pfd_render);
        // plt_pfd_render = NULL;

        // mt_cairo_render_fini(plt_pfd_bezel_render);
        // plt_pfd_bezel_render = NULL;

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
        // plt_pfd_draw_loop(NULL, NULL, NULL);
        if (g_pdf_nanovg_context == NULL) {     // Safety check: ensure NanoVG context is valid
            return;
        }

        int outLeft, outTop, outRight, outBottom;
        XPLMGetAvionicsGeometry(plt_pfd_handle, &outLeft, &outTop, &outRight, &outBottom);

        // Calculate width and height from the returned geometry
        // Note: X-Plane's coordinates usually have Y-axis increasing upwards.
        // So, height = Top - Bottom.
        int screen_width = outRight - outLeft;
        int screen_height = outTop - outBottom;

        // Ensure dimensions are positive (safety check, though unlikely to be negative)
        if (screen_width <= 0 || screen_height <= 0) {
            XPLMDebugString("plt_pfd_screen_draw: Invalid screen geometry detected.\n");
            return;
        }

        // --- OpenGL State Management (CRITICAL!) ---
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

        // Begin NanoVG Frame
        nvgBeginFrame(g_pdf_nanovg_context, (float)screen_width, (float)screen_height, 1.0f);

        // --- YOUR NANOVG DRAWING CODE HERE ---
        // Example 1: Black background fill for the entire screen
        nvgBeginPath(g_pdf_nanovg_context);
        nvgRect(g_pdf_nanovg_context, 0, 0, (float)screen_width, (float)screen_height);
        nvgFillColor(g_pdf_nanovg_context, nvgRGBA(0, 0, 0, 255));
        nvgFill(g_pdf_nanovg_context);

        // Draw Circle
        float center_x = (float)screen_width / 2.0f;
        float center_y = (float)screen_height / 2.0f;
        float radius = fmin((float)screen_width/2, (float)screen_height/2);

        nvgBeginPath(g_pdf_nanovg_context);
        nvgCircle(g_pdf_nanovg_context, center_x, center_y, radius);
        nvgFillColor(g_pdf_nanovg_context, nvgRGBA(50, 50, 200, 200));
        nvgFill(g_pdf_nanovg_context);
        nvgStrokeColor(g_pdf_nanovg_context, nvgRGBA(255, 255, 255, 255));
        nvgStrokeWidth(g_pdf_nanovg_context, 2.0f);
        nvgStroke(g_pdf_nanovg_context);

        const char* text_message = "HELLO FROM NANOVG!";
        nvgFontSize(g_pdf_nanovg_context, 40.0f);
        nvgFontFace(g_pdf_nanovg_context, "avanti_pdf_font");
        nvgTextAlign(g_pdf_nanovg_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(g_pdf_nanovg_context, nvgRGBA(255, 255, 0, 255));
        nvgText(g_pdf_nanovg_context, center_x, center_y, text_message, NULL);

        // Image
        // int img_w, img_h;
        // nvgImageSize(g_pdf_nanovg_context, g_compass_card_image_id, &img_w, &img_h);

        // float compass_x = (float)screen_width / 2.0f;
        // float compass_y = (float)screen_height / 2.0f;
        // float compass_size = fmin((float)screen_width, screen_height) * 0.9f;

        // float rotation_radians = 0.0f;
        // NVGpaint image_paint = nvgImagePattern(g_pdf_nanovg_context, compass_x - compass_size, compass_y - compass_size, compass_size, compass_size, rotation_radians, g_compass_card_image_id, 1.0f);
        // nvgBeginPath(g_pdf_nanovg_context);
        // nvgCircle(g_pdf_nanovg_context, compass_x, compass_y, compass_size / 2.0f);
        // nvgFillPaint(g_pdf_nanovg_context, image_paint);
        // nvgFill(g_pdf_nanovg_context);

        // // If you need to transform the *entire drawing context* (e.g., to rotate the card),
        //     // you apply transformations *before* calling drawing commands.
        //     // NanoVG uses a matrix stack, so you can save/restore state.
        //     nvgSave(g_pdf_nanovg_context); // Save current transformation state

        //     // Translate to the center of where you want the compass card to be
        //     nvgTranslate(g_pdf_nanovg_context, compass_x, compass_y);

        //     // Rotate by the heading (converted to radians and possibly inverted based on how your image's N is aligned)
        //     // If 0 degrees in your image is North (straight up), and you want that N to point to aircraft heading,
        //     // you rotate the image by -heading (because if aircraft turns 90 degrees right, card turns 90 degrees left).
        //     // M_PI is defined in math.h, or define NVG_PI in nanovg.h if not available.
        //     // Make sure to convert degrees to radians: degrees * (NVG_PI / 180.0f)
        //     // Example: float current_heading_deg = XPLMGetDataf(g_mag_heading_dr);
        //     // float rotation_angle = -(current_heading_deg) * NVG_PI / 180.0f; // Rotate counter-clockwise from 0 (North)
        //     // Let's try rotating it by a fixed amount for demonstration
        //     nvgRotate(g_pdf_nanovg_context, (float)NVG_PI / 4.0f); // Rotate 45 degrees clockwise for testing

        //     // Scale the image to fit the desired size if it's not already
        //     // The image itself is probably higher resolution than your desired display size.
        //     // Calculate scaling factor: desired_size / original_image_size
        //     // float scale_factor_x = compass_size / img_w;
        //     // float scale_factor_y = compass_size / img_h;
        //     // nvgScale(g_pdf_nanovg_context, scale_factor_x, scale_factor_y);

        //     // Draw the image at its new "local" origin (which is now 0,0 after translate/rotate)
        //     // You must draw it as a rectangle or circle with the image_paint
        //     // Re-create the paint with the new transformed context
        //     image_paint = nvgImagePattern(g_pdf_nanovg_context,
        //                                    -compass_size / 2.0f, // Draw from top-left of the rotated canvas
        //                                    -compass_size / 2.0f,
        //                                    compass_size, compass_size,
        //                                    0.0f, // No pattern rotation, as we're rotating the context
        //                                    g_compass_card_image_id,
        //                                    1.0f);

        //     nvgBeginPath(g_pdf_nanovg_context);
        //     nvgCircle(g_pdf_nanovg_context, 0, 0, compass_size / 2.0f); // Draw at the new 0,0 (center)
        //     nvgFillPaint(g_pdf_nanovg_context, image_paint);
        //     nvgFill(g_pdf_nanovg_context);

        //     nvgRestore(g_pdf_nanovg_context); // Restore previous transformation state

        nvgEndFrame(g_pdf_nanovg_context);

        glPopClientAttrib();
        glPopMatrix();
        glPopAttrib();
}

int plt_pfd_bezel_click_f(int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {

        return 1;
}


void plt_pfd_init(void) {

        char deviceID[64];
            snprintf(deviceID, 64, "PLT_PFD");

            char deviceName[256];
            snprintf(deviceName, 256, "Pilot PFD");

            g_pdf_nanovg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
            if (g_pdf_nanovg_context == NULL) {
                XPLMDebugString("PltPDF: Failed to create NanoVG GL3 context!\n");
                return;
            }
            XPLMDebugString("PltPDF: NanoVG GL3 context created for PFD.\n");

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
    if (g_pdf_nanovg_context) {
        nvgDeleteGL3(g_pdf_nanovg_context);
        g_pdf_nanovg_context = NULL;
        XPLMDebugString("PltPDF: NanoVG GL3 context destroyed for PFD.\n");
    }
    // if (g_compass_card_image_id != -1) {
    //         nvgDeleteImage(g_pdf_nanovg_context, g_compass_card_image_id);
    //         g_compass_card_image_id = -1;
    //         XPLMDebugString("PltPDF: Compass card image destroyed.\n");
    //     }

        XPLMDestroyAvionics(plt_pfd_handle);
}
