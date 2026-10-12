/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3550_slot04_0b(Object *obj);

void start_gauge_attack_sequence_slot04_0b(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->gauge = 0x64;
    obj->countdown = 0;
    obj->offset_index = 0;
    set_sequence_by_side_and_mode(obj, 0x18);
}

void update_sequence_until_flag_2_slot04_0b(Object *obj) {
    if ((u8)obj->step_flags != 2) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801b3550_slot04_0b(obj);
    }
}
