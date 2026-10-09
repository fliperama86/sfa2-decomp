/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
extern u8 data_801c4164_slot04_04[];
u8 func_80125734(Object *object, int a);
void func_80130678(Object *object, int index);

void func_801b0250_slot04_04(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0284_slot04_04(Object *obj) {
    if (game_state.field_64 == 0 || game_state.field_5c == 0) {
        obj->field_06++;
    }
    func_80130efc(obj);
}

void func_801b02d4_slot04_04(Object *obj) {
    s16 k;

    obj->field_46 = 0x3c;
    obj->field_06++;
    game_state.field_76 = 0x1e;
    k = 3;
    if ((s16)obj->field_5c != 0x90) {
        k = data_801c4164_slot04_04[func_80151184() & 0xf];
    }
    k = (s8)func_80125734(obj, (s8)k);
    k += 0x23;
    func_80130678(obj, k);
}

void func_801b0370_slot04_04(Object *obj) {
    s16 t = obj->field_46;
    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}
