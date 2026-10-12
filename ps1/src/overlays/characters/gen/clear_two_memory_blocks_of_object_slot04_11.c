/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void add_to_meter_clamped(Object *object, short delta);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void clear_two_byte_blocks_of_object_slot04_11(Object *obj);
Object *get_table_object_by_field_slot04_11(Object *obj);

extern ObjectFn data_801c7804_slot04_11[];
extern ObjectFn data_801c780c_slot04_11[];
extern u8 data_801c7818_slot04_11[];

void clear_two_memory_blocks_of_object_slot04_11(Object *obj) {
    clear_two_byte_blocks_of_object_slot04_11(obj);
}

void call_handler_by_input_dir_second_slot04_11(Object *obj) {
    data_801c7804_slot04_11[obj->input_dir_second >> 1](obj);
}

void call_table_function_by_state_3_13_slot04_11(Object *obj) {
    data_801c780c_slot04_11[obj->state_3](obj);
}

void start_sequence_1a_set_facing_slot04_11(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 8);
    obj->facing_side = 0;
    if (!(obj->mode_flag_2 != 0 ? (s16)((u16)get_table_object_by_field_slot04_11(obj)->pos_x + 0xc0) < obj->pos_x : (obj->input_bits & 0x8000) != 0)) {
        obj->facing_side++;
    }
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void update_attack_sequence_with_table_hit_slot04_11(Object *obj) {
    int i;
    s16 c;
    s16 d;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[6];

    ref_other.p = obj->other;
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
    } else if (*(u8 *)&obj->step_flags != 0) {
        i = ((*(u8 *)&obj->step_flags + 0xff) << 4) & 0x3f0;
        obj->step_flags = 0;
        c = *(u16 *)(data_801c7818_slot04_11 + i + 8);
        d = *(u16 *)(data_801c7818_slot04_11 + i + 12);
        spawn_effect_at_object_offset(obj, obj->direction_index >> 1, *(s16 *)(data_801c7818_slot04_11 + i), *(s16 *)(data_801c7818_slot04_11 + i + 4));
        if (apply_damage_to_other_return_defeated(obj, c, d)) {
            play_sound_at(obj, obj->side, 0x336);
            obj->state_3++;
            set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
        } else {
            play_sound_at(obj, obj->side, 0x304);
            ref_other.p = obj->other;
            if (ref_other.p->field_15b == 0) {
                obj->state_3++;
                set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
            }
        }
    }
    update_sequence(obj);
}
