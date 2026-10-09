/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b6eb4_slot04_sel[];
extern u16 *data_801b6ebc_slot04_sel[];
extern u16 data_801b6ec4_slot04_sel[];

void func_801b2008_slot04_sel(Object *obj, Slot04SelRec *rec) {
    u16 v;

    if (rec->field_12 == 0) {
        if (rec->field_13 == 0) {
            if (obj->kind == 2 && *data_801b6eb4_slot04_sel[obj->side] == 0x100) {
                rec->field_13 = 0xff;
            }
        } else if (*data_801b6ebc_slot04_sel[obj->side] != 0x100) {
            rec->field_13 = 0;
            rec->field_12 = 0xff;
        }
    }
    if (rec->field_12 != 0) {
        v = *data_801b6eb4_slot04_sel[obj->side];
        if (v != 0) {
            if (v == data_801b6ec4_slot04_sel[rec->field_14]) {
                if (++rec->field_14 == 0xb) {
                    rec->field_15 = 0xff;
                }
            } else {
                rec->field_14 = 0;
                rec->field_12 = 0;
            }
        }
    }
    if (rec->field_15 != 0) {
        if (*data_801b6ebc_slot04_sel[obj->side] & 0x100) {
            rec->field_16 = 0xff;
        } else {
            rec->field_16 = 0;
        }
    }
}
