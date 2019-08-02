#ifndef PROTOCOL
#define PROTOCOL

#include <stdint.h>

#define REQ_CREATE_BUFFER 1

#define RES_BAD_REQUEST  1
#define RES_SERVER_ERROR 2
#define RES_REGISTER 3

struct req_create_buffer {
    uint16_t width;
    uint16_t height;
    uint8_t alpha;
};

struct res_create_buffer {
    uint32_t buffer_name;
    
};

#endif