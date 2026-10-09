/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed480_slot06_08[];

void func_801ea2f0_slot06_08(Object *obj) {
    int idx;
    int k;
    obj->field_81 = 4;
    obj->field_04++;
    obj->field_0f = 1;
    if (obj->field_03 != 0) {
        /* The 1 goes through a local: with the literal at the store two instruction slots differ. */
        k = 1;
        obj->field_4c = 0;
        obj->field_0d = 0;
        obj->field_0c = 0;
        obj->field_0a = 1;
        if (game_state.field_42 & 6) {
            obj->field_4c = k;
        }
    }
    if (obj->field_03 != 0) {
        obj->field_3c = &player_right;
    } else {
        obj->field_3c = &player_left;
    }
    idx = 4;
    if (obj->field_4c != 0) {
        idx = 5;
    }
    func_80130700(obj, data_801ed480_slot06_08[idx]);
}
