/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b58dc_slot04_09(Object *obj);

void update_hit_sequence_spawn_child_on_flag_slot04_09(Object *obj) {
    u16 t;
    Object *p;
    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->state_3++;
        obj->field_165 = 0;
        obj->other_lock = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->other_lock = 0xe;
        }
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x16;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = 2;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->scroll_factor = obj->scroll_factor;
            *(u32 *)&p->field_a4 = 0x1000800;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            func_801b58dc_slot04_09(obj);
        }
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x31c);
        spawn_children_per_direction_index(obj, 0, 0x54);
    }
}
