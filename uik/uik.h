#ifndef UIK_MAINLOOP_H
#define UIK_MAINLOOP_H

#include "ut/uthash.h"
#include "ut/utlist.h"
#include "protocol/protocol.h"

// Mainloop
int uik_mainloop();

// Object registry
void uik_add_obj(char* id, void* obj);
void uik_del_obj(char* id);
void uik_get_obj(char* id, void** obj);

// Widget
struct uik_widget {
    uint8_t class;
    struct uik_widget* parent;
    struct uik_widget* children;
    struct uik_widget* prev;
    struct uik_widget* next;
};

// Windows
// private
#define _UIK_WIDGET_WINDOW 1

enum _uik_window_todo_type {
    CREATE_WINDOW,
    RESIZE_WINDOW,
    MOVE_WINDOW,
    CLOSE_WINDOW,
};

struct _uik_window_todo_item {
	UT_hash_handle hh;
    enum _uik_window_todo_type type;
    char* win_id;
};
extern struct _uik_window_todo_item* _uik_window_todo_list;

// public
struct uik_window {
    struct uik_widget widget;
    uint8_t type;
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
    // private
    char _buf_name[BUF_NAME_LENGTH];
    uint32_t* _p;
    uint32_t _size;
};

struct uik_window* uik_window_new(char* name, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

void uik_window_clear(struct uik_window* win, uint32_t color);

void uik_window_pack(struct uik_window* win, struct uik_widget* child);
struct geometry _uik_window_layout(struct uik_widget* widget, struct constrains* constrains);

// Layout
#define _UIK_WIDGET_FLEX 2

enum uik_flex_orientation {
    HORIZONTAL,
    VERTICAL,
};

struct uik_flex {
    struct uik_widget widget;
    uint8_t orientation;
};

struct uik_flex* uik_flex_new(uint8_t orientation);

struct geometry _uik_flex_layout(struct uik_widget* widget, struct constrains* constrains);

struct constrains {

};

struct geometry {

};

struct geometry _uik_layout(struct uik_widget* widget, struct constrains* constrains);

#endif