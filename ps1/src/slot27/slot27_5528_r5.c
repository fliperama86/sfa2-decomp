/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015950_slot27(Object *obj) {
    Object *p;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xa9;
        p->table_index = 2;
        p->pos_x = obj->pos_x + 0x100;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xa9;
        p->table_index = 3;
        p->pos_x = obj->pos_x + 0x200;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xa9;
        p->table_index = 4;
        p->pos_x = obj->pos_x + 0x300;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
}

extern void (*data_80028aa0_slot27[])(Object *obj);
extern void (*data_80028aac_slot27[])(Object *obj);

void func_80015a3c_slot27(Object *obj) {
    if (obj->table_index & 0x80) {
        data_80028aac_slot27[obj->state_0](obj);
    } else {
        data_80028aa0_slot27[obj->state_0](obj);
    }
}
