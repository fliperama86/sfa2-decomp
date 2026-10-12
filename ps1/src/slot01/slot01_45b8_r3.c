/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80033ba4_slot01;
extern int data_80033b9c_slot01;
extern int data_80033ba0_slot01;
void set_parent_kind_and_load_kind_pointers_slot01(Object *obj, int arg);

void start_state_if_parent_side_bit_set_slot01(Object *obj) {
    u8 *m = &game_state.mode;
    Object *p = obj->parent;
    data_80033ba4_slot01.p = p;
    data_80033b9c_slot01 = *m | game_state.side_mask;
    if ((data_80033b9c_slot01 >> p->side) & 1) {
        obj->state_1 = 0;
        obj->field_01 = 1;
        obj->state_0++;
        data_80033ba0_slot01 = 0;
        if ((*m >> data_80033ba4_slot01.p->side) & 1) {
            data_80033ba0_slot01 = 1;
            obj->state_1++;
        }
        set_parent_kind_and_load_kind_pointers_slot01(obj, (u8)data_80033ba0_slot01);
    }
}

extern ObjectFn data_80015dd4_slot01[];
void call_handler_by_state_1_then_advance_slot01(Object *obj) {
    data_80015dd4_slot01[obj->state_1](obj);
    advance_sequence_checked(obj);
    set_flag_and_push_if_mode_8(obj);
}
