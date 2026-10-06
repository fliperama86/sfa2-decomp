/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984;
extern u16 data_801a6966;
void func_8013f2a8(Object *object, u8 index, u8 arg);
void func_8013f1bc(Object *object, u8 index, u8 arg, u16 entry);

/* buttons is not set on every path: when data_801a6984 is not zero and the
   second condition does not hold, the code tests the register as it is. A
   fourth parameter in place of the local was tried and gives other code. The
   call to func_8013f1bc is written with a fourth argument, entry, because the
   original has that value in the fourth argument register at the call; the
   definition of func_8013f1bc in this tree takes two parameters. */
void func_8013d520(Object *object, u8 index, u8 arg) {
    u16 entry;
    u8 t0;
    int buttons;
    if (data_801a6984 == 0) {
        if (object->side == 0) {
            buttons = data_801a6966 & 0xf000;
        } else {
            buttons = data_801a6972 & 0xf000;
        }
    }
    if (game_state.field_30 != 0 && data_801a6984 != 0 && game_state.mode == object->field_02 + 1) {
        buttons = object->field_c2 & 0xf000;
    }
    if ((u16)buttons == 0) {
        func_8013f2a8(object, (u8)index, (u8)arg);
    } else {
        if ((u16)buttons == 0x1000) {
            t0 = 0;
        } else if ((u16)buttons == 0x8000) {
            t0 = 1;
        } else if ((u16)buttons == 0x4000) {
            t0 = 2;
        } else if ((u16)buttons == 0x2000) {
            t0 = 3;
        } else {
            func_8013f2a8(object, (u8)index, (u8)arg);
            return;
        }
        object->slots[(u8)index].field_00 = object->slots[(u8)index].field_00 + 1;
        entry = table_8017a8cc[(u8)arg * 7];
        object->slots[(u8)index].field_01 = t0;
        *(u8 *)&object->slots[(u8)index].field_02 = entry;
        func_8013f1bc(object, (u8)index, (u8)arg, entry);
    }
}
