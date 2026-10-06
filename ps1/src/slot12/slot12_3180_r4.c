/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800243bc_slot12[])(Object *);
extern void (*data_800243c8_slot12[])(Object *);

void func_80013608_slot12(Object *obj) {
    Object *o = game_state.field_154;
    if (game_state.field_09 == 0 && game_state.field_2c == 0 && (s16)o->field_b2 != obj->field_48) {
        obj->field_48 = *(u8 *)&o->field_b2;
    }
}

void func_8001365c_slot12(Object *obj) {
    obj->field_01 = 0;
    game_state.field_2bd = 0;
    data_800243bc_slot12[data_8018f5a0->field_4c](obj);
}

void func_800136b0_slot12(Object *obj) {
    obj->field_01 = 0;
    game_state.field_2bd = 1;
    data_800243c8_slot12[data_8018f5a0->field_4c](obj);
}
