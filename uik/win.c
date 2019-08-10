#include "uik.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "ut/uthash.h"
#include "protocol/protocol.h"

struct _uik_window_todo_item* _uik_window_todo_list;

void uik_window_new(char* name, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
    struct uik_window* win = malloc(sizeof(struct uik_window));

    win->type = BUFFER_NORMAL;
    win->width = width;
    win->height = height;
    win->x = x;
    win->y = y;
    
    uik_add_obj(name, win);

    // Add todo list item, so that the mainloop
    // creates a window on the next loop
    struct _uik_window_todo_item* item = malloc(sizeof(struct _uik_window_todo_item));
    item->type = _UIK_WIN_TODO_CREATE_WINDOW;
    item->win_id = malloc(strlen(name)+1);
    strcpy(item->win_id, name);
    HASH_ADD_STR(_uik_window_todo_list, win_id, item);

    return win;
}

void uik_window_clear(struct uik_window* win, uint32_t color) {
    for (uint16_t y = 0; y < win->height; ++y) {
        for (uint16_t x = 0; x < win->width; ++x) {
            win->_p[y * win->width + x] = color;
        }
    }
}