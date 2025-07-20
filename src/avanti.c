/*
Plugin for P-180 Avanti II for X-Plane
ilias@tselios.com - All rights reserved - 2023.

Scope to understand how work with c/c++ and produce a working plugin for X-Plane

*/

// system includes
#include <sched.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMDisplay.h"

#if IBM
#include <windows.h>
#include <GL/gl.h>
#endif
#ifdef _WIN32
#include <GL/glext.h>
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

#include "datarefs.h"
#include "tablet_ui.h"
#include "units_conv.h"
#include "pfd.h"
#include "plt_pfd_device.h"
#include "tablet_device.h"

#include "acfutils/log.h"

// #include "libelec/libelec.h"

// Log buffer
char logbuff[256];

//  Add dataref to DRE message
#define MSG_ADD_DATAREF 0x01000000


#define PLUGIN_VERSION "1.0.0"

//LIBELEC stuff
// #define XPLANE
// #define LIBELEC_WITH_DRS
// static elec_sys_t* elecsys = NULL;

/* This will be your custom logging function */
static void my_dbg_logger(const char *str)
{
        XPLMDebugString(str);
}

float deferredStart(float meTime, float loopTime, int counter, void *refCon) {
  logMsg("Deffered Start");
  // char *elecFile = "/mnt/916d7a1f-7d19-4354-823b-6606cd3a516e/X-Plane 12/Aircraft/ILIAS/P180_Avanti_II/plugins/avanti_avionics/resources/elec.net";
  // if (elecFile) {
  //     logMsg("Elec net file found: %s", elecFile);
  // } else {
  //     logMsg("Elec net file not found");
  // }
  // elecsys = libelec_new(elecFile);

  //  libelec_sys_start(elecsys);

  return 0;
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
        XPLMRegisterFlightLoopCallback(deferredStart, -1, NULL);

        XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");

        // init drawing

        drefs_init();

        drawing_init();

        plt_pfd_init();

        tablet_init();

        log_init(my_dbg_logger, "[P180-AVANTI_AVIONICS]: ");


        if (PluginID != XPLM_NO_PLUGIN_ID)
        {

        }

        logMsg("Plugin version: %s", PLUGIN_VERSION);

return 1;

}


PLUGIN_API void XPluginStop(void)
{
        // Unregister acfutils logging
        log_fini();

        // uregister drawing
        drawing_disable();
        plt_pfd_draw_disable();
        tablet_draw_disable();

}


PLUGIN_API int XPluginEnable(void)
{

        drawing_enable();
        load_resources();
        plf_pfd_draw_enable();
        tablet_draw_enable();
        TabletUIStart();
        // libelec_sys_start(elecsys);

        return 1;
}

PLUGIN_API void XPluginDisable(void)
{
        drefs_fini();
        plt_pfd_destroy();
        tablet_destroy();
        TabletUIStop();
        // libelec_sys_stop(elecsys);
        // libelec_destroy(elecsys);
}


PLUGIN_API void XPluginReceiveMessage(XPLMPluginID    inFromWho, int inMessage,  void * inParam)
{

}

float MainFlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon) {

        drefs_update();

        return 0.01;
}
