/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e78e0_slot0f(Object *obj);
void func_800e7940_slot0f(Object *obj);
extern void (*data_800f01bc_slot0f[])(Object *);

void func_800e75ec_slot0f(Object *obj) {
    s16 t;
    int a;
    int b;
    if (game_state.field_2bd == 0) {
        a = game_state.field_2bc;
        b = 3;
    } else {
        a = game_state.field_2bc;
        b = 5;
    }
    if (a == b) {
        t = obj->field_46;
        t -= 1;
        obj->field_46 = t;
        if (t != 0) {
            if ((t & 3) == 0) {
                func_800e7940_slot0f(obj);
                return;
            }
        } else {
            obj->field_05 = obj->field_05 + 1;
            game_state.field_2bc = 4;
        }
        func_800e78e0_slot0f(obj);
    }
}

void func_800e768c_slot0f(Object *obj) {
    u8 *p = &game_state.field_2bc;
    s16 t;
    if (*p == 2) {
        t = obj->field_46;
        t -= 1;
        obj->field_46 = t;
        if (t != 0) {
            if ((t & 3) == 0) {
                func_800e7940_slot0f(obj);
                return;
            }
        } else {
            obj->field_05 = 2;
            *p = 0xb;
        }
        func_800e78e0_slot0f(obj);
    }
}

void func_800e7708_slot0f(Object *obj) {
    data_800f01bc_slot0f[obj->field_05](obj);
}
