#ifndef UIK_MAINLOOP_H
#define UIK_MAINLOOP_H

#include "ut/uthash.h"
#include "protocol/protocol.h"

// Mainloop
int uik_mainloop();

// Object registry
void uik_add_obj(char* id, void* obj);
void uik_del_obj(char* id);
void uik_get_obj(char* id, void** obj);

// Widget
struct uik_widget {

};

// Windows
// private
#define _UIK_WIN_TODO_CREATE_WINDOW 1
#define _UIK_WIN_TODO_RESIZE_WINDOW 2
#define _UIK_WIN_TODO_MOVE_WINDOW 3
#define _UIK_WIN_TODO_CLOSE_WINDOW 4

struct _uik_window_todo_item {
	UT_hash_handle hh;
    uint8_t type;
    char* win_id;
};
extern struct _uik_window_todo_item* _uik_window_todo_list;

// public
struct uik_window {
    uint8_t type;
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
    // private
    struct uik_widget _widget;
    char _buf_name[BUF_NAME_LENGTH];
    uint32_t* _p;
    uint32_t _size;
};

struct uik_window* uik_window_new(char* name, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

void uik_window_clear(struct uik_window* win, uint32_t color);

#endif