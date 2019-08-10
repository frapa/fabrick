#include "uik.h"

#include <string.h>
#include <stdlib.h>
#include "ut/uthash.h"

struct registry_object {
	UT_hash_handle hh;
    char* id;
    void* obj;
};

struct registry_object* registry;

void uik_add_obj(char* id, void* obj) {
    struct registry_object* reg_obj = malloc(sizeof(struct registry_object));

    reg_obj->obj = obj;
    reg_obj->id = malloc(strlen(id)+1);
    strcpy(reg_obj->id, id);

    HASH_ADD_STR(registry, id, reg_obj);
}

void uik_del_obj(char* id) {
    struct registry_object* reg_obj;

    HASH_FIND_STR(registry, id, reg_obj);
    HASH_DEL(registry, reg_obj);

    free(reg_obj->id);
    free(reg_obj);
}

void uik_get_obj(char* id, void** obj) {
    struct registry_object* reg_obj;
    HASH_FIND_STR(registry, id, reg_obj);
    *obj = reg_obj->obj;
}