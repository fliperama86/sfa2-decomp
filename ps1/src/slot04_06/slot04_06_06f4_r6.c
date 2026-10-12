/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c52b8_slot04_06[];

int has_remaining_count_slot04_06(Object *obj) {
    return obj->remaining_count != 0;
}

int is_meter_at_least_threshold_3_slot04_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

void func_801b13d8_slot04_06(Object *obj) {
    data_801c52b8_slot04_06[obj->field_15a](obj);
}
