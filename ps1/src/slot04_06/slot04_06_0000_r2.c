/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b00f8_slot04_06(Object *obj) {
    Object *c;
    u8 d;

    if (*((u8 *)&obj->step_flags + 1) & 0x80) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        if (*(u8 *)&obj->step_flags != 0) {
            *(u8 *)&obj->step_flags = 0;
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->active_count = 1;
                c->table_index = 4;
                c->parent = obj;
                c->field_1c = obj->field_1c;
                c->spawn_x = obj->spawn_x;
                c->spawn_y = obj->spawn_y;
                d = obj->image_row;
                c->field_02 = 0x11;
                c->mode_id = 0x20;
                c->image_row = d;
                c->field_90 = obj->field_90;
                c->sound_side = obj->sound_side;
                c->data_ptr_a = obj->data_ptr_a;
                c->data_ptr_b = obj->data_ptr_b;
            }
        }
        update_sequence(obj);
    }
}
