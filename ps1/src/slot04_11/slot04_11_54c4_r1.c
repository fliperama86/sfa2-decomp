/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7838_slot04_11[];

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_position_from_other_box(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_sequence_and_tables_from_direction(Object *object);

void func_801b5874_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

void update_sequence_or_load_velocity_table_2_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b5508_slot04_11(Object *obj) {
    data_801c7838_slot04_11[obj->state_3](obj);
}

void start_sequence_1b_set_facing_position_slot04_11(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 8);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (func_801b5914_slot04_11(obj)->pos_x + 0xc0 >= obj->pos_x) {
            goto inc;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
inc:
        obj->facing_side++;
    }
    set_sequence_by_side_and_mode(obj, 0x1b);
    set_position_from_other_box(obj);
}

void update_sequence_with_flag_hit_and_sound_slot04_11(Object *obj) {
    ref_other.p = obj->other;
    if (obj->step_flags & 4) {
        obj->state_3++;
        set_other_hit_reaction_state_0(obj, 8, 5, 0xf, 0, 0, 1);
    } else if (obj->step_flags & 2) {
        obj->step_flags = obj->step_flags & 0xfffd;
        spawn_effect_at_object_offset(obj, obj->direction_index >> 1, -0x59, 0x1e);
        play_sound_at(obj, obj->side ^ 1, 0x30d);
        play_side_sound_at(obj, obj->side, 6);
    }
    update_sequence(obj);
    if (obj->step_flags & 1) {
        set_position_from_other_box(obj);
    }
}

void update_sequence_or_load_velocity_table_3_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b5740_slot04_11(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
