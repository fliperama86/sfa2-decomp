/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b7d4c_slot04_sel[];

void update_kind_2_4_hold_counters_b_slot04_sel(Object *obj, Slot04SelRec9d38 *r) {
    if (obj->kind == 2 && (*data_801b7d4c_slot04_sel[obj->side] & 0x100) != 0) {
        u8 t = r->field_14 + 1;
        r->field_14 = t;
        if (t > 0xb4) {
            r->field_14 = 0xb4;
        }
    } else {
        r->field_14 = 0;
    }
    if (obj->kind == 4 && (*data_801b7d4c_slot04_sel[obj->side] & 0x100) != 0) {
        u8 t = r->count_2 + 1;
        r->count_2 = t;
        if (t > 0xb4) {
            r->count_2 = 0xb4;
        }
    } else {
        r->count_2 = 0;
    }
}
