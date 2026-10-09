/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984;
extern u16 data_801a6966;

void func_8013d65c(Object *object, u8 index, u8 arg) {
    u16 buttons;
    s16 dir;

    object->slots[index].field_04--;
    if (object->slots[index].field_04 == 0) {
        func_8013f2a8(object, index, arg);
        return;
    }
    if (data_801a6984 == 0) {
        if (object->side == 0) {
            buttons = data_801a6966 & 0xf000;
        } else {
            buttons = data_801a6972 & 0xf000;
        }
    }
    if (game_state.field_30 != 0 && data_801a6984 != 0 &&
        game_state.mode == object->field_02 + 1) {
        buttons = object->field_c2 & 0xf000;
    }
    if (buttons != 0) {
        func_8013dce0(object, index, arg);
        if (buttons == data_80188f4c) {
            dir = 1;
            goto apply;
        }
        if (buttons == data_80188f50) {
            dir = -1;
            goto apply;
        }
    }
    func_8013f2c8(object);
    return;
apply:
    object->slots[index].field_00++;
    ((u8 *)&object->slots[index].field_02)[0]--;
    ((u8 *)&object->slots[index].field_02)[1] = dir;
    if (dir == -1) {
        object->slots[index].field_01--;
    } else {
        object->slots[index].field_01++;
    }
    object->slots[index].field_01 &= 3;
    func_8013f1bc(object, index, arg);
}
