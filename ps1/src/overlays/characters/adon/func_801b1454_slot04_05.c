/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFnInt data_801c139c_slot04_05[];
extern ObjectFn data_801c13c4_slot04_05[];
extern ObjectFn data_801c13ec_slot04_05[];

void func_801b1454_slot04_05(Object *obj) {
    script_condition_result = data_801c139c_slot04_05[obj->field_15a](obj);
}

int return_true_always_slot04_05(Object *obj) {
    return 1;
}

int is_meter_at_least_threshold_slot04_05(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int is_meter_at_least_threshold_2_slot04_05(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_05(Object *obj) {
    return obj->remaining_count != 0;
}

int is_meter_at_least_threshold_3_slot04_05(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

void func_801b14ec_slot04_05(Object *obj) {
    data_801c13c4_slot04_05[obj->field_15a](obj);
}

void func_801b152c_slot04_05(Object *obj) {
    data_801c13ec_slot04_05[obj->field_15a](obj);
}
