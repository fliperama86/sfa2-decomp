/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot27Rec9338 data_80029338_slot27[];
extern u16 *data_80026960_slot27[];
extern u16 *data_80026968_slot27[];
extern u16 data_80026970_slot27[];

void update_input_sequence_match_record_slot27(void) {
    Slot27Rec9338 *rec = &data_80029338_slot27[ref_other.p->side];
    Object *obj = ref_other.p;
    u16 v;

    if (rec->slot_a == 0) {
        if (rec->slot_b == 0) {
            if (obj->kind == 2 && *data_80026960_slot27[obj->side] == 0x100) {
                rec->slot_b = 0xff;
            }
        } else if (*data_80026968_slot27[obj->side] != 0x100) {
            rec->slot_b = 0;
            rec->slot_a = 0xff;
        }
    }
    if (rec->slot_a != 0) {
        v = *data_80026960_slot27[obj->side];
        if (v != 0) {
            if (v == data_80026970_slot27[rec->step_count]) {
                if (++rec->step_count == 0xb) {
                    rec->slot_c = 0xff;
                }
            } else {
                rec->step_count = 0;
                rec->slot_a = 0;
            }
        }
    }
    if (rec->slot_c != 0) {
        if (*data_80026968_slot27[obj->side] & 0x100) {
            rec->slot_d = 0xff;
        } else {
            rec->slot_d = 0;
        }
    }
}
