#ifndef DM_SHBUF
#define DM_SHBUF

#include <stdint.h>
#include "ut/uthash.h"

struct shbuf {
	UT_hash_handle hh;
    int fd;
    char file_name[18];
    uint16_t width;
    uint16_t height;
    uint32_t size;
    uint32_t* p;
    uint32_t* p_committed;
};

#endif