/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Block172 *ref_third;

/* Exact. The stores at offsets 0x2c and 0x30 are 32-bit in the original, but
   Object has field_2c as u8 and field_32 at 0x32, so a u32 field cannot be
   added there; both are written through a u32 cast. Needs a decision on the
   layout of 0x2c..0x33 (see report). */
void func_8011ef88(Object *o) {
    u8 *p;
    u8 z;
    func_80119b34(o);
    o->field_04 = 0;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
    o->field_28 = 0;
    *(u32 *)&o->field_2c = 0;
    *(u32 *)((u8 *)o + 0x30) = 0;
    o->field_34 = 0;
    o->field_45 = 0;
    o->field_6a = 0;
    o->field_69 = 0;
    o->field_bc = 0;
    o->field_bd = 0;
    o->field_be = 0;
    o->field_bf = 0;
    o->field_7e = 0;
    z = 0;
    for (p = &o->field_128; p != (u8 *)o + 0x391; p++) {
        *p = z;
    }
}
