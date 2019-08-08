#ifndef DUMBO_H
#define DUMBO_H

/*
 * Library with simple API to manage dumb buffers.
 * 
 * Features:
 *  - Create and map dumb buffers with one simple function call
 */

#include <stdint.h>

/*
 * Struct to old a buffer pointer and some related information
 * like size and buffer id for buffer flipping.
 */
struct dumb_buf {
    // set by requesting code
    uint16_t width;
    uint16_t height;
    // set by library
    uint32_t id;
    uint64_t size;
    uint32_t* fb; // framebuffer (pointer to memory)
};

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
int create_buffer(int fd, struct dumb_buf* buf);

#endif