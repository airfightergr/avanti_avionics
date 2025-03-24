/*
Plugin for P-180 Avanti II for X-Plane
ilias@tselios.com - All rights reserved - 2023.

Scope to understand how work with c/c++ and produce a working plugin for X-Plane

*/

// system includes
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMDisplay.h"

#include "datarefs.h"
#include "units_conv.h"
#include "pfd.h"
#include "plt_pfd_device.h"

#include <acfutils/log.h>

// Log buffer
char logbuff[256];

//  Add dataref to DRE message
#define MSG_ADD_DATAREF 0x01000000

#if IBM
#include <windows.h>
#endif
#if LIN
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#if __GNUC__ && APL
#include <OpenGL/gl.h>
#endif
#if __GNUC__ && IBM
#include <GL/gl.h>
#endif

/* This will be your custom logging function */
static void my_dbg_logger(const char *str)
{
        XPLMDebugString(str);
}


// Our flight loop callback
float MainFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);

// Plugin Start
PLUGIN_API int XPluginStart(char* outName, char* outSig,char* outDesc){

        XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

        // Plugin Info
        strcpy(outName, "P-180 Avanti II Avionics");
        strcpy(outSig, "tselios.avanti.avionics");
        strcpy(outDesc, "Avionics Plugin for Avanti");

        XPLMRegisterFlightLoopCallback(MainFlightLoopCallback, -1, NULL);

        XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");

        // init drawing

        drefs_init();

        drawing_init();

        plt_pfd_init();

        log_init(my_dbg_logger, "[P180-AVANTI_AVIONICS]: ");

        if (PluginID != XPLM_NO_PLUGIN_ID)
        {

        }

return 1;

}


PLUGIN_API void XPluginStop(void)
{
        // Unregister acfutils logging
        log_fini();

        // uregister drawing
        drawing_disable();
        plt_pfd_draw_disable();

}


PLUGIN_API int XPluginEnable(void)
{

        drawing_enable();
        load_resources();
        plf_pfd_draw_enable();

        return 1;
}


PLUGIN_API void XPluginDisable(void)
{
        drefs_fini();
}


PLUGIN_API void XPluginReceiveMessage(XPLMPluginID    inFromWho, int inMessage,  void *           inParam)
{

}

float MainFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon) {

        drefs_update();

        return 0.01;
}
