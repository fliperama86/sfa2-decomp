/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b6eb4_slot04_sel[];
extern u16 *data_801b6ebc_slot04_sel[];
extern u16 data_801b6ec4_slot04_sel[];

void update_button_sequence_matcher_slot04_sel(Object *obj, Slot04SelRec *rec) {
    u16 v;

    if (rec->slot_a_flag == 0) {
        if (rec->slot_b_flag == 0) {
            if (obj->kind == 2 && *data_801b6eb4_slot04_sel[obj->side] == 0x100) {
                rec->slot_b_flag = 0xff;
            }
        } else if (*data_801b6ebc_slot04_sel[obj->side] != 0x100) {
            rec->slot_b_flag = 0;
            rec->slot_a_flag = 0xff;
        }
    }
    if (rec->slot_a_flag != 0) {
        v = *data_801b6eb4_slot04_sel[obj->side];
        if (v != 0) {
            if (v == data_801b6ec4_slot04_sel[rec->sequence_index]) {
                if (++rec->sequence_index == 0xb) {
                    rec->slot_c_flag = 0xff;
                }
            } else {
                rec->sequence_index = 0;
                rec->slot_a_flag = 0;
            }
        }
    }
    if (rec->slot_c_flag != 0) {
        if (*data_801b6ebc_slot04_sel[obj->side] & 0x100) {
            rec->slot_d_flag = 0xff;
        } else {
            rec->slot_d_flag = 0;
        }
    }
}
