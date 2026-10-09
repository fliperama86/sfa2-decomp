/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1598_slot04_05[];
extern u8 data_801c1590_slot04_05[];

void func_801b29e4_slot04_05(Object *o) {
    o->field_225 = 1;
    o->field_07++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    o->field_4c = data_801c1598_slot04_05[o->field_12a];
    o->field_54 = data_801c1598_slot04_05[o->field_12a + 1];
    func_801307e0(o, *(s16 *)((u8 *)data_801c1590_slot04_05 + (o->field_12a & 0xfe)));
}

void func_801b2a9c_slot04_05(Object *o) {
    if ((u8)o->field_3a != 0) {
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        o->field_07++;
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x10, 0x43);
    }
    func_80130efc(o);
}
