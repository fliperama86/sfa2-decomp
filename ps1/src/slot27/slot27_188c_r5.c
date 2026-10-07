/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_80026958_slot27[];
extern Slot27Rec9338 data_80029338_slot27[];

void func_80011d90_slot27(void) {
    Slot27Rec9338 *r = &data_80029338_slot27[ref_other.p->side];
    Object *o = ref_other.p;

    if (o->kind == 2 && (*data_80026958_slot27[o->side] & 0x100) != 0) {
        u8 t = r->field_06 + 1;
        r->field_06 = t;
        if (t > 0xb4) {
            r->field_06 = 0xb4;
        }
    } else {
        r->field_06 = 0;
    }
    if (o->kind == 4 && (*data_80026958_slot27[o->side] & 0x100) != 0) {
        u8 t = r->field_05 + 1;
        r->field_05 = t;
        if (t > 0xb4) {
            r->field_05 = 0xb4;
        }
    } else {
        r->field_05 = 0;
    }
}
