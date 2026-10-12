/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd4c8_slot05_06[];
extern u8 data_801dd4d0_slot05_06[];
extern ObjectFn data_801dd4d8_slot05_06[];

void start_sequence_2d_with_table_step_timer_slot05_06(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    obj->field_29c = 3;
    add_to_meter_clamped(obj, 4);
    set_sequence_by_side_and_mode(obj, 0x2d);
    obj->step_timer = *(u16 *)(data_801dd4c8_slot05_06 + (obj->direction_index & 0xfe));
}

void func_801cc354_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->mode != 0) {
        ((Slot04aObj *)obj)->mode = 0;
        obj->step_timer = *(u16 *)(data_801dd4d0_slot05_06 + (obj->direction_index & 0xfe));
    }
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 1);
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x49);
    }
}

void update_sequence_or_end_clear_flag_4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801cc438_slot05_06(Object *obj) {
    data_801dd4d8_slot05_06[obj->state_3](obj);
}
