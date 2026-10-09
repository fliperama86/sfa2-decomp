/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b7d54_slot04_sel[];
extern u16 *data_801b7d5c_slot04_sel[];
extern u16 data_801b7d64_slot04_sel[];

void func_801b31f4_slot04_sel(Object *obj, Slot04SelRec9d38 *r) {
    if (r->field_0e == 0) {
        if (r->field_0f == 0) {
            if (obj->kind == 2 && *data_801b7d54_slot04_sel[obj->side] == 0x100) {
                r->field_0f = 0xff;
            }
        } else if (*data_801b7d5c_slot04_sel[obj->side] != 0x100) {
            r->field_0f = 0;
            r->field_0e = 0xff;
        }
    }
    if (r->field_0e != 0) {
        u16 v = *data_801b7d54_slot04_sel[obj->side];
        if (v != 0) {
            if (v == data_801b7d64_slot04_sel[r->field_10]) {
                u8 t = r->field_10 + 1;
                r->field_10 = t;
                if (t == 0xb) {
                    r->field_11 = 0xff;
                }
            } else {
                r->field_10 = 0;
                r->field_0e = 0;
            }
        }
    }
    if (r->field_11 != 0) {
        if ((*data_801b7d5c_slot04_sel[obj->side] & 0x100) != 0) {
            r->field_12 = 0xff;
        } else {
            r->field_12 = 0;
        }
    }
}
