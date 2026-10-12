/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126bcc(GameState *state, Object *object) {
    s32 v;
    func_80126de8(state, object);
    if (func_80126e74(state, object) == 0) {
        func_80126cd0(state, object);
        return;
    }
    if (object->side == 0)
        state->side_bits_b &= 0xfe;
    else if (object->side == 1)
        state->side_bits_b &= 0xfd;
    v = add_bcd(object->score_2, data_80181534);
    object->score_2 = v;
    if (v > 0x9999998)
        object->score_2 = 0x9999999;
    object->step_index = 2;
    object->countdown_timer = 0x7f;
    object->db_flag = 1;
    object->score = object->score_2;
    object->phase_index = 0;
    object->cost_index++;
    func_80126778(state, object);
}
