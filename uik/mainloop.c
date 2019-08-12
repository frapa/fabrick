#include "uik.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "sock/client.h"
#include "protosock/protosock.h"

static void _process_window_todo(int fd) {
    struct _uik_win_todo_item* item;
    struct _uik_win_todo_item* tmp;

    HASH_ITER(hh, _uik_window_todo_list, item, tmp) {
        struct uik_win* win;
        uik_get_obj(item->win_id, (void**)&win);

        switch (item->type) {
            case CREATE_WINDOW:
                win->_size = create_buffer(
                    fd, 
                    win->width, win->height,
                    win->x, win->y,
                    &win->_p, win->_buf_name
                );
                if (win->_size < 0) {
                    printf("Error: cannot create window buffer, %m\n", errno);
                }
                break;
        }

        HASH_DEL(_uik_window_todo_list, item);
        free(item->win_id);
        free(item);
    }

}

int uik_mainloop() {
    int err;

    int fd = connect_to_named_socket("fabrick");
    if (fd < 0) {
        printf("Error: failed to connect to display server, %m\n", errno);
        return fd;
    }

    while (1) {
        // get events

        // process windows items (create, close, resize windows)
        _process_window_todo(fd);

        // draw
    }

    err = close(fd);
    if (err) {
        return err;
    }
}