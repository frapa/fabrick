#include "handlers.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/random.h>
#include <fcntl.h>

static uint16_t buf_count = 0;

static char* generate_new_buf_name() {
    char buf_name[18];
    
    buf_name[0] = '/';
    getrandom(&buf_name + 1, 16, );
    buf_name[17] = 0;

    return buf_name;
}

int handler_create_buffer(struct client* client, struct req_create_buffer* req) {
    // TODO: check request parameters (size) against malicius values
    // (dimesions too large or bigger than screen)
    // For now we just randomly check something
    if (req->width > 3840) return 1;
    if (req->height > 2160) return 1;
    
    // Generate next_buf_name
    char* buf_name = generate_new_buf_name();

    // Create virtual file
    int fd = shm_open(
        buf_name,
        O_RDWR | O_CREAT, // create if it does not exists
        S_IRWXU | S_IROTH | S_IWOTH // RW permission to others
    );
    if (fd < 0) {
        return fd;
    }

    // Grow file to the required buffer size
    uint32_t size = req->width * req->height * (req->alpha ? 4 : 3);
}