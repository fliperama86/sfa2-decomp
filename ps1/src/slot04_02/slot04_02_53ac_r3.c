/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot04_02(Object *object);

void start_state_spawn_child_set_sequence_slot04_02(Object *obj) {
    Object *p;
    int a;
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    } else {
        p = (Object *)pop_block_from_pool();
        a = 0x2b;
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x7d;
            p->parent = obj;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_0e = obj->field_0e;
            obj->parent = p;
            p->field_02 = 9;
            p->mode_id = 0x20;
            p->sound_side = obj->sound_side;
        }
    }
    set_sequence_by_side(obj, a);
}

void update_sequence_spawn_effects_on_step_flag_slot04_02(Object *obj) {
    int a;
    int b;
    if ((s16)obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = 0xd;
            b = 0x39;
        } else {
            a = 1;
            b = 0x47;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_02(obj);
    }
    update_sequence(obj);
}
