/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80022de8_slot12[];

void move_y_by_table_until_countdown_ends_slot12(Object *obj) {
    int n;
    obj->pos_y += (s8)data_80022de8_slot12[(s16)obj->countdown];
    n = obj->countdown - 1;
    obj->countdown = n;
    if ((s16)n < 0) {
        obj->state_1++;
    }
}

void noop_slot00_slot12(Object *obj) {
}

void func_80011a58_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
