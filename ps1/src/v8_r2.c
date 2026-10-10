/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8013e84c(Object *object, int index, int arg) {
    u16 entry = table_8017aaf8[(u8)arg * 7 + object->slots[(u8)index].field_01];
    int mine = entry & -0x1000;
    int theirs = object->field_150 & -0x1000;

    if (!(entry & 0x400)) {
        if ((s16)mine == (s16)theirs) {
            goto ok;
        }
    } else if ((theirs & mine) != 0) {
        goto ok;
    }
    func_8013f2c8(object);
    return;
ok:
    if (!(entry & 1)) {
        func_8013f0c8(object, (u8)index, (u8)arg);
    } else {
        object->slots[(u8)index].field_04 = 0xb;
        object->slots[(u8)index].field_02 = 6;
        object->slots[(u8)index].field_00++;
        object->slots[(u8)index].field_01++;
    }
}
