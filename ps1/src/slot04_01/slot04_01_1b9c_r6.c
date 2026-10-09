/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138b38(GameState *state, Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);

void func_801483a4(Object *object, int a_arg, int b_arg);

void func_801b23c8_slot04_01(Object *o) {
    *(s32 *)&o->field_4c = 0x80000;
    o->field_54 = -0x8000;
    o->field_07++;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_801204f4(o, o->side, 0xd);
    func_801307e0(o, (o->field_12a >> 1) + 0x28);
}

void func_801b2448_slot04_01(Object *o) {
    if ((u8)o->field_3a != 0) {
        o->field_07++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x18, 0x1c);
    }
    func_80130efc(o);
}
