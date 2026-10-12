/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c24fc_slot04_0b[];

void func_801b14ac_slot04_0b(Object *obj) {
    data_801ad398 = data_801c24fc_slot04_0b[obj->field_15a](obj);
}

int has_remaining_count_slot04_0b(Object *obj) {
    return obj->remaining_count != 0;
}

int is_meter_at_least_threshold_slot04_0b(Object *obj) {
    return (s16)obj->meter >= 0x30;
}
