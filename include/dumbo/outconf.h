#ifndef OUTCONF_H
#define OUTCONF_H

/*
 * Library with helper functions to manage the output configuration.
 * 
 * Features:
 *  - Data structure for storing a display configuration
 *  - Data structure for storing a output configuration
 */

#include <stdint.h>
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "dumbo.h"

/*
 * Stores the configuration for the output of a buffer
 * to a single display (such as which controller and port,
 * resolution and refresh rate should be used).
 */
struct display_conf {
    uint32_t crtc_id;
    uint32_t connector_id;
    drmModeModeInfo mode;
};

/*
 * Stores the output configuration of the whole machine,
 * such as 
 */
struct out_conf {
    // This limited the output to 16 displays to avoid
    // dynamic memory allocations
    struct display_conf displays[16];
};

/* 
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * @param buf_id - Id of the buffer to be used for scanout.
 *
 * @return error - Returns error code on error, zero on success.
 * 
 * Sets the buffer buf_id as the scanout buffer, with the conf
 * display configuration (combination of controller and output port).
 */
int set_buf_id(int fd, struct display_conf conf, uint32_t buf_id);

/* 
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * @param buf - Buffer to be used for scanout.
 *
 * @return error - Returns error code on error, zero on success.
 * 
 * Sets the buffer buf as the scanout buffer, with the conf
 * display configuration (combination of controller and output port).
 */
int set_buf(int fd, struct display_conf conf, struct dumb_buf buf);

/*
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * 
 * @return display_conf - Returns valid display_conf.
 * 
 * Finds the first valid display configuration possible and returns
 * it. This function is mean to quickly set up a display to show
 * something, either for debugging or testing or in situations
 * where a simple configuration is desirable (boot time).
 */
struct display_conf find_basic_conf(int fd);

#endif