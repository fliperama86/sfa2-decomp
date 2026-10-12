/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot05_06(Object *obj);

void start_sequence_by_active_state_slot05_06(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void spawn_effects_on_status_flags_slot05_06(Object *obj) {
    s16 a;
    int b;

    if (((Slot04aObj *)obj)->status_flags != 0) {
        obj->field_165 = 0xff;
        ((Slot04aObj *)obj)->timer = 0x1e;
        ((Slot04aObj *)obj)->flag_bits = 1;
        obj->state_3++;
        if (obj->active_state == 0) {
            a = -8;
            b = 0x51;
        } else {
            a = -0xa;
            b = 0x6a;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot05_06(obj);
    }
    update_sequence(obj);
}

void count_down_timer_then_advance_state_3_slot05_06(Object *obj) {
    int a;
    ((Slot04aObj *)obj)->timer--;
    if (((Slot04aObj *)obj)->timer == 0) {
        ((Slot04aObj *)obj)->timer = 0x14;
        obj->state_3++;
        obj->hit_count_3 = 0;
        ((Slot04aObj *)obj)->field_27d = 0;
        if (obj->field_4b == 0) {
            a = ((Slot04aObj *)obj)->base_offset + 0x1e;
        } else {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}
