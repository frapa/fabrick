#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uik/uik.h"

void main() {
    int err;

    // int fd = connect_to_named_socket("fabrick");
    // if (fd < 0) {
    //     perror("asd");
    //     return printf("Error: failed to connect\n");
    // }

    // uint32_t* buf_p = NULL;
    // char buffer_name[BUF_NAME_LENGTH];
    // uint32_t size = create_buffer(
    //     fd, 
    //     800, 600,
    //     50, 50,
    //     &buf_p, buffer_name
    // );
    // if (size < 0) {
    //     return printf("Error: cannot create buffer\n");
    // }
    
    // memset(buf_p, 255, size);
    // commit_buffer(fd, buffer_name);
    // if (size < 0) {
    //     return printf("Error: cannot commit buffer\n");
    // }
    
    // sleep(3);

    // memset(buf_p, 120, size);
    // commit_buffer(fd, buffer_name);
    // if (size < 0) {
    //     return printf("Error: cannot commit buffer\n");
    // }

    // sleep(10);

    // err = close(fd);
    // if (err) {
    //     return printf("Error: closing socket failed\n");
    // }

    uik_window_new("win1", 600, 400, 50, 50);

    err = uik_mainloop();
}