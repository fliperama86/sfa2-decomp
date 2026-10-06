/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800284d8_slot12[])(Object *);
void func_80015850_slot12(Object *obj);
void func_800158b0_slot12(Object *obj);
void func_80015fd4_slot12(Object *obj, int a);

void func_800154f8_slot12(Object *obj) {
    u8 t = obj->field_05;
    obj->field_46 = 0x1e;
    obj->field_05 = t + 1;
    func_80015850_slot12(obj);
    func_80015fd4_slot12(obj, 0);
    game_state.field_2bc = 1;
}

void func_8001554c_slot12(Object *obj) {
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
                func_800158b0_slot12(obj);
                return;
            }
        } else {
            obj->field_05 = obj->field_05 + 1;
            game_state.field_2bc = 4;
        }
        func_80015850_slot12(obj);
    }
}

void func_800155ec_slot12(Object *obj) {
    data_800284d8_slot12[obj->field_05](obj);
}
