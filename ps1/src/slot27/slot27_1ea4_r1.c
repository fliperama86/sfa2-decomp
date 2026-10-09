/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec9338 data_80029338_slot27[];
extern u16 *data_80026960_slot27[];
extern u16 *data_80026968_slot27[];
extern u16 data_80026970_slot27[];

void func_80011ea4_slot27(void) {
    Slot27Rec9338 *rec = &data_80029338_slot27[ref_other.p->side];
    Object *obj = ref_other.p;
    u16 v;

    if (rec->field_00 == 0) {
        if (rec->field_01 == 0) {
            if (obj->kind == 2 && *data_80026960_slot27[obj->side] == 0x100) {
                rec->field_01 = 0xff;
            }
        } else if (*data_80026968_slot27[obj->side] != 0x100) {
            rec->field_01 = 0;
            rec->field_00 = 0xff;
        }
    }
    if (rec->field_00 != 0) {
        v = *data_80026960_slot27[obj->side];
        if (v != 0) {
            if (v == data_80026970_slot27[rec->field_02]) {
                if (++rec->field_02 == 0xb) {
                    rec->field_03 = 0xff;
                }
            } else {
                rec->field_02 = 0;
                rec->field_00 = 0;
            }
        }
    }
    if (rec->field_03 != 0) {
        if (*data_80026968_slot27[obj->side] & 0x100) {
            rec->field_04 = 0xff;
        } else {
            rec->field_04 = 0;
        }
    }
}
