/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_side_sound_from_step_flags_slot04_01(Object *obj);
extern ObjectFnInt data_801beed0_slot04_01[];
extern ObjectFn data_801beefc_slot04_01[];
extern ObjectFn data_801bef28_slot04_01[];
extern ObjectFn data_801bef54_slot04_01[];

void func_801b1070_slot04_01(Object *obj) {
    data_801ad398 = data_801beed0_slot04_01[obj->field_15a](obj);
}

int return_true_always_slot04_01(Object *obj) {
    return 1;
}

int is_linked_object_3_null_slot04_01(Object *obj) {
    return obj->linked_object_3 == 0;
}

int is_active_state_set_slot04_01(Object *obj) {
    return obj->active_state != 0;
}

int func_801b10d8_slot04_01(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_01(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b10f8_slot04_01(Object *obj) {
    data_801beefc_slot04_01[obj->field_15a](obj);
}

void func_801b1138_slot04_01(Object *obj) {
    data_801bef28_slot04_01[obj->field_15a](obj);
}

void func_801b1178_slot04_01(Object *obj) {
    data_801bef54_slot04_01[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_01(obj);
}
