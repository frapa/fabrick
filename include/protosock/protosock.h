#ifndef PROTOSOCK_H
#define PROTOSOCK_H

#include "protocol/protocol.h"

int create_buffer(
    int fd,
    uint16_t width, uint16_t height,
    uint16_t x, uint16_t y,
    uint32_t** buf_p, char buffer_name[BUF_NAME_LENGTH]
);

int commit_buffer(int fd, char buffer_name[BUF_NAME_LENGTH]);

#endif