/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80024488_slot12[];
extern SequenceStep *data_800280a0_slot12[];

void update_countdown_then_advance_sequence_slot12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if ((s16)t < 0) {
            obj->state_1++;
            game_state.field_2bc = 9;
        }
    }
    advance_sequence_checked(obj);
}

void advance_state_0_on_2bc_10_and_sequence_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
}

void func_80014db0_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void func_80014dd0_slot12(Object *obj, int a) {
    char *base = (char *)0x8005b000;
    obj->field_90 = base;
    obj->field_90 = base + data_80024488_slot12[obj->field_60];
    set_sequence_from_table_5(obj, (a + obj->field_60 * 2) & 0xff, data_800280a0_slot12);
}
