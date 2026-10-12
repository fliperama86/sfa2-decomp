/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b28_slot04_0f[];
extern ObjectFn data_801c5b34_slot04_0f[];

void spawn_offset_effect_and_play_sounds(Object *object);

void call_table_function_by_state_3_11_slot04_0f(Object *obj) {
    data_801c5b28_slot04_0f[obj->state_3](obj);
}

void call_effect_on_step_flag_byte_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void set_sequence_49_on_negative_step_flags_slot04_0f(Object *obj) {
    u16 t = obj->step_flags;
    if ((t << 16) < 0) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x49);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_finish_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_12_slot04_0f(Object *obj) {
    data_801c5b34_slot04_0f[obj->state_3](obj);
}
