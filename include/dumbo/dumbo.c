#include "dumbo.h"

/*
 * Library with simple API to manage dumb buffers.
 * 
 * Features:
 *  - Create and map dumb buffers with one simple function call
 */

#include <sys/mman.h>
#include "xf86drm.h"
#include "xf86drmMode.h"

/* 
 * @param fd  - File descriptor of a drm device (usually /dev/dri/card0).
 * @param buf - Pointer to a dumb_buf struct that contains the requested
 *              width and height of the buffer. The rest of the data will
 *              be filled by the function.
 *
 * @return error - Returns error code on error, zero on success.
 * 
 * This functions creates a dumb buffer with the specified width and height.
 * The buffer has always 32 bits per pixel but color depth of 24
 * (the first 8 bits are unused). The fb property of the dumb_buf object
 * is a pointer to the memory and can be used for writing pixels.
 */
int create_buffer(int fd, struct dumb_buf* buf) {
    int err;

    // STEP 1: Request creation fo the buffer
    // --------------------------------------
    struct drm_mode_create_dumb create_req;

    create_req.width = buf->width;
    create_req.height = buf->height;
    create_req.bpp = 32;
    create_req.flags = 0; // man page says it must be set to zero

    err = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_req);
    if (err) {
        return err;
    }

    // save buffer size
    buf->size = create_req.size;

    // STEP 2: Register buffer and get buffer id
    // -----------------------------------------
    err = drmModeAddFB(fd, 
        buf->width, buf->height,
        // color depth and bit per pixel
        24, create_req.bpp,
        create_req.pitch,
        // specifies which buffer it is
        create_req.handle,
        // the buffer id will be saved here
        &buf->id
    );
    if (err) {
        return err;
    }

    // STEP 3: Prepare buffers for memory mapping
    // ------------------------------------------
    struct drm_mode_map_dumb map_req;

    map_req.handle = create_req.handle;
    map_req.pad = 0; // man page says it must be set to zero

    err = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map_req);
    if (err) {
        return err;
    }

    // STEP 4: Memory Map the buffer and get pointer for drawing
    // ---------------------------------------------------------
    uint8_t* map = (uint8_t*)mmap(0, buf->size,
        PROT_READ | PROT_WRITE, MAP_SHARED,
        fd, map_req.offset);
    if (map == MAP_FAILED) {
        return -1;
    }

    // Save the buffer pointer.
    // We chose to convert it to a uint32_t pointer
    // so that each element represents a pixel.
    // Writing color values can be done easily with
    // the binay shift operator:
    //
    //     buf->fb[i] = (r << 16) | (g << 8) | b
    //
    // where r, g, b are the uint8_t component values
    buf->fb = (uint32_t*)map;

    return 0;
}