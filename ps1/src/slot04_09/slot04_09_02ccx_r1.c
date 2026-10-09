/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c7b94_slot04_09[];

void func_80130678(Object *object, int arg);
u8 func_80125734(Object *object, int a);

void func_801b02cc_slot04_09(Object *obj) {
    s16 t;
    int k;

    if (game_state.field_64 == 0) {
        if (game_state.field_5c == 0) {
            obj->field_46 = 0x3c;
            obj->field_06++;
            game_state.field_76 = 0x1e;
            t = obj->field_5c;
            if (t < 0x1c) {
                k = 0;
            } else if (t < 0x38) {
                k = 1;
            } else if (t < 0x54) {
                k = 2;
            } else if (t < 0x70) {
                k = 3;
            } else {
                k = 4;
                if (t != 0x90) {
                    k = 5;
                }
            }
            k = (s8)func_80125734(obj, k);
            obj->field_48 = k;
            func_80130678(obj, data_801c7b94_slot04_09[k]);
        }
    } else {
        func_80130efc(obj);
    }
}
