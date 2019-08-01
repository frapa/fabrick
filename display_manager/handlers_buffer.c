#include "handlers.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static uint16_t buf_count = 0;

static char* generate_new_buf_name() {
    char* buf_name = "/fabric_bf_0000";
    
    uint32_t r = buf_count;
    for (uint8_t i = 0; i < 4; ++i) {
        // 2 for the null char
        buf_name[sizeof(buf_name)-2-i] = r % 16 + '0'; 
        r /= 16;
    }

    ++buf_count;

    return buf_name;
}

int handler_create_buffer(struct client* client, struct req_create_buffer* req) {
    // Generate next_buf_name
    char* buf_name = generate_new_buf_name();

    // int err = shm_open(buf_name, O_RDWR | O_CREAT, mode_t mode);
}