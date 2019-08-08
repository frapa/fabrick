#ifndef DM_STATE
#define DM_STATE

#include <stdint.h>

#include "dumbo/outconf.h"

struct state {
    int fb_fd;
    struct display_conf display_conf;
    uint8_t dirty;
};

extern struct state state;

#endif