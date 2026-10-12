/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013e04_slot12(Object *obj);
u8 func_80014754_slot12(Object *obj);

void func_80014134_slot12(Object *obj) {
    func_80014754_slot12(obj);
    if (obj->field_f0 == 0) {
        data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
        game_state.field_2bc = 2;
        func_80013e04_slot12(obj);
        game_state.countdown_4 = 0;
    }
}

void func_800141a4_slot12(Object *obj) {
    data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
}

void func_800141c4_slot12(Object *obj) {
    if (game_state.field_2bc == 9) {
        data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
        game_state.field_2c0 = 0x32;
    }
}

void func_80014208_slot12(Object *obj) {
    s16 t = game_state.field_2c0;
    t -= 1;
    game_state.field_2c0 = t;
    if (t < 0) {
        if (data_80190949 == 0) {
            game_state.field_2bc = 10;
            data_8018f5a0->substep_index = 0;
            data_8018f5a0->step_index = data_8018f5a0->step_index + 1;
        }
    }
}
