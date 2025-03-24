/*
* datarefs.h
*
* Created by: Ilias Tselios
* email: ilias@tselios.com
* Date: 2023-09-01
* Copyright (c) 2025 Ilias Tselios
*
*/

#ifndef DATAREFS_H
#define DATAREFS_H

#include <stdbool.h>

typedef struct {
        float ahrs_hdg;
} xp_data_t;

// This sets up all the datarefs and all. Call this early, probably in XPluginEnable
void drefs_init(void);

// This tears down any data we had to set up. Call this in XPluginDisable.
void drefs_fini(void);

// This **must** be called on the X-Plane thread. You probably want to call it every frame
// in a flight loop callback.
void drefs_update(void);

// This can be called on any thread, and gives you the data that was obtained from datarefs in
// drefs_update. The way to call them is:
//
// xp_data_t data = {};
// xp_data_get(&data);
// // use the data here!
//
void drefs_get(xp_data_t *data);

#endif //DATAFER_H
