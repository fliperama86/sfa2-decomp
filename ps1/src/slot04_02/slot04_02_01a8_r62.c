/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6e2c_slot04_02(Object *o) {
    Object *p = o->parent;
    if (p->linked_object == (u32)o) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}

void func_801b6e68_slot04_02(Object *o) {
    if (o->parent->side == 0) {
        func_80130768(o, o->variant_index, data_1f8000b4);
    } else {
        func_80130768(o, o->variant_index, data_1f800164);
    }
}

void func_801b6ec0_slot04_02(Object *o) {
    Object *p = o->parent;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->facing_side = 0;
    if (p->facing_side != 0) {
        o->pos_x = o->pos_x - 5;
    }
}
