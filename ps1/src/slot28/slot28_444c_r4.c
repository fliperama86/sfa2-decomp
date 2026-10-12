/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8002acb8_slot28[])(Object *);
extern SequenceStep *data_80029ef4_slot28[];
extern Object *data_8005187c_slot28[];

void func_80014854_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_8005187c_slot28[3], arg, data_80029ef4_slot28);
}

void func_80014884_slot28(Object *object) {
    data_8002acb8_slot28[object->state_0](object);
}

void increment_state_0_slot28(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void advance_sequence_set_pos_y_from_vel_y_slot28(Object *obj) {
    if (obj->table_index == 0) {
        advance_sequence_checked(obj);
    } else {
        advance_sequence_checked(obj);
        if ((obj->step_flags & 0x7fff) == 1) {
            obj->pos_y = *(u16 *)&obj->vel_y + 8;
        } else {
            obj->pos_y = *(u16 *)&obj->vel_y;
        }
    }
}
