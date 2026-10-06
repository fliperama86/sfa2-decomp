/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 table_8017ab6c[];
int func_8013f04c(u8 *p, s16 mask, int value, s16 limit, u16 bits);

void func_8013ef28(Object *object, int index, int arg) {
    Slot *slot = &object->slots[(u8)index];
    if ((u8)func_8013f04c(&slot->field_01, table_8017ab6c[(u8)arg * 7] & 0xc0, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 1], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 2], object->field_134) == 0
        && (u8)func_8013f04c(&slot->field_01 + 2, table_8017ab6c[(u8)arg * 7] & 0x30, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 3], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 4], object->field_134) == 0
        && (u8)func_8013f04c(&slot->field_01 + 4, table_8017ab6c[(u8)arg * 7] & 0xc, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 5], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 6], object->field_134) == 0) {
        func_8013f2c8(object);
    } else {
        data_80188f44 = 1;
    }
}
