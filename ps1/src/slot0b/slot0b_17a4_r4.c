/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e4d5c_slot0b[])(Object *);
extern void (*data_801e4d78_slot0b[])(Object *);

void release_object_record_5_slot0b(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void add_vel_x_and_accel_x_to_position_words_slot0b(Object *obj) {
    *(int *)&obj->pos_x_frac = *(int *)&obj->pos_x_frac + obj->vel_x;
    *(int *)&obj->pos_y_frac = *(int *)&obj->pos_y_frac + obj->accel_x;
}

void func_801e1b64_slot0b(void) {
    u8 *p = &data_801ae02c;
    if (*p != 0) {
        data_801e4d5c_slot0b[data_801ae02e]((Object *)p);
    }
}

void call_handler_by_state_0_2_slot0b(Object *obj) {
    data_801e4d78_slot0b[obj->state_0](obj);
}
