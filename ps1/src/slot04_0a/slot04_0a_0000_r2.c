/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b01ac_slot04_0a(Object *obj) {
    Object *c;

    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + 0xc000;
    if (obj->pos_y + 0x20 >= obj->ground_y) {
        obj->state_2 = obj->state_2 + 1;
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->field_00 = c->field_00 + 1;
            c->field_02 = 0xd;
            c->field_03 = 0;
            c->field_3c = obj;
            c->spawn_x = obj->spawn_x;
            c->spawn_y = obj->spawn_y;
            c->field_0d = obj->field_0d;
            c->field_08 = 0x20;
            c->field_66 = obj->field_66;
            c->field_90 = obj->field_90;
            c->field_98 = obj->field_98;
            c->field_9c = obj->field_9c;
        }
    }
    update_sequence(obj);
}

void func_801b0290_slot04_0a(Object *obj) {
    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + 0xc000;
    if (obj->pos_y + 0x10 >= obj->ground_y) {
        obj->state_2 = obj->state_2 + 1;
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
        set_sequence_by_side(obj, 0x31);
    } else {
        update_sequence(obj);
    }
}
