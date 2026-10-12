/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_80026958_slot27[];
extern Slot27Rec9338 data_80029338_slot27[];

void update_hold_counters_for_kind_2_and_4_slot27(void) {
    Slot27Rec9338 *r = &data_80029338_slot27[ref_other.p->side];
    Object *o = ref_other.p;

    if (o->kind == 2 && (*data_80026958_slot27[o->side] & 0x100) != 0) {
        u8 t = r->count_b + 1;
        r->count_b = t;
        if (t > 0xb4) {
            r->count_b = 0xb4;
        }
    } else {
        r->count_b = 0;
    }
    if (o->kind == 4 && (*data_80026958_slot27[o->side] & 0x100) != 0) {
        u8 t = r->count_a + 1;
        r->count_a = t;
        if (t > 0xb4) {
            r->count_a = 0xb4;
        }
    } else {
        r->count_a = 0;
    }
}
