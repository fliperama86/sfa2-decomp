/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void update_airborne_spawn_child_or_land_slot04_02(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 3, 2);
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 1) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 2;
                p->table_index = 2;
                p->active_flag_2 = 0;
                p->health = 0;
                p->sound_side = obj->sound_side;
                p->team_id = obj->team_id;
                p->vel_table_index = (obj->direction_index >> 1) + 3;
                p->facing_side = obj->facing_side;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->image_row = obj->image_row;
                p->scroll_factor = obj->scroll_factor;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = obj->ground_y;
                p->spawn_x = 0x60;
                p->parent = obj;
                p->spawn_y = 0x1e0;
                p->ground_y = t;
                p->field_90 = obj->field_90;
                p->data_ptr_a = obj->data_ptr_a;
                p->data_ptr_b = obj->data_ptr_b;
                obj->linked_object_3 = (s32)p;
                obj->accel_y = -0x2800;
                obj->link_flags++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}
