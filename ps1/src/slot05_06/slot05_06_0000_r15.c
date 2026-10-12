/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801dd288_slot05_06[];
extern ObjectFn data_801dd2b4_slot05_06[];
extern ObjectFn data_801dd2e4_slot05_06[];

void func_801c92e0_slot05_06(Object *obj) {
    data_801ad398 = data_801dd288_slot05_06[obj->field_15a](obj);
}

int set_direction_index_2_return_true_slot05_06(Object *obj) {
    obj->direction_index = 2;
    return 1;
}

int is_meter_at_least_threshold_slot05_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int return_true_always_slot05_06(Object *obj) {
    return 1;
}

int is_meter_at_least_threshold_2_slot05_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int is_all_states_one_and_healthy_slot05_06(Object *obj) {
    if (*(u32 *)&obj->state_0 == 0x1010101) {
        if (obj->active_state == 0) {
            if (obj->field_163 == 0) {
                if (*(s16 *)&obj->health >= 0) goto yes;
            }
        }
    }
    return 0;
yes:
    return 1;
}

int has_remaining_count_slot05_06(Object *obj) {
    return obj->remaining_count != 0;
}

int is_meter_at_least_threshold_3_slot05_06(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

void func_801c93d8_slot05_06(Object *obj) {
    data_801dd2b4_slot05_06[obj->field_15a](obj);
}

void call_table_function_by_state_3_7_slot05_06(Object *obj) {
    data_801dd2e4_slot05_06[obj->state_3](obj);
}
