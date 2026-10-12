/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800243bc_slot12[])(Object *);
extern void (*data_800243c8_slot12[])(Object *);

void func_80013608_slot12(Object *obj) {
    Object *o = game_state.object;
    if (game_state.field_09 == 0 && game_state.state_2c_flag == 0 && (s16)o->field_b2 != obj->variant_index) {
        obj->variant_index = *(u8 *)&o->field_b2;
    }
}

void func_8001365c_slot12(Object *obj) {
    obj->field_01 = 0;
    game_state.mode_flag_2 = 0;
    data_800243bc_slot12[data_8018f5a0->step_index](obj);
}

void func_800136b0_slot12(Object *obj) {
    obj->field_01 = 0;
    game_state.mode_flag_2 = 1;
    data_800243c8_slot12[data_8018f5a0->step_index](obj);
}
