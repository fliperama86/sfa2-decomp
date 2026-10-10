/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* The local w holds the halfword read from the list and later field_240 for
   its test. With field_240 tested in place, this function differs from the
   original in 4 instruction slots. The halfword is put together again in two
   statements; in one statement: 13. Its high byte is stored through t; stored
   straight from w >> 8: 6, and 8 bytes shorter. It reaches w through t; read
   straight into w: 3, and 4 bytes shorter. */
void func_8014a94c(Object *object) {
    u16 *p = data_80189460;
    Object *other;
    s16 w;
    u16 t;
    data_80189460 = p + 1;
    other = object->other;
    t = *p;
    w = t;
    t = w >> 8;
    object->field_210 = t;
    object->field_211 = w;
    w = other->field_240;
    if (w == 0 || other->field_14c == 0) {
        goto none;
    }
    ref_other.p = other->field_14c;
    if (ref_other.p->field_04 != 1) {
        goto none;
    }
    ref_second.p = other->field_14c;
    func_8014c4a8(object);
    t = object->field_211;
    t = t | (object->field_210 << 8);
    if (t >= data_80189464) {
    none:
        func_8014c914(object);
    } else {
        object->field_209 = 7;
        object->field_208 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}
