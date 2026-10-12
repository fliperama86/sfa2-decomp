/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c54cc_slot04_06[];
extern u8 data_801c54d4_slot04_06[];

void func_801b42d8_slot04_06(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    obj->field_29c = 3;
    func_80141f28(obj, 4);
    func_801307e0(obj, 0x2d);
    obj->step_timer = *(u16 *)(data_801c54cc_slot04_06 + (obj->direction_index & 0xfe));
}

void func_801b4358_slot04_06(Object *obj) {
    if (((Slot04aObj *)obj)->mode != 0) {
        ((Slot04aObj *)obj)->mode = 0;
        obj->step_timer = *(u16 *)(data_801c54d4_slot04_06 + (obj->direction_index & 0xfe));
    }
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 1);
        func_801307e0(obj, (obj->direction_index >> 1) + 0x49);
    }
}

void func_801b43fc_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        func_801312b8(obj);
    }
}
