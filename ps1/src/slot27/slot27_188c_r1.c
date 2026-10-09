/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Pair data_800267c4_slot27[];

void func_80011978_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    int i = obj->other->side;

    obj->pos_x = data_800267c4_slot27[i].first;
    obj->pos_y = data_800267c4_slot27[i].second;
}
