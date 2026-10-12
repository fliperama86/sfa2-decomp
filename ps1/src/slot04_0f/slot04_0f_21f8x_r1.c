/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The table of signed halfwords: a local declaration, inferred. */
extern s16 data_801c5ae4_slot04_0f[];

/* What it does (inferred, not an original name): the object's byte index
   selects a signed halfword of a table (data_801c5ae4_slot04_0f, the byte
   with its lowest bit cleared is the byte offset) and is cleared. The
   halfword is negated when the object's facing_side is not 0. The object's
   pos_x_frac is cleared. A 32-bit value is then made whose high half is
   (field_332 + that halfword) minus the object's position x, as a 16-bit
   quantity. The object's vel_x receives that value shifted right by 4,
   arithmetically.
   The low half of that 32-bit value is what the function finds in the third
   argument register on entry: the original never sets it. Its two callers
   pass one argument, the object, so the low 12 bits of vel_x depend on what
   earlier code left in that register. The definition has three parameters
   for that reason: the second is used as a local (its incoming value is not
   read), the third is the value read. On another machine than the console
   the low 12 bits of vel_x are not defined by this text. */
void func_801b21f8_slot04_0f(Object *o, int a, int pos) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;
    int index;

    index = obj->index;
    obj->index = 0;
    a = *(u16 *)((u8 *)data_801c5ae4_slot04_0f + (index & 0xfe)) << 16;
    if (o->facing_side != 0) {
        a = ((u32)-a >> 16) << 16;
    }
    pos &= 0xffff;
    pos |= (obj->field_332 + (a >> 16)) << 16;
    o->pos_x_frac = 0;
    d = (pos >> 16) - (u16)o->pos_x;
    pos &= 0xffff;
    pos |= d << 16;
    o->vel_x = pos >> 4;
}
