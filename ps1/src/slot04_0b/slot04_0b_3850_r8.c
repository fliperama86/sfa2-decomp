/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_from_other_side_table_slot04_0b(Object *o, int index);

void func_801b46b8_slot04_0b(Object *o) {
    o->state_1++;
    o->ot_depth = 0;
    o->pos_x = ref_other.p->pos_x;
    o->pos_y = ref_other.p->pos_y;
    set_sequence_from_other_side_table_slot04_0b(o, 0x19);
    func_8011ffdc(o);
}

void advance_state_when_ref_step_flag_set_slot04_0b(Object *o) {
    if (ref_other.p->step_flags & 1) {
        o->state_1 = 0;
        o->state_0++;
    } else {
        func_8011ffdc(o);
    }
}
