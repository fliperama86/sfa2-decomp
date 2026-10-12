/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_3_start_table_sequence_alt_slot04_15(Object *obj);

extern u8 data_801c2dec_slot04_15[];
extern u8 data_801c2df4_slot04_15[];

void update_sequence_chain_next_on_button_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    update_sequence(o);
    if ((s16)o->step_flags >= 0) {
        a = o->pressed_bits;
        if ((a & *(u16 *)(data_801c2dec_slot04_15 + (o->direction_index & 0xfe))) != 0) {
            obj->active_flag_2 = 1;
        }
    } else {
        if (obj->active_flag_2 == 0 || (obj->active_flag_2 = 0, obj->table_index_3 == 2)) {
            advance_state_3_start_table_sequence_alt_slot04_15(o);
        } else {
            u16 t;

            obj->table_index_3 = obj->table_index_3 + 1;
            add_to_meter_clamped(o, 1);
            t = data_801c2df4_slot04_15[obj->table_index_3];
            a = 0x28;
            if (o->field_49 != 0) {
                a = 0x46;
            }
            a += t;
            set_sequence_by_side_and_mode(o, (u16)a);
        }
    }
}
