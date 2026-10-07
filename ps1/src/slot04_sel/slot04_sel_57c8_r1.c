/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8db0_slot04_sel[];
extern u16 *data_801b8db8_slot04_sel[];
extern u16 data_801b8dc0_slot04_sel[];

void func_801b57c8_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *rec) {
    u16 v;

    if (rec->field_0f == 0) {
        if (rec->field_10 == 0) {
            if (obj->kind == 2 && *data_801b8db0_slot04_sel[other->side] == 0x100) {
                rec->field_10 = 0xff;
            }
        } else if (*data_801b8db8_slot04_sel[other->side] != 0x100) {
            rec->field_10 = 0;
            rec->field_0f = 0xff;
        }
    }
    if (rec->field_0f != 0) {
        v = *data_801b8db0_slot04_sel[other->side];
        if (v != 0) {
            if (v == data_801b8dc0_slot04_sel[rec->field_11]) {
                if (++rec->field_11 == 0xb) {
                    rec->field_12 = 0xff;
                }
            } else {
                rec->field_11 = 0;
                rec->field_0f = 0;
            }
        }
    }
    if (rec->field_12 != 0) {
        if (*data_801b8db8_slot04_sel[other->side] & 0x100) {
            rec->field_13 = 0xff;
        } else {
            rec->field_13 = 0;
        }
    }
}
