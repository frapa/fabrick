#include "uik.h"

struct uik_flex* uik_flex_new(char* name, uint8_t orientation) {
    struct uik_flex* flex = malloc(sizeof(struct uik_flex));

    flex->widget.class = _UIK_WIDGET_FLEX;
    flex->widget.parent = NULL;
    flex->widget.children = NULL;

    flex->orientation = orientation;

    uik_add_obj(name, flex);

    return flex;
}

struct geometry _uik_layout(struct uik_widget* widget, struct constrains* constrains) {
    switch (widget->class) {
        case _UIK_WIDGET_WINDOW:
            return _uik_window_layout(widget, constrains);
            break;
        case _UIK_WIDGET_FLEX:
            return _uik_flex_layout(widget, constrains);
            break;
    }
}