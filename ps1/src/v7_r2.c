/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, int index, int unused);

void func_8013e710(Object *object, int index, int arg) {
    u16 entry = table_8017aaf8[(u8)arg * 7 + 1];
    int mine = entry & -0x1000;
    int theirs = object->field_150 & -0x1000;

    if (!(entry & 0x400)) {
        if ((s16)theirs != (s16)mine) goto other;
    } else {
        if ((theirs & mine) == 0) goto other;
    }
    func_8013f2c8(object);
    return;
other:
    object->slots[(u8)index].field_00++;
    func_8013f0c8(object, (u8)index, (u8)arg);
    func_8013e84c(object, (u8)index, (u8)arg);
}
