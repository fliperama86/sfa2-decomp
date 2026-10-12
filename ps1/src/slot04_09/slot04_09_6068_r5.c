/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7e24_slot04_09[];

void unlink_from_parent_and_release_object_slot04_09(Object *o, Object *p) {
    if ((s32)o == *(s32 *)&p->linked_object_2) {
        *(s32 *)&p->linked_object_2 = 0;
    }
    func_8011f38c(o);
}

void set_position_from_parent_table_offset_slot04_09(Object *obj, Object *parent, int i) {
    u16 x; int t;
    obj->facing_side = parent->facing_side;
    if (obj->facing_side != 0) {
        t = data_801c7e24_slot04_09[i * 2] - 0x20;
        x = parent->pos_x;
        x += t;
        obj->pos_x = x;
    } else {
        t = data_801c7e24_slot04_09[i * 2] - 0x20;
        x = parent->pos_x;
        x -= t;
        obj->pos_x = x;
    }
    t = data_801c7e24_slot04_09[i * 2 + 1] - 0x20;
    x = parent->pos_y;
    x -= t;
    obj->pos_y = x;
}
