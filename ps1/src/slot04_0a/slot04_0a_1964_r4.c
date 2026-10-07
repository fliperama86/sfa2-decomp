/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c00_slot04_0a(Object *obj);
u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b2048_slot04_0a(Object *obj);
void func_80138ae8(GameState *state, Object *object);
void func_80141f28(Object *object, short delta);

void func_801b1ce8_slot04_0a(Object *obj) {
    u16 t;
    int k;

    if (obj->field_50 < 0) {
        t = obj->field_c2;
        if (t & 0x80) {
            k = 0;
        } else if (t & 0x10) {
            k = 1;
        } else if (t & 4) {
            k = 2;
        } else if (t & 1) {
            k = 0;
        } else {
            goto back;
        }
        obj->field_07++;
        func_801307e0(obj, k | 0x28);
        obj->field_50 = 0xfffd8000;
        obj->field_58 = -0x2000;
        func_80141f28(obj, 2);
        func_80138ae8(&game_state, obj);
        func_801204f4(obj, obj->side, 9);
        func_801b2048_slot04_0a(obj);
        if (func_801b1e04_slot04_0a(obj)) {
            func_801b1c00_slot04_0a(obj);
        }
    } else {
back:
        func_801b2048_slot04_0a(obj);
        if (func_801b1e04_slot04_0a(obj)) {
            func_801b1c00_slot04_0a(obj);
        }
        func_80130efc(obj);
    }
}

u16 func_801b1e04_slot04_0a(Object *obj) {
    u16 r = 0;

    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        r = obj->pos_y > obj->field_70;
    }
    return r;
}
