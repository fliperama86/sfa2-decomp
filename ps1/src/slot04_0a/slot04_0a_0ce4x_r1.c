/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The local b holds the byte at 0x129, then the value that is stored; a holds the byte at 0x21a. Written with one local for both, this function differs from the original in 7 instruction slots. */
int func_801b0ce4_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = o->field_21a;
    int b = o->field_129;
    a = a != 0;
    a = a << 2;
    if (b != 0) a += 2;
    b = a;
    obj->field_102 = b;
    return 1;
}
