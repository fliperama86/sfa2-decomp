/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8da8_slot04_sel[];

void update_kind_2_4_hold_counters_c_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *r) {
    if (obj->kind == 2 && (*data_801b8da8_slot04_sel[other->side] & 0x100) != 0) {
        u8 t = r->hold_count_b + 1;
        r->hold_count_b = t;
        if (t > 0xb4) {
            r->hold_count_b = 0xb4;
        }
    } else {
        r->hold_count_b = 0;
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
