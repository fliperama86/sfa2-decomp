/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8da8_slot04_sel[];

void func_801b56e0_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *r) {
    if (obj->kind == 2 && (*data_801b8da8_slot04_sel[other->side] & 0x100) != 0) {
        u8 t = r->field_15 + 1;
        r->field_15 = t;
        if (t > 0xb4) {
            r->field_15 = 0xb4;
        }
    } else {
        r->field_15 = 0;
    }
    if (obj->kind == 4 && (*data_801b8da8_slot04_sel[other->side] & 0x100) != 0) {
        u8 t = r->field_14 + 1;
        r->field_14 = t;
        if (t > 0xb4) {
            r->field_14 = 0xb4;
        }
    } else {
        r->field_14 = 0;
    }
}
