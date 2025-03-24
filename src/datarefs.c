/*
* datarefs.c
*
* Created by: Ilias Tselios
* email: ilias@tselios.com
* Date: 2023-09-01
* Copyright (c) 2025 Ilias Tselios
*
*/


#include "datarefs.h"
#include "acfutils/assert.h"
#include "acfutils/dr.h"
#include "acfutils/safe_alloc.h"
#include "acfutils/thread.h"


static mutex_t data_mutex;
static xp_data_t data;
static struct {
    dr_t ahars_heading;
} dr;


static bool is_init = false;

void drefs_init(void) {
        ASSERT(!is_init);

        // Init or mutex
        mutex_init(&data_mutex);

        // Make sure that our saved data is a known value (0)
        BZERO(&data);

        // Fetch datarefs
        fdr_find(&dr.ahars_heading, "sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_pilot");

       	// And mark ourselves as initialised.
	is_init = true;
}


void drefs_fini(void){
        ASSERT(is_init);

	// Clean up our mutex.
	mutex_destroy(&data_mutex);

	// Mark ourselves as not initialised.
	is_init = false;
}


void drefs_update(void){
        ASSERT(is_init);

       	// By "entering" the mutex, we lock it: that means that any other thread that tries to
	// enter the mutex will pause until we exit it.
	mutex_enter(&data_mutex);

	data.ahrs_hdg = dr_getf(&dr.ahars_heading);

	// When we're done modifying fields in `data`, we can "exit"/release the mutex
	mutex_exit(&data_mutex);
}

void drefs_get(xp_data_t *out){
       	// Since we are about to read from our own copy of data, we need to first enter the mutex!
	mutex_enter(&data_mutex);
	// Then we can copy our own version of data to `out`
	*out = data;
	// And release the mutex when we're done so other threads can do things here.
	mutex_exit(&data_mutex);
}
