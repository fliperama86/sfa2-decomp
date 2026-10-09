/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
extern u8 data_801c43dc_slot04_04[];

void func_801b2b8c_slot04_04(Object *o) {
    o->field_07++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    func_801307e0(o, 0x59);
}

void func_801b2be8_slot04_04(Object *o) {
    func_80130efc(o);
    if (*(u8 *)&o->field_3a != 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        func_801204f4(o, o->side, 8);
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x1a, 0x33);
    }
}

void func_801b2c74_slot04_04(Object *o) {
    u8 t;

    func_80130efc(o);
    if (*(u8 *)&o->field_3a == 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            t = 3;
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            t = o->field_12a >> 1;
        }
        ((Slot04bObj *)o)->field_27b = data_801c43dc_slot04_04[t];
        o->field_165 = 0;
        func_801204f4(o, o->side, 9);
    }
}
