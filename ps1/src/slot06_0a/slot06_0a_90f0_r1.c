/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern ObjectFn data_801ed444_slot06_0a[];
extern void (*data_801ed454_slot06_0a[])(Object *, int, int);

void call_handler_by_state_0_slot06_0a(Object *obj) {
    data_801ed444_slot06_0a[obj->state_0](obj);
}

void init_object_velocity_from_position_set_sequence_slot06_0a(Object *obj) {
    obj->variant_index = 1;
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->field_0c = 0;
    obj->vel_x = *(s32 *)&obj->pos_x_frac;
    obj->vel_y = 0xf80000 - *(s32 *)&obj->pos_y_frac;
    set_sequence_entry(obj, data_801ed400_slot06_0a[2 + obj->table_index]);
}

void call_table_handler_with_bound_deltas_slot06_0a(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)data_801aa5d4;

    int x = *(s32 *)&l->bound_x_a - *(s32 *)&l->bound_x_c;
    int y = *(s32 *)&l->bound_y_a - *(s32 *)&l->bound_y_c;

    data_801ed454_slot06_0a[obj->table_index](obj, x, y);
    obj->variant_index = obj->variant_index ^ 1;
    if (obj->variant_index != 0) {
        clear_or_set_flag_by_pos_x_limit(obj);
    } else {
        obj->field_01 = 0;
    }
}
