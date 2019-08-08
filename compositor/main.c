#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include "ut/uthash.h"

#include "dm.h"
#include "state.h"
#include "clients.h"

#include "fb.h"

pthread_t dm_thread;

int main() {
    int err;
    
    err = init_fb();
    if (err) {
        return 1;
    }

    err = pthread_create(&dm_thread, NULL, run_dm, NULL);
    if (err) {
        fprintf(stderr, "Error: Cannot create display manager thread, %s.\n",
            strerror(errno));
        return 1;
    }

    struct timespec sleep_time = { .tv_sec = 0, .tv_nsec = 10 * 1000 * 1000 };
    while (1) {
        if (state.dirty) {
            memset(dbuf->fb, 50, dbuf->size);

            struct client* client;
            for (client = clients; client != NULL; client = client->hh.next) {
                if (HASH_COUNT(client->buffers)) {
                    uint16_t sx = 64;
                    uint16_t sy = 48;
                    struct shbuf* client_buf = client->buffers;
                    for (int i = 0; i < client_buf->height; ++i) {
                        memcpy(
                            dbuf->fb + dbuf->width * (sy + i) + sx,
                            client_buf->p_committed + client_buf->width * i,
                            4 * client_buf->width
                        );
                    }
                }
            }

            flip();
            state.dirty = 0;
        }

        nanosleep(&sleep_time, NULL);
    }
}