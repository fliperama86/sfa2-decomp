/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_3_start_table_sequence_alt_slot04_0d(Object *obj);

extern u16 data_801c2768_slot04_0d[];
extern u8 data_801c2770_slot04_0d[];

void update_chained_sequence_by_pressed_bits_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;
    u16 t;

    update_sequence(o);
    if ((s16)o->step_flags >= 0) {
        a = o->pressed_bits;
        if ((a & *(u16 *)((u8 *)data_801c2768_slot04_0d + (o->direction_index & 0xfe))) != 0) {
            obj->active_flag_2 = 1;
        }
    } else {
        if (obj->active_flag_2 == 0 || (obj->active_flag_2 = 0, obj->table_index_3 == 2)) {
            advance_state_3_start_table_sequence_alt_slot04_0d(o);
        } else {
            obj->table_index_3 = obj->table_index_3 + 1;
            add_to_meter_clamped(o, 1);
            t = data_801c2770_slot04_0d[obj->table_index_3];
            a = 0x28;
            if (o->field_49 != 0) {
                a = 0x46;
            }
            a += t;
            set_sequence_by_side_and_mode(o, (u16)a);
        }
    }
}
