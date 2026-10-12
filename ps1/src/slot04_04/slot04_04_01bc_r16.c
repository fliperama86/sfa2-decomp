/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c41a8_slot04_04[];
extern ObjectFn data_801c41d4_slot04_04[];

int return_true_always_slot04_04(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_04(Object *obj) {
    return obj->link_flags == 0;
}

int is_meter_at_least_threshold_slot04_04(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int is_meter_at_least_threshold_2_slot04_04(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int is_meter_at_least_threshold_3_slot04_04(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_04(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1610_slot04_04(Object *obj) {
    data_801c41a8_slot04_04[obj->field_15a](obj);
}

void func_801b1650_slot04_04(Object *obj) {
    data_801c41d4_slot04_04[obj->field_15a](obj);
}
