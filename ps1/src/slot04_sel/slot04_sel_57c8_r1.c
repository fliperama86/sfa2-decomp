/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8db0_slot04_sel[];
extern u16 *data_801b8db8_slot04_sel[];
extern u16 data_801b8dc0_slot04_sel[];

void update_button_sequence_matcher_3_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *rec) {
    u16 v;

    if (rec->set_flag_a == 0) {
        if (rec->set_flag_b == 0) {
            if (obj->kind == 2 && *data_801b8db0_slot04_sel[other->side] == 0x100) {
                rec->set_flag_b = 0xff;
            }
        } else if (*data_801b8db8_slot04_sel[other->side] != 0x100) {
            rec->set_flag_b = 0;
            rec->set_flag_a = 0xff;
        }
    }
    if (rec->set_flag_a != 0) {
        v = *data_801b8db0_slot04_sel[other->side];
        if (v != 0) {
            if (v == data_801b8dc0_slot04_sel[rec->sequence_index]) {
                if (++rec->sequence_index == 0xb) {
                    rec->set_flag_c = 0xff;
                }
            } else {
                rec->sequence_index = 0;
                rec->set_flag_a = 0;
            }
        }
    }
    if (rec->set_flag_c != 0) {
        if (*data_801b8db8_slot04_sel[other->side] & 0x100) {
            rec->set_flag_d = 0xff;
        } else {
            rec->set_flag_d = 0;
        }
    }
}
