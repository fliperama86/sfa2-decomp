/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6c68_slot04_10[];

void set_table_value_when_flag_high_byte_clear_slot04_10(Object *obj) {
    s16 i;
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        i = 0;
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = (obj->direction_index >> 1) + 1;
        }
        ((Slot04bObj *)obj)->table_value = data_801c6c68_slot04_10[i];
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
    }
}
