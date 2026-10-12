/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void copy_string_with_suffix_by_mode_slot0f(u8 *src, u8 *dst, int mode) {
    while (*src != 0) {
        *dst++ = *src++;
    }
    switch (mode) {
    case 0:
        break;
    case 1:
        *dst++ = 'P';
        *dst++ = 'T';
        *dst++ = 'S';
        break;
    case 2:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        break;
    case 3:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        *dst++ = 'S';
        break;
    }
    *dst++ = '@';
    *dst++ = '@';
    *dst = 0;
}

void spawn_object_at_spawn_position_slot0f(Object *obj, int arg) {
    Object *b;
    if (ptr_8019040c->field_0a != 0) {
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x97;
            b->variant_index = 2;
            b->ot_depth = 0;
            b->pos_x = obj->spawn_x;
            b->pos_y = obj->spawn_y;
            b->table_index = 0;
            b->health = obj->health;
        }
    }
}
