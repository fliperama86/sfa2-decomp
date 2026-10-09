/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138b38(GameState *state, Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);

extern s32 data_801befe8_slot04_01[];

void func_801b2108_slot04_01(Object *object);

void func_801b1fa8_slot04_01(Object *o) {
    int a;

    o->field_07++;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    a = o->field_12a >> 1;
    o->field_58 = -0x6000;
    o->field_50 = data_801befe8_slot04_01[o->field_12a >> 1];
    func_801307e0(o, a + 0x38);
}

void func_801b2034_slot04_01(Object *o) {
    func_80130efc(o);
    if ((u8)o->field_3a != 0) {
        o->field_07++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        func_801b2108_slot04_01(o);
    }
}

void func_801b2098_slot04_01(Object *o) {
    func_80130efc(o);
    if ((s16)o->field_3a & 0x8000) {
        o->field_45 = 1;
        o->field_07++;
        func_801307e0(o, (o->field_12a >> 1) + 0x3b);
    } else {
        func_801b2108_slot04_01(o);
    }
}
