/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void spawn_three_children_and_land_at_ground_slot04_07(Object *obj) {
    u16 t = obj->step_flags;
    int i;
    Object *c;

    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        for (i = 0; i < 3; i++) {
            c = (Object *)pop_block_from_pool();
            if (c != 0) {
                c->active_count = 1;
                c->field_02 = 7;
                c->other = obj;
                c->table_index = i;
                c->spawn_x = obj->spawn_x;
                c->spawn_y = obj->spawn_y;
                c->image_row = obj->image_row;
                c->mode_id = 0x20;
                c->sound_side = obj->sound_side;
                c->field_90 = obj->field_90;
                c->data_ptr_a = obj->data_ptr_a;
                c->data_ptr_b = obj->data_ptr_b;
            }
        }
    }
    func_801b41c4_slot04_07(obj);
    if (obj->vel_y >= 0 || (i = obj->ground_y, obj->pos_y < i)) {
        update_sequence(obj);
    } else {
        obj->state_2 += 1;
        obj->pos_y = i;
        if (obj->side == 0) {
            obj->pos_x = 0x220;
        } else {
            obj->pos_x = 0x2e0;
        }
        obj->vel_x = 0;
        obj->vel_y = 0;
        obj->accel_x = 0;
        obj->accel_y = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
    }
}
