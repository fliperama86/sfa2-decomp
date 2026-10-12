/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6218_slot04_02[];
extern ObjectFn data_801c6258_slot04_02[];

int is_active_state_set_slot04_02(Object *obj) {
    return obj->active_state != 0;
}

int is_airborne_without_link_slot04_02(Object *obj) {
    int r = 0;
    int t;

    if (obj->link_flags == 0) {
        if (obj->active_state != 0) {
            t = (u16)obj->ground_y;
            t -= 0x30;
            r = obj->pos_y < (s16)t;
        }
    }
    return r;
}

int has_meter_30_and_no_link_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        r = obj->link_flags == 0;
    }
    return r;
}

int func_801b1804_slot04_02(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_meter_30_active_and_no_link_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->active_state != 0) {
            r = obj->link_flags == 0;
        }
    }
    return r;
}

int func_801b1854_slot04_02(Object *obj) {
    return (s16)obj->meter >= 0x90;
}

int has_remaining_count_slot04_02(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1874_slot04_02(Object *obj) {
    data_801c6218_slot04_02[obj->field_15a](obj);
}

void func_801b18b4_slot04_02(Object *obj) {
    data_801c6258_slot04_02[obj->field_15a](obj);
}
