/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The masked index is held twice: in the byte local slot, which the second
   half of the function uses, and written back to the parameter, which the
   first half uses. With slot everywhere and no write back, this function
   differs from the original in 16 instruction slots; with the parameter
   masked in place and used everywhere, in 4. The last argument of the third
   call goes through f: with (u8)arg passed there it differs in 5. */
void func_8013e1e4(Object *object, int index, int arg) {
    u8 slot = index;
    u16 entry;
    u16 m;
    u16 f;
    index = slot;
    object->slots[index].field_01 = 0;
    entry = table_8017a8cc[(u8)arg * 7];
    f = object->field_134;
    m = entry & 0xf0ff;
    if ((f & m) == 0 || (!(entry & 0x400) && m != (object->field_130 & 0xf0ff))) {
        func_8013f2a8(object, index, (u8)arg);
    } else {
        object->slots[slot].field_00++;
        if (entry & 1) {
            func_8013f2d8(object, slot, (u8)arg);
        } else {
            f = arg;
            func_8013f0c8(object, slot, (u8)f);
        }
    }
}
